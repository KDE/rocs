/*
    This file is part of Rocs.
    Copyright 2006-2007  Gael de Chalendar <kleag@free.fr>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    Rocs is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation, version 2.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA
*/

#include "DotGrammar.h"
#include "DotGraphParsingHelper.h"
#include <Document.h>

#include <KDebug>
#include <QFile>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_int.hpp>
#include <boost/spirit/include/qi_real.hpp>
#include <boost/spirit/include/qi_string.hpp>
#include <boost/spirit/repository/include/qi_distinct.hpp>
#include <boost/spirit/repository/include/qi_confix.hpp>
#include <boost/throw_exception.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

using namespace std;
using namespace boost;
using namespace boost::spirit::qi;

using namespace DotParser;

#define KGV_MAX_ITEMS_TO_LOAD std::numeric_limits<size_t>::max()
#define BOOST_SPIRIT_DEBUG 1

DotGraphParsingHelper* phelper = 0;

// create distinct parser for dot keywords
namespace distinct
{
    //[qi_distinct_encapsulation
    namespace spirit = boost::spirit;
    namespace ascii = boost::spirit::ascii;
    namespace repo = boost::spirit::repository;

    // Define metafunctions allowing to compute the type of the distinct()
    // and ascii::char_() constructs
    namespace traits
    {
        // Metafunction allowing to get the type of any repository::distinct(...)
        // construct
        template <typename Tail>
        struct distinct_spec
          : spirit::result_of::terminal<repo::tag::distinct(Tail)>
        {};

        // Metafunction allowing to get the type of any ascii::char_(...) construct
        template <typename String>
        struct char_spec
          : spirit::result_of::terminal<spirit::tag::ascii::char_(String)>
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

