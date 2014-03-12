/*
    This file is part of Rocs.
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DotGrammar.h"
#include "DotGraphParsingHelper.h"
#include <Document.h>

#include <QDebug>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_int.hpp>
#include <boost/spirit/include/qi_real.hpp>
#include <boost/spirit/include/qi_string.hpp>
#include <boost/spirit/repository/include/qi_distinct.hpp>
#include <boost/spirit/repository/include/qi_confix.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

using namespace DotParser;

#define KGV_MAX_ITEMS_TO_LOAD std::numeric_limits<size_t>::max()

// get debug output for parser generator
#define BOOST_SPIRIT_DEBUG 1

// define skipper for spaces, c style comments, and c++ style comments
#define SKIPPER space | confix("//", eol)[*(char_ - eol)] | confix("/*", "*/")[*(char_ - "*/")]

// workaround for linking boost
void boost::throw_exception(std::exception const & e)
{
}

// create distinct parser for dot keywords
namespace distinct
{
    //[qi_distinct_encapsulation
    namespace spirit = boost::spirit;
    namespace standard = boost::spirit::standard;
    namespace repo = boost::spirit::repository;

    // Define metafunctions allowing to compute the type of the distinct()
    // and standard::char_() constructs
    namespace traits
    {
        // Metafunction allowing to get the type of any repository::distinct(...)
        // construct
        template <typename Tail>
        struct distinct_spec
          : spirit::result_of::terminal<repo::tag::distinct(Tail)>
        {};

        // Metafunction allowing to get the type of any standard::char_(...) construct
        template <typename String>
        struct char_spec
          : spirit::result_of::terminal<spirit::tag::standard::char_(String)>
        {};
    };

    // Define a helper function allowing to create a distinct() construct from
    // an arbitrary tail parser
    template <typename Tail>
    inline typename traits::distinct_spec<Tail>::type
    distinct_spec(Tail const& tail)
    {
        return repo::qi::distinct(tail);
    }

    // Define a helper function allowing to create a standard::char_() construct
    // from an arbitrary string representation
    template <typename String>
    inline typename traits::char_spec<String>::type
    char_spec(String const& str)
    {
        return standard::char_(str);
    }

    // the following constructs the type of a distinct_spec holding a
    // charset("0-9a-zA-Z_") as its tail parser
    typedef traits::char_spec<std::string>::type charset_tag_type;
    typedef traits::distinct_spec<charset_tag_type>::type keyword_tag_type;

    // Define a new Qi 'keyword' directive usable as a shortcut for a
    // repository::distinct(char_(std::string("0-9a-zA-Z_")))
    std::string const keyword_spec("0-9a-zA-Z_");
    keyword_tag_type const keyword = distinct_spec(char_spec(keyword_spec));
    //]
}

// The parser is implemented to fulfull exactly the DOT file specification. For details on the DOT
// file format see /usr/share/doc/graphviz/html/info/lang.html or the graphviz website. The used
// specification is as follows:
//
//   graph      :  [ strict ] (graph | digraph) [ ID ] '{' stmt_list '}'
//   stmt_list  :  [ stmt [ ';' ] [ stmt_list ] ]
//   stmt       :  node_stmt
//              |  edge_stmt
//              |  attr_stmt
//              |  ID '=' ID
//              |  subgraph
//   attr_stmt  :  (graph | node | edge) attr_list
//   attr_list  :  '[' [ a_list ] ']' [ attr_list ]
//   a_list     :  ID [ '=' ID ] [ ',' ] [ a_list ]
//   edge_stmt  :  (node_id | subgraph) edgeRHS [ attr_list ]
//   edgeRHS    :  edgeop (node_id | subgraph) [ edgeRHS ]
//   node_stmt  :  node_id [ attr_list ]
//   node_id    :  ID [ port ]
//   port       :  ':' ID [ ':' compass_pt ]
//              |  ':' compass_pt
//   subgraph   :  [ subgraph [ ID ] ] '{' stmt_list '}'
//   compass_pt :  (n | ne | e | se | s | sw | w | nw | c | _)
//
// Definition for the ID non-terminal: any of
// * any string of alphabetic ([a-zA-Z'200-'377]) characters, underscores ('_') or digits ([0-9]), not beginning with a digit;
// * a numeral [-]?(.[0-9]+ | [0-9]+(.[0-9]*)? );
// * any double-quoted string ("...") possibly containing escaped quotes ('")1;
// * an <A NAME=html>HTML string</a> (<...>).
//
// Current problems of the parser:
// * parsing of HTML/XML tags not completely implemented
// * use of non ascii identifiers can cause parser errors

