/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "dotgrammar.h"
#include "dotgrammarhelper.h"
#include "graphdocument.h"
#include "node.h"
#include "logging_p.h"

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
using namespace GraphTheory;

#define KGV_MAX_ITEMS_TO_LOAD std::numeric_limits<size_t>::max()

// get debug output for parser generator
#define BOOST_SPIRIT_DEBUG 1

// define skipper for spaces, c style comments, and c++ style comments
#define SKIPPER space | confix("//", eol)[*(char_ - eol)] | confix("/*", "*/")[*(char_ - "*/")]

// workaround for linking boost
namespace boost {
void throw_exception(std::exception const &e)
{
    qCCritical(GRAPHTHEORY_FILEFORMAT) << "Exception:" << e.what();
}
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

// The parser is implemented to fulfill exactly the DOT file specification. For details on the DOT
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

DotGraphParsingHelper* phelper = nullptr;

template <typename Iterator, typename Skipper = space_type>
struct DotGrammar : boost::spirit::qi::grammar<Iterator, Skipper> {

    DotGrammar() : DotGrammar::base_type(graph) {

        graph = -distinct::keyword["strict"][&setStrict]
                >> (distinct::keyword["graph"][&setUndirected] | distinct::keyword["digraph"][&setDirected])
                >> -ID[&setGraphId]
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

        attr_stmt = ( (distinct::keyword["graph"][phx::ref(phelper->attributed)="graph"] >> attr_list[&applyAttributeList])[&setGraphAttributes]
                    | (distinct::keyword["node"][phx::ref(phelper->attributed)="node"] >> attr_list[&applyAttributeList])
                    | (distinct::keyword["edge"][phx::ref(phelper->attributed)="edge"] >> attr_list[&applyAttributeList])
                    );

        attr_list = '[' >> -a_list >>']';

        a_list = (ID[&attributeId] >> -('=' >> ID[&valid]))[&insertAttributeIntoAttributeList]
                 >> -char_(',') >> -a_list;

        edge_stmt = (
                        (node_id[&edgebound] | subgraph) >> edgeRHS >> -(attr_list[phx::ref(phelper->attributed)="edge"])
                    )[&createAttributeList][&applyAttributeList][&createEdge][&removeAttributeList];

        edgeRHS = edgeop[&checkEdgeOperator] >> (node_id[&edgebound] | subgraph) >> -edgeRHS;

        node_stmt  = (
                         node_id[&createNode] >> -attr_list
                     )[phx::ref(phelper->attributed)="node"][&createAttributeList][&applyAttributeList][&setNodeAttributes][&removeAttributeList];

        node_id = ID >> -port;

        port = (':' >> ID >> -(':' >> compass_pt))
               | (':' >> compass_pt);

        subgraph = -(distinct::keyword["subgraph"] >> -ID[&subGraphId])
                   >> char_('{')[&createSubGraph][&createAttributeList]
                   >> stmt_list
                   >> char_('}')[&leaveSubGraph][&removeAttributeList];

        compass_pt  = (distinct::keyword["n"] | distinct::keyword["ne"] | distinct::keyword["e"]
                    | distinct::keyword["se"] | distinct::keyword["s"] | distinct::keyword["sw"]
                    | distinct::keyword["w"] | distinct::keyword["nw"]);

        edgeop = string("->") | string("--");

        ID = lexeme[
                // parse alpha-numeric sequence that is not a keyword
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

void leaveSubGraph()
{
    if (!phelper) {
        return;
    }
    phelper->leaveSubGraph();
}

void setStrict()
{
    qCCritical(GRAPHTHEORY_FILEFORMAT) << "Graphviz \"strict\" keyword is not implemented.";
}

void setUndirected()
{
    phelper->document->edgeTypes().first()->setDirection(EdgeType::Bidirectional);
}

void setDirected()
{
    phelper->document->edgeTypes().first()->setDirection(EdgeType::Unidirectional);
}

void setGraphId(const std::string& str)
{
    QString name = QString::fromStdString(str);
    qCCritical(GRAPHTHEORY_FILEFORMAT) << "Graph ID not supported, _not_ setting: " << name;
    //TODO not implemented
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

void subGraphId(const std::string& str)
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
    phelper->setSubGraphId(id);
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
    phelper->graphAttributeStack.push_back(phelper->graphAttributes);
    phelper->nodeAttributeStack.push_back(phelper->nodeAttributes);
    phelper->edgeAttributeStack.push_back(phelper->edgeAttributes);
}

void removeAttributeList()
{
    if (!phelper) {
        return;
    }
    phelper->graphAttributes = phelper->graphAttributeStack.back();
    phelper->graphAttributeStack.pop_back();
    phelper->nodeAttributes = phelper->nodeAttributeStack.back();
    phelper->nodeAttributeStack.pop_back();
    phelper->edgeAttributes = phelper->edgeAttributeStack.back();
    phelper->edgeAttributeStack.pop_back();
}

void createNode(const std::string& str)
{
    QString label = QString::fromStdString(str);
    if (!phelper || label.length() == 0) {
        return;
    }
    // remove quotation marks
    if (label.endsWith('"')) {
        label.remove(label.length()-1, 1);
    }
    if (label.startsWith('"')) {
        label.remove(0, 1);
    }
    if (!phelper->nodeMap.contains(label)) {
        phelper->createNode(label);
    }
}

void createSubGraph()
{
    if (!phelper) {
        return;
    }
    phelper->createSubGraph();
}

void setGraphAttributes()
{
    if (!phelper) {
        return;
    }
    phelper->setDocumentAttributes();
}

void setNodeAttributes()
{
    if (!phelper) {
        return;
    }
    phelper->setNodeAttributes();
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

    if (((phelper->document->edgeTypes().first()->direction() == EdgeType::Unidirectional) && (str.compare("->") == 0)) ||
            ((phelper->document->edgeTypes().first()->direction() == EdgeType::Bidirectional) && (str.compare("--") == 0)))
    {
        return;
    }

    qCCritical(GRAPHTHEORY_FILEFORMAT) << "Error: incoherent edge direction relation";
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

void createEdge()
{
    if (!phelper) {
        return;
    }
    phelper->createEdge();
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

bool parse(const std::string& str, GraphDocumentPtr document)
{
    delete phelper;
    phelper = new DotGraphParsingHelper;
    phelper->document = document;

    std::string input(str);
    std::string::iterator iter = input.begin();
    DotGrammar<std::string::iterator, skipper_type> r;

    if (phrase_parse(iter, input.end(), r, SKIPPER)) {
        qCDebug(GRAPHTHEORY_FILEFORMAT) << "Complete dot file was parsed successfully.";
        return true;
    } else {
        qCWarning(GRAPHTHEORY_FILEFORMAT) << "Dot file parsing failed. Unable to parse:";
        qCDebug(GRAPHTHEORY_FILEFORMAT) << "///// FILE CONTENT BEGIN /////";
        qCDebug(GRAPHTHEORY_FILEFORMAT) << QString::fromStdString(std::string(iter, input.end()));
        qCDebug(GRAPHTHEORY_FILEFORMAT) << "///// FILE CONTENT END /////";
    }
    return false;
}

}
