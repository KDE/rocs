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

#include <iostream>

#include <KDebug>

#include <QFile>

#include <boost/spirit/include/classic_confix.hpp>
#include <boost/throw_exception.hpp>

using namespace std;
using namespace boost;
using namespace boost::spirit::classic;

using namespace DotParser;

#define KGV_MAX_ITEMS_TO_LOAD std::numeric_limits<size_t>::max()
#define BOOST_SPIRIT_DEBUG 1

DotGraphParsingHelper* phelper = 0;

std::string therenderop;
std::string thestr;

void anychar(char const c);

// keyword_p for C++
// (for basic usage instead of std_p)
const boost::spirit::classic::distinct_parser<> keyword_p("0-9a-zA-Z_");

template <typename ScannerT>
DotGrammar::definition<ScannerT>::definition(DotGrammar const& /*self*/)
{
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

    // DOT file
    graph = (!(keyword_p("strict")[&strict])
            >> (keyword_p("graph")[&undirectedDataStructure] | keyword_p("digraph")[&directedDataStructure])
            >> !ID[&dataStructureId] >> ch_p('{') >> !stmt_list >> ch_p('}'))[&finalactions];
    stmt_list =  stmt >> !(ch_p(';')) >> !(stmt_list) ;
    stmt = ( node_stmt
           | edge_stmt
           | attr_stmt
           | (ID >> '=' >> ID)
           | subgraph
           );
    attr_stmt = ( (keyword_p("graph")[assign_a(phelper->attributed)] >> attr_list[&setAttributedList])[&setDataStructureAttributes]
                | (keyword_p("node")[assign_a(phelper->attributed)] >> attr_list[&setAttributedList])
                | (keyword_p("edge")[assign_a(phelper->attributed)] >> attr_list[&setAttributedList])
                ) ;
    attr_list = ch_p('[') >> !(a_list) >> ch_p(']');
    a_list = ((ID[&attributeId] >> !('=' >> ID[&valid]))[&addattr] >> !(',' >> a_list));
    edge_stmt = (
                    (node_id[&edgebound] | subgraph) >> edgeRHS >> !(attr_list[assign_a(phelper->attributed, "edge")])
                )[&pushAttrList][&setAttributedList][&createPointers][&popAttrList];
    edgeRHS  =  edgeop[&checkedgeop] >> (node_id[&edgebound] | subgraph) >> !(edgeRHS);
    node_stmt  = (
                     node_id[&createData] >> !(attr_list)
                 )[assign_a(phelper->attributed, "node")][&pushAttrList][&setAttributedList][&setDataAttributes][&popAttrList];
    node_id = (ID >> !(port));
    port = (ch_p(':') >> ID >> !(':' >> compass_pt))
           | (':' >> compass_pt);
    subgraph = (
                    !(keyword_p("subgraph") >> !(ID[&subDataStructureId]))
                    >> ch_p('{')[&createSubDataStructure][&incrz][&pushAttrListC]
                    >> stmt_list
                    >> ch_p('}') [&decrz][&popAttrListC]
               )
               | (keyword_p("subgraph") >> ID[&subDataStructureId]);
    compass_pt  = (keyword_p("n") | keyword_p("ne") | keyword_p("e")
                  | keyword_p("se") | keyword_p("s") | keyword_p("sw")
                  | keyword_p("w") | keyword_p("nw"));

    // Identifiers
    tag = ('<' >> *(anychar_p  - '>') >> '>');
    edgeop = str_p("->") | str_p("--");
    ID = (
             (((anychar_p - punct_p) | '_') >> *((anychar_p - punct_p) | '_'))
             | real_p
             | ('"' >>  *((ch_p('\\') >> '"') | (anychar_p - '"')) >>  '"')
             | (ch_p('<') >>  *((anychar_p  - '<' - '>') | tag) >>  '>')
         );
}




void incrz(char const /*first*/)
{
    if (phelper) {
//     phelper->z++;
//     if (phelper->z > phelper->maxZ)
//     {
//       phelper->maxZ = phelper->z;
//     }
    }
}

void anychar(char const /*c*/)
{
//   kDebug() << c;
}

void decrz(char const /*first*/)
{
    if (phelper) {
//     phelper->z--;
//     kDebug() << QString::fromStdString(phelper->subdataTypeid);
        phelper->subdataTypeid.removeLast();
    }
}

void dump(char const* first, char const* last)
{
    std::string str(first, last);
    kError() << ">>>> " << QString::fromStdString(str) << " <<<<" << endl;
}

void strict(char const* /*first*/, char const* /*last*/)
{
//   if (phelper) phelper;//->dataStructure->strict(true);
}