namespace DotParser {

namespace phx = boost::phoenix;

using boost::phoenix::ref;
using boost::phoenix::push_back;
using boost::spirit::standard::alpha;
using boost::spirit::standard::digit;
using boost::spirit::standard::string;
using boost::spirit::standard::space;
using boost::spirit::qi::_1;
using boost::spirit::qi::_val;
using boost::spirit::qi::char_;
using boost::spirit::qi::eol;
using boost::spirit::qi::int_;
using boost::spirit::qi::lexeme;
using boost::spirit::qi::phrase_parse;
using boost::spirit::qi::rule;
using boost::spirit::qi::standard::space_type;
using boost::spirit::repository::qi::confix;

typedef BOOST_TYPEOF(SKIPPER) skipper_type;

DotGraphParsingHelper* phelper = 0;

template <typename Iterator, typename Skipper = space_type>
struct DotGrammar : boost::spirit::qi::grammar<Iterator, Skipper> {

    DotGrammar() : DotGrammar::base_type(graph) {

        graph = -distinct::keyword["strict"][&setStrict]
                >> (distinct::keyword["graph"][&undirectedDataStructure] | distinct::keyword["digraph"][&directedDataStructure])
                >> -ID[&dataStructureId]
                >> '{'
                >> stmt_list
                >> '}';

        stmt_list = stmt >> -char_(';') >> -stmt_list;

        stmt = (    (ID[&attributeId] >> '=' >> ID[&valid])[&applyAttributeList]
                    | attr_stmt
                    | edge_stmt
                    | node_stmt
                    | subgraph
                );

        attr_stmt = ( (distinct::keyword["graph"][phx::ref(phelper->attributed)="graph"] >> attr_list[&applyAttributeList])[&setDataStructureAttributes]
                    | (distinct::keyword["node"][phx::ref(phelper->attributed)="node"] >> attr_list[&applyAttributeList])
                    | (distinct::keyword["edge"][phx::ref(phelper->attributed)="edge"] >> attr_list[&applyAttributeList])
                    );

        attr_list = '[' >> -a_list >>']';

        a_list = (ID[&attributeId] >> -('=' >> ID[&valid]))[&insertAttributeIntoAttributeList]
                 >> -char_(',') >> -a_list;

        edge_stmt = (
                        (node_id[&edgebound] | subgraph) >> edgeRHS >> -(attr_list[phx::ref(phelper->attributed)="edge"])
                    )[&createAttributeList][&applyAttributeList][&createPointers][&removeAttributeList];

        edgeRHS = edgeop[&checkEdgeOperator] >> (node_id[&edgebound] | subgraph) >> -edgeRHS;

        node_stmt  = (
                         node_id[&createData] >> -attr_list
                     )[phx::ref(phelper->attributed)="node"][&createAttributeList][&applyAttributeList][&setDataAttributes][&removeAttributeList];

        node_id = ID >> -port;

        port = (':' >> ID >> -(':' >> compass_pt))
               | (':' >> compass_pt);

        subgraph = -(distinct::keyword["subgraph"] >> -ID[&subDataStructureId])
                   >> char_('{')[&createSubDataStructure][&createAttributeList]
                   >> stmt_list
                   >> char_('}')[&leaveSubDataStructure][&removeAttributeList];

        compass_pt  = (distinct::keyword["n"] | distinct::keyword["ne"] | distinct::keyword["e"]
                    | distinct::keyword["se"] | distinct::keyword["s"] | distinct::keyword["sw"]
                    | distinct::keyword["w"] | distinct::keyword["nw"]);

        edgeop = string("->") | string("--");

        ID = lexeme[
                // parse alpha-numberic sequence that is not a keyword
                ( !(distinct::keyword["graph"] | distinct::keyword["edge"] | distinct::keyword["node"])
                    >> char_("a-zA-Z0-9") >> *char_("a-zA-Z0-9_")
                )
                // parse number
                | (-char_('-') >> ('.' >> +digit) | (+digit >> -('.' >> *digit)))
                // parse anything that is in quotation marks
                | ('"' >>  *(char_ - '"') >>  '"')
                // parse XML attribute sequence
                | ('<' >>  *(char_ - '>')  >>  '>') //TODO xml parser does not parse interlaced tags
             ];
    }