    // Define a helper function allowing to create a ascii::char_() construct
    // from an arbitrary string representation
    template <typename String>
    inline typename traits::char_spec<String>::type
    char_spec(String const& str)
    {
        return ascii::char_(str);
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



    // grammar definition of the DOT language as specified by Graphviz DOT language specification,
    // see below:
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
    // Further specifications for keywords and identifier available in DOT documentation.
    // Usually availabel under: /usr/share/doc/graphviz/html/info/lang.html

    // An ID is one of the following:
    // Any string of alphabetic ([a-zA-Z'200-'377]) characters, underscores ('_') or digits ([0-9]), not beginning with a digit;
    // a numeral [-]?(.[0-9]+ | [0-9]+(.[0-9]*)? );
    // any double-quoted string ("...") possibly containing escaped quotes ('")1;
    // an HTML string (<...>).

using boost::phoenix::ref;
using boost::spirit::qi::_1;
using boost::spirit::qi::_val;

template <typename Iterator>
struct DotGrammar : boost::spirit::qi::grammar<Iterator, spirit::ascii::space_type> {

//TODO list for grammar
// * check if keywords node, edge, digraph... are parsed case-independent

    DotGrammar() : DotGrammar::base_type(graph) {

        graph = -distinct::keyword["strict"][&setStrict]
                >> (distinct::keyword["graph"][&undirectedDataStructure] | distinct::keyword["digraph"][&directedDataStructure])
                >> -ID[&dataStructureId]
                >> '{'
                >> stmt_list
                >> '}';

        stmt_list = stmt >> -char_(';') >> -stmt_list;

        stmt = (    edge_stmt
                    | node_stmt
                    | attr_stmt
                    | (ID >> '=' >> ID)
                    | subgraph
                );

        attr_stmt = ( (distinct::keyword["graph"][phoenix::ref(phelper->attributed)="graph"] >> attr_list[&setAttributedList])[&setDataStructureAttributes]
                    | (distinct::keyword["node"][phoenix::ref(phelper->attributed)="node"] >> attr_list[&setAttributedList])
                    | (distinct::keyword["edge"][phoenix::ref(phelper->attributed)="edge"] >> attr_list[&setAttributedList])
                    );

        attr_list = '[' >> -a_list >>']';

        a_list = (ID[&attributeId] >> -('=' >> ID[&valid]))[&addattr]
                 >> -(',' >> a_list);

        edge_stmt = (
                        (node_id[&edgebound] | subgraph) >> edgeRHS >> -(attr_list[phoenix::ref(phelper->attributed)="edge"])
                    )[&pushAttrList][&setAttributedList][&createPointers][&popAttrList];

        edgeRHS = edgeop[&checkedgeop] >> (node_id[&edgebound] | subgraph) >> -edgeRHS;

        node_stmt  = (
                         node_id[&createData] >> -attr_list
                     )[phoenix::ref(phelper->attributed)="node"][&pushAttrList][&setAttributedList][&setDataAttributes][&popAttrList];

        node_id = ID >> -port;

        port = (':' >> ID >> -(':' >> compass_pt))
               | (':' >> compass_pt);

        subgraph = -(distinct::keyword["subgraph"] >> -ID[&subDataStructureId])
                   >> char_('{')[&createSubDataStructure][&enterSubDataStructure][&pushAttrListC]
                   >> stmt_list
                   >> qi::char_('}')[&leaveSubDataStructure][&popAttrListC];

        compass_pt  = (distinct::keyword["n"] | distinct::keyword["ne"] | distinct::keyword["e"]
                    | distinct::keyword["se"] | distinct::keyword["s"] | distinct::keyword["sw"]
                    | distinct::keyword["w"] | distinct::keyword["nw"]);

        tag = '<' >>  qi::lexeme[+(qi::char_ - '>')] >>  '>';

        edgeop = spirit::ascii::string("->") | spirit::ascii::string("--");

        ID = qi::lexeme[
                 (+(spirit::ascii::alpha|spirit::ascii::digit|'_')) //TODO HEX values missing
                 | qi::double_                                      //TODO differs from DOT definition
                 | ('"' >>  *(qi::char_ - '"') >>  '"')
                 | ('<' >>  *(qi::char_ - '>')  >>  '>')            //TODO this is only an elementary tag parser
             ];

//OLD:
//         ID = (
//                 (((qi::char_ - spirit::ascii::punct) | '_') >> *((qi::char_ - spirit::ascii::punct) | '_'))
//                 | qi::double_
//                 | ('"' >>  *((qi::char_('\\') >> '"') | (qi::char_ - '"')) >>  '"')
//                 | ('<' >>  *((qi::char_  - '<' - '>') | tag) >>  '>')
//             );
    }

    qi::rule<Iterator,                spirit::ascii::space_type> graph;
    qi::rule<Iterator, std::string(), spirit::ascii::space_type> ID;
    qi::rule<Iterator, std::string(), spirit::ascii::space_type> tag;
    qi::rule<Iterator,                spirit::ascii::space_type> stmt_list;
    qi::rule<Iterator,                spirit::ascii::space_type> stmt;
    qi::rule<Iterator,                spirit::ascii::space_type> attr_stmt;
    qi::rule<Iterator,                spirit::ascii::space_type> attr_list;
    qi::rule<Iterator,                spirit::ascii::space_type> a_list;
    qi::rule<Iterator,                spirit::ascii::space_type> edge_stmt;
    qi::rule<Iterator, std::string(), spirit::ascii::space_type> edgeop;
    qi::rule<Iterator,                spirit::ascii::space_type> edgeRHS;
    qi::rule<Iterator,                spirit::ascii::space_type> node_stmt;
    qi::rule<Iterator, std::string(), spirit::ascii::space_type> node_id;
    qi::rule<Iterator, std::string(), spirit::ascii::space_type> port;
    qi::rule<Iterator,                spirit::ascii::space_type> subgraph;
    qi::rule<Iterator, std::string(), spirit::ascii::space_type> compass_pt;
};


void enterSubDataStructure()
{
    kWarning() << "Sub data structure creation: not implemented";
    if (phelper) {
//     phelper->z++;
//     if (phelper->z > phelper->maxZ)
//     {
//       phelper->maxZ = phelper->z;
//     }
    }
}


void leaveSubDataStructure()
{
    kWarning() << "Sub data structure creation: not implemented";
//     if (phelper) {
//     phelper->z--;
//     kDebug() << QString::fromStdString(phelper->subdataTypeid);
//         phelper->subdataTypeid.removeLast();
//     }
}


void setStrict()
{
    kDebug() << "Graphviz \"strict\" keyword is not implemented.";
}


void undirectedDataStructure()
{
    kDebug() << "Create new data structure of type: Graph undirected";
    if(!phelper->dataStructure) {
        DataStructurePtr dataStructure = phelper->gd->addDataStructure("");
        phelper->dataStructure = boost::static_pointer_cast<Rocs::GraphStructure>(dataStructure);
    }
    phelper->dataStructure->setDirected(false);
}


void directedDataStructure()
{
    kDebug() << "Create new data structure of type: Graph directed";
    if (!phelper->dataStructure) {
        DataStructurePtr dataStructure = phelper->gd->addDataStructure("");
        phelper->dataStructure = boost::static_pointer_cast<Rocs::GraphStructure>(dataStructure);
    }
    phelper->dataStructure->setDirected(true);
}


void dataStructureId(const std::string& str)
{
    QString name = QString::fromStdString(str);
    kDebug() << "Set data structure name: " << name;
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
    std::string id(str);
    if (id.size() > 0 && id[0] == '"') id = id.substr(1);
    if (id.size() > 0 && id[id.size() - 1] == '"') id = id.substr(0, id.size() - 1);
    phelper->attributeId = QString::fromStdString(id);
    phelper->valid = QString();
}


void subDataStructureId(const std::string& str)
{
    if (!phelper) {
        return;
    }
    std::string id(str);
    if (id.size() > 0 && id[0] == '"') {
        id = id.substr(1);
    }
    if (id.size() > 0 && id[id.size() - 1] == '"') {
        id = id.substr(0, id.size() - 1);
    }
//     phelper->subdataTypeid
    phelper->subdataTypeid.append(QString::fromStdString(id));
}

void valid(const std::string& str)
{
    if (!phelper) {
        return;
    }
    std::string id(str);
    if (id.size() > 0 && id[0] == '"') {
        id = id.substr(1);
    }
    if (id.size() > 0 && id[id.size() - 1] == '"') {
        id = id.substr(0, id.size() - 1);
    }
    phelper->valid = QString::fromStdString(id);
}

void addattr()
{
    if (!phelper) {
        return;
    }
    phelper->attributes.insert(phelper->attributeId, phelper->valid);
}

void pushAttrListC()
{
    pushAttrList();
}

void pushAttrList()
{
    if (!phelper) {
        return;
    }
    phelper->dataStructureAttributesStack.push_back(phelper->dataStructureAttributes);
    phelper->dataAttributesStack.push_back(phelper->dataAttributes);
    phelper->pointersAttributesStack.push_back(phelper->pointersAttributes);
}

void popAttrListC()
{
    popAttrList();
}

void popAttrList()
{
    if (!phelper) {
        return;
    }
    phelper->dataStructureAttributes = phelper->dataStructureAttributesStack.back();
    phelper->dataStructureAttributesStack.pop_back();
    phelper->dataAttributes = phelper->dataAttributesStack.back();
    phelper->dataAttributesStack.pop_back();
    phelper->pointersAttributes = phelper->pointersAttributesStack.back();
    phelper->pointersAttributesStack.pop_back();
}

void createData(const std::string& str)
{
    std::string id(str);
    if (!phelper || str.length()==0) {
        return;
    }
    kDebug() << "create data: " << QString::fromStdString(str);
    if (id.size() > 0 && id[0] == '"') {
        id = id.substr(1);
    }
    if (id.size() > 0 && id[id.size() - 1] == '"') {
        id = id.substr(0, id.size() - 1);
    }
    if (!phelper->dataMap.contains(QString::fromStdString(id))) {
        phelper->createData(QString::fromStdString(id));
    }
}

void createSubDataStructure()
{
    kWarning() << "Creation of sub data structures not implemented!";
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

void setAttributedList()
{
    if (!phelper) {
        return;
    }
    phelper->setAttributedList();
}

void checkedgeop(const std::string& str)
{
    if (!phelper) {
        return;
    }

    if (((phelper->dataStructure->directed()) && (str.compare("->") == 0)) ||
            ((!phelper->dataStructure->directed()) && (str.compare("--") == 0)))
    {
        return;
    }

    kError() << "Error: incoherent relation : directed = '" << phelper->dataStructure->directed() << "' and edge operator = '" << QString::fromStdString(str) << "'" << endl;
}

void edgebound(const std::string& str)
{
    if (!phelper) {
        return;
    }
    std::string id(str);
    kDebug() << "edgebound: " << QString::fromStdString(id);
    if (id.size() > 0 && id[0] == '"') id = id.substr(1);
    if (id.size() > 0 && id[id.size() - 1] == '"') id = id.substr(0, id.size() - 1);
    phelper->addEdgeBound(QString::fromStdString(id));
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
    using boost::spirit::qi::int_;
    using boost::spirit::qi::phrase_parse;
    using boost::spirit::qi::_1;
    using boost::spirit::ascii::space;
    using boost::phoenix::push_back;
    using boost::phoenix::ref;

    return phrase_parse(str.begin(), str.end(),
        //  Begin grammar
        (
            int_[push_back(ref(v), _1)]
                >> *(',' >> int_[push_back(ref(v), _1)])
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

    using boost::spirit::qi::_1;
    using boost::spirit::ascii::space;
    using boost::phoenix::ref;

    std::string input(str);
    std::string::iterator iter = input.begin();
    DotGrammar<std::string::iterator> r;

    //TODO reenable skip parser: (+spirit::ascii::space | spirit::repository::qi::confix("/*", "*/"))
    if (boost::spirit::qi::phrase_parse(iter, input.end(), r, spirit::ascii::space)) {
        kDebug() << "Complete dot file was parsed successfully.";
        return true;
    } else {
        kWarning() << "Dot file parsing failed. Unable to parse:";
        kWarning() << "///// FILE CONTENT BEGIN /////";
        kWarning() << QString::fromStdString(std::string(iter, input.end()));
        kWarning() << "///// FILE CONTENT END /////";
    }
    return false;
}