void gotid(char const* first, char const* last)
{
    std::string id(first, last);
//   kDebug() << "Got ID  = '"<<QString::fromStdString(phelper->attrid)<<"'";
}

void undirectedDataStructure(char const* /*first*/, char const* /*last*/)
{
    kDebug() << "Create new datastructure of type: Graph undirected";
    if(!phelper->dataStructure) {
        DataStructurePtr dataStructure = phelper->gd->addDataStructure("");
        phelper->dataStructure = boost::static_pointer_cast<Rocs::GraphStructure>(dataStructure);
    }
    phelper->dataStructure->setDirected(false);
}

void directedDataStructure(char const* /*first*/, char const* /*last*/)
{
    kDebug() << "Create new datastructure of type: Graph directed";
    if (!phelper->dataStructure) {
        DataStructurePtr dataStructure = phelper->gd->addDataStructure("");
        phelper->dataStructure = boost::static_pointer_cast<Rocs::GraphStructure>(dataStructure);
    }
    phelper->dataStructure->setDirected(true);
}

void dataStructureId(char const* first, char const* last)
{
    QString name = QString::fromStdString(std::string(first, last));
    kDebug() << "Set data structure name: " << name;
    if (!phelper->dataStructure) {

        DataStructurePtr dataStructure = phelper->gd->addDataStructure(name);
        phelper->dataStructure = boost::static_pointer_cast<Rocs::GraphStructure>(dataStructure);
    }
    phelper->dataStructure->setName(name);
}

void attributeId(char const* first, char const* last)
{
    if (phelper) {
        std::string id(first, last);
        if (id.size() > 0 && id[0] == '"') id = id.substr(1);
        if (id.size() > 0 && id[id.size() - 1] == '"') id = id.substr(0, id.size() - 1);
        phelper->attributeId = QString::fromStdString(id);
        phelper->valid = QString();
//     kDebug() << "Got attr ID  = '"<<QString::fromStdString(phelper->attrid)<<"'";
    }
}

void subDataStructureId(char const* first, char const* last)
{
    std::string id(first, last);
//   kDebug() << QString::fromStdString(id);
    if (phelper) {
        if (id.size() > 0 && id[0] == '"') id = id.substr(1);
        if (id.size() > 0 && id[id.size() - 1] == '"') id = id.substr(0, id.size() - 1);
//     phelper->subdataTypeid
        phelper->subdataTypeid.append(QString::fromStdString(id));
//     kDebug() << "Got subdataType id = '"<<phelper->subdataTypeid<<"'";
    }
}

void valid(char const* first, char const* last)
{
    if (!phelper) {
        return;
    }
    std::string id(first, last);
    if (id.size() > 0 && id[0] == '"') {
        id = id.substr(1);
    }
    if (id.size() > 0 && id[id.size() - 1] == '"') {
        id = id.substr(0, id.size() - 1);
    }
    phelper->valid = QString::fromStdString(id);
}

void addattr(char const* /*first*/, char const* /*last*/)
{
    if (phelper) {
        phelper->attributes.insert(phelper->attributeId, phelper->valid);
    }
}

void pushAttrListC(char const /*c*/)
{
    pushAttrList(0, 0);
}

void pushAttrList(char const* /*first*/, char const* /*last*/)
{
//   kDebug() << "Pushing attributes";
    if (phelper) {
        phelper->dataStructureAttributesStack.push_back(phelper->dataStructureAttributes);
        phelper->dataAttributesStack.push_back(phelper->dataAttributes);
        phelper->pointersAttributesStack.push_back(phelper->pointersAttributes);
    }
}

void popAttrListC(char const /*c*/)
{
    popAttrList(0, 0);
}

void popAttrList(char const* /*first*/, char const* /*last*/)
{
//   kDebug() << "Poping attributes";
    if (phelper) {
        phelper->dataStructureAttributes = phelper->dataStructureAttributesStack.back();
        phelper->dataStructureAttributesStack.pop_back();
        phelper->dataAttributes = phelper->dataAttributesStack.back();
        phelper->dataAttributesStack.pop_back();
        phelper->pointersAttributes = phelper->pointersAttributesStack.back();
        phelper->pointersAttributesStack.pop_back();
    }
//   kDebug() << "Poped";
}