    rule<Iterator,                Skipper> graph;
    rule<Iterator, std::string(), Skipper> ID;
    rule<Iterator,                Skipper> stmt_list;
    rule<Iterator,                Skipper> stmt;
    rule<Iterator,                Skipper> attr_stmt;
    rule<Iterator,                Skipper> attr_list;
    rule<Iterator,                Skipper> a_list;
    rule<Iterator,                Skipper> edge_stmt;
    rule<Iterator, std::string(), Skipper> edgeop;
    rule<Iterator,                Skipper> edgeRHS;
    rule<Iterator,                Skipper> node_stmt;
    rule<Iterator, std::string(), Skipper> node_id;
    rule<Iterator, std::string(), Skipper> port;
    rule<Iterator,                Skipper> subgraph;
    rule<Iterator, std::string(), Skipper> compass_pt;
};

void leaveSubDataStructure()
{
    if (!phelper) {
        return;
    }
    phelper->leaveSubDataStructure();
}

void setStrict()
{
    qWarning() << "Graphviz \"strict\" keyword is not implemented.";
}

void undirectedDataStructure()
{
    qDebug() << "Create new data structure of type: Graph undirected";
    if(!phelper->dataStructure) {
        DataStructurePtr dataStructure = phelper->gd->addDataStructure("");
        phelper->dataStructure = boost::static_pointer_cast<Rocs::GraphStructure>(dataStructure);
    }
    phelper->gd->pointerType(0)->setDirection(PointerType::Bidirectional);
}

void directedDataStructure()
{
    qDebug() << "Create new data structure of type: Graph directed";
    if (!phelper->dataStructure) {
        DataStructurePtr dataStructure = phelper->gd->addDataStructure("");
        phelper->dataStructure = boost::static_pointer_cast<Rocs::GraphStructure>(dataStructure);
    }
    phelper->gd->pointerType(0)->setDirection(PointerType::Unidirectional);
}

void dataStructureId(const std::string& str)
{
    QString name = QString::fromStdString(str);
    qDebug() << "Set data structure name: " << name;
    if (!phelper->dataStructure) {
        DataStructurePtr dataStructure = phelper->gd->addDataStructure(name);
        phelper->dataStructure = boost::static_pointer_cast<Rocs::GraphStructure>(dataStructure);
    }
    phelper->dataStructure->setName(name);
}

void attributeId(const std::string& str)
{
    if (!phelper) {
        return;
    }
    // remove quotation marks
    QString id = QString::fromStdString(str);
    if (id.endsWith('"')) {
        id.remove(id.length()-1, 1);
    }
    if (id.startsWith('"')) {
        id.remove(0, 1);
    }
    phelper->attributeId = id;
    phelper->valid.clear();
}

void subDataStructureId(const std::string& str)
{
    if (!phelper) {
        return;
    }
    // remove quotation marks
    QString id = QString::fromStdString(str);
    if (id.endsWith('"')) {
        id.remove(id.length()-1, 1);
    }
    if (id.startsWith('"')) {
        id.remove(0, 1);
    }
    phelper->setSubDataStructureId(id);
}

void valid(const std::string& str)
{
    if (!phelper) {
        return;
    }
    // remove quotation marks
    QString id = QString::fromStdString(str);
    if (id.endsWith('"')) {
        id.remove(id.length()-1, 1);
    }
    if (id.startsWith('"')) {
        id.remove(0, 1);
    }
    phelper->valid = id;
}

void insertAttributeIntoAttributeList()
{
    if (!phelper) {
        return;
    }
    phelper->unprocessedAttributes.insert(phelper->attributeId, phelper->valid);
}


void createAttributeList()
{
    if (!phelper) {
        return;
    }
    phelper->dataStructureAttributeStack.push_back(phelper->dataStructureAttributes);
    phelper->dataAttributeStack.push_back(phelper->dataAttributes);
    phelper->pointerAttributeStack.push_back(phelper->pointerAttributes);
}

void removeAttributeList()
{
    if (!phelper) {
        return;
    }
    phelper->dataStructureAttributes = phelper->dataStructureAttributeStack.back();
    phelper->dataStructureAttributeStack.pop_back();
    phelper->dataAttributes = phelper->dataAttributeStack.back();
    phelper->dataAttributeStack.pop_back();
    phelper->pointerAttributes = phelper->pointerAttributeStack.back();
    phelper->pointerAttributeStack.pop_back();
}

void createData(const std::string& str)
{
    QString id = QString::fromStdString(str);
    if (!phelper || id.length()==0) {
        return;
    }
    // remove quotation marks
    if (id.endsWith('"')) {
        id.remove(id.length()-1, 1);
    }
    if (id.startsWith('"')) {
        id.remove(0, 1);
    }
    if (!phelper->dataMap.contains(id)) {
        phelper->createData(id);
    }
}

void createSubDataStructure()
{
    if (!phelper) {
        return;
    }
    phelper->createSubDataStructure();
}

void setDataStructureAttributes()
{
    if (!phelper) {
        return;
    }
    phelper->setDataStructureAttributes();
}

void setDataAttributes()
{
    if (!phelper) {
        return;
    }
    phelper->setDataAttributes();
}

void applyAttributeList()
{
    if (!phelper) {
        return;
    }
    phelper->applyAttributedList();
}

void checkEdgeOperator(const std::string& str)
{
    if (!phelper) {
        return;
    }

    if (((phelper->gd->pointerType(0)->direction() == PointerType::Unidirectional) && (str.compare("->") == 0)) ||
            ((phelper->gd->pointerType(0)->direction() == PointerType::Bidirectional) && (str.compare("--") == 0)))
    {
        return;
    }

    qCritical() << "Error: incoherent edge direction relation" << endl;
}

void edgebound(const std::string& str)
{
    if (!phelper) {
        return;
    }
    // remove quotation marks
    QString id = QString::fromStdString(str);
    if (id.endsWith('"')) {
        id.remove(id.length()-1, 1);
    }
    if (id.startsWith('"')) {
        id.remove(0, 1);
    }
    phelper->addEdgeBound(id);
}

void createPointers()
{
    if (!phelper) {
        return;
    }
    phelper->createPointers();
}

bool parseIntegers(const std::string& str, std::vector<int>& v)
{
    return phrase_parse(str.begin(), str.end(),
        //  Begin grammar
        (
            int_[phx::push_back(phx::ref(v), _1)]
                >> *(',' >> int_[phx::push_back(phx::ref(v), _1)])
        )
        ,
        //  End grammar
        space);
}

bool parse(const std::string& str, Document * graphDoc)
{
    delete phelper;
    phelper = new DotGraphParsingHelper;
    phelper->gd = graphDoc;

    std::string input(str);
    std::string::iterator iter = input.begin();
    DotGrammar<std::string::iterator, skipper_type> r;

    if (phrase_parse(iter, input.end(), r, SKIPPER)) {
        qDebug() << "Complete dot file was parsed successfully.";
        return true;
    } else {
        qWarning() << "Dot file parsing failed. Unable to parse:";
        qDebug() << "///// FILE CONTENT BEGIN /////";
        qDebug() << QString::fromStdString(std::string(iter, input.end()));
        qDebug() << "///// FILE CONTENT END /////";
    }
    return false;
}

}