void createData(char const* first, char const* last)
{
    if (!phelper || first == 0 || last == 0) {
        return;
    }
//   kDebug() << (void*)first << (void*)last << QString::fromStdString(std::string(first,last));
    std::string id(first, last);
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

void createSubDataStructure(char const /*c*/)
{
//   if (phelper)
//   {
//     phelper->createsubdataType();
//   }
}

void setDataStructureAttributes(char const* /*first*/, char const* /*last*/)
{
//   kDebug() << "setdataTypeattributes with z = " << phelper->z;
    if (phelper) {
//     if (phelper->z == 1) // main dataType
//     {
        phelper->setDataStructureAttributes();
//     }
//     else
//     {
//       phelper->setsubdataTypeattributes();
//     }
    }
}

void setDataAttributes(char const* /*first*/, char const* /*last*/)
{
//   kDebug() << "setnodeattributes with z = " << phelper->z;
    if (phelper) {
        phelper->setDataAttributes();
    }
}

void setAttributedList(char const* /*first*/, char const* /*last*/)
{
    if (phelper) {
        phelper->setAttributedList();
    }
}

void checkedgeop(char const* first, char const* last)
{
    std::string str(first, last);
    if (phelper) {
        if (((phelper->dataStructure->directed()) && (str == "->")) ||
                ((!phelper->dataStructure->directed()) && (str == "--")))
            return;

        kError() << "Error !! uncoherent relation : directed = '" << phelper->dataStructure->directed() << "' and op = '" << QString::fromStdString(str) << "'" << endl;
    }
}

void edgebound(char const* first, char const* last)
{
   kDebug() << "edgebound: " << QString::fromStdString(std::string(first,last));
    if (phelper) {
        std::string id(first, last);
        if (id.size() > 0 && id[0] == '"') id = id.substr(1);
        if (id.size() > 0 && id[id.size() - 1] == '"') id = id.substr(0, id.size() - 1);
        phelper->addEdgeBound(QString::fromStdString(id));
    }
}

void createPointers(char const* /*first*/, char const* /*last*/)
{
    if (phelper) {
        phelper->createPointers();
    }
}

void finalactions(char const* /*first*/, char const* /*last*/)
{
    if (phelper) {
        phelper->finalactions();
    }
}

bool parse_point(char const* str, QPoint& p)
{
    int x, y;
    bool res;
    res = parse(str,
                (
                    int_p[assign_a(x)] >> ',' >> int_p[assign_a(y)]
                )
                ,
                +space_p).full;
    if (!res) return false;
    p = QPoint(x, y);
    return true;
}

bool parse_numeric_color(char const* str, QColor& c)
{
    if (str == 0) return false;
    int r, g, b, a;
    bool res;
    uint_parser<unsigned, 16, 2, 2> hex2digits_p;
    res = parse(str,
                (
                    ch_p('#') >>
                    hex2digits_p[assign_a(r)] >>
                    hex2digits_p[assign_a(g)] >>
                    hex2digits_p[assign_a(b)] >>
                    !hex2digits_p[assign_a(a)]
                )
                ,
                +space_p).full;
    if (res) {
        c.setRgb(r, g, b);
        return true;
    }

    double h, s, v;
    res = parse(str,
                (
                    real_p[assign_a(h)] >> !ch_p(',') >> real_p[assign_a(s)] >> !ch_p(',') >> real_p[assign_a(v)]
                )
                ,
                +space_p).full;
    if (res) {
        c.setHsv(int(255 * h), int(255 * s), int(255 * v));
        return true;
    }
    return false;
}

bool parse_real(char const* str, double& d)
{
    return parse(str,
                 (
                     real_p[assign_a(d)]
                 )
                 ,
                 +space_p).full;
}

bool parse_integers(char const* str, std::vector<int>& v)
{
    return parse(str,
                 (
                     int_p[push_back_a(v)] >> *(',' >> int_p[push_back_a(v)])
                 )
                 ,
                 +space_p).full;
}

bool parse_spline(char const* str, QVector< QPair< float, float > >& points)
{
//   kDebug() << "Parsing spline..." << QString::fromStdString(str);
    char e = 'n', s = 'n';
    int sx, sy, ex, ey;
    QPair< float, float > p;
    bool res;
    res = parse(str,
                (
                    !(ch_p('e')[assign_a(e)] >> ',' >> int_p[assign_a(ex)] >> ',' >> int_p[assign_a(ey)])
                    >> !(ch_p('s')[assign_a(s)] >> ',' >> int_p[assign_a(sx)] >> ',' >> int_p[assign_a(sy)])
                    >> ((int_p[assign_a(p.first)] >> ',' >> int_p[assign_a(p.second)]))[push_back_a(points, p)]
                    >> +(
                        (int_p[assign_a(p.first)] >> ',' >> int_p[assign_a(p.second)])[push_back_a(points, p)] >>
                        (int_p[assign_a(p.first)] >> ',' >> int_p[assign_a(p.second)])[push_back_a(points, p)] >>
                        (int_p[assign_a(p.first)] >> ',' >> int_p[assign_a(p.second)])[push_back_a(points, p)]
                    )
                )
                ,
                +space_p).full;
    if (!res) return false;
    if (s == 's') {
//     kDebug() << "inserting the s point";
        points.insert(points.begin(), qMakePair(float(sx), float(sy)));
    }
    if (e == 'e') {
//     points.insert(points.begin(), qMakePair(float(ex),float(ey)));
    }
    return true;
}

// DotRenderOp renderop ;

// DotRenderOpVec* renderopvec = 0;

void init_op()
{
//   renderop = DotRenderOp();
}

void valid_op(char const* first, char const* last)
{
    std::string s(first, last);
//   renderop.renderop = QString::fromUtf8(therenderop.c_str());
//   renderop.str = QString::fromUtf8(thestr.c_str());

//   kDebug() << "Validating render operation '"<<QString::fromStdString(s)<<"': '"<<renderop.renderop<<"/"<<renderop.str<<"'";
//   renderopvec->push_back(renderop);
//   renderop.renderop = "";
//   renderop.integers = QList<int>();
//   renderop.str = "";
}

bool parse_renderop(const std::string& /*str*//*, DotRenderOpVec& arenderopvec*/)
{
    return false;
}
// //   kDebug() << QString::fromUtf8(str.c_str()) << str.size();
//   if (str.empty())
//   {
//     return false;
//   }
//   init_op();
//   renderopvec = &arenderopvec;
//   bool res;
//   int c;
//   res = parse(str.c_str(),
//               (
//                 +(
//                    (
//                      (ch_p('E')|ch_p('e'))[assign_a(therenderop)] >> +space_p >>
//                      repeat_p(4)[int_p[push_back_a(renderop.integers)] >> +space_p]
//                    )[&valid_op]
//                    | (
//                        (ch_p('P')|ch_p('p')|ch_p('L')|ch_p('B')|ch_p('b'))[assign_a(therenderop)] >> +space_p >>
//                        int_p[assign_a(c)][push_back_a(renderop.integers)] >> +space_p >>
//                        repeat_p(boost::ref(c))[
//                                                 int_p[push_back_a(renderop.integers)] >> +space_p >>
//                                                 int_p[push_back_a(renderop.integers)] >> +space_p
//                                               ]
//                      )[&valid_op]
//   // "T 1537 228 0 40 9 -#1 (== 0) T 1537 217 0 90 19 -MAIN:./main/main.pl "
//                    | (
//                        ch_p('T')[assign_a(therenderop)] >> +space_p >>
//                        int_p[push_back_a(renderop.integers)] >> +space_p >>
//                        int_p[push_back_a(renderop.integers)] >> +space_p >>
//                        int_p[push_back_a(renderop.integers)] >> +space_p >>
//                        int_p[push_back_a(renderop.integers)] >> +space_p >>
//                        int_p[assign_a(c)] >> +space_p >> '-' >>
//                        (repeat_p(boost::ref(c))[anychar_p])[assign_a(thestr)] >> +space_p
//                      )[&valid_op]
//   // c 9 -#000000ff
//                      | (
//                        (ch_p('C')|ch_p('c')|ch_p('S'))[assign_a(therenderop)] >> +space_p >>
//                        int_p[assign_a(c)] >> +space_p >> '-' >>
//                        (repeat_p(boost::ref(c))[anychar_p])[assign_a(thestr)] >> +space_p
//                      )[&valid_op]
//   // F 14,000000 11 -Times-Roman
//                     | (
//                        ch_p('F')[assign_a(therenderop)] >> +space_p >>
//                        int_p[push_back_a(renderop.integers)] >> (ch_p(',')|ch_p('.')) >> int_p >> +space_p >>
//                        int_p[assign_a(c)] >> +space_p >> '-' >>
//                        (repeat_p(boost::ref(c))[anychar_p])[assign_a(thestr)] >> +space_p
//                      )[&valid_op]
//                  )
//                  ) >> !end_p
//              ).full;
//   if (res ==false)
//   {
//     kError() << "ERROR: parse_renderop failed on "<< QString::fromStdString(str);
//     kError() << "       Last renderop string is "<<QString::fromStdString(str.c_str());
//   }
// //   delete renderop; renderop = 0;
//   return res;
// }

bool parse(const std::string& str, Document * graphDoc)
{
    DotGrammar g;
    delete phelper;
    phelper = new DotGraphParsingHelper;
    phelper->gd = graphDoc;

    return boost::spirit::classic::parse(str.c_str(), g >> end_p, (+boost::spirit::classic::space_p | boost::spirit::classic::comment_p("/*", "*/"))).full;
}

