/* This file is part of Rocs.
   Copyright (C) 2010 Wagner Reck <wagner.reck@gmail.com>

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

/*
 * GraphViz dot Graph parsing grammar implemented with boost Spirit
 */

#ifndef GML_GRAMMAR_H
#define GML_GRAMMAR_H

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <iostream>
#include <string>

class DataTypeDocument;
class QString;

namespace Rocs{
namespace GMLPlugin
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;


    void gotKey(const std::string &key);

    void gotValue(const std::string &Value);

    void beginList();


    void endList();

    void t();
    void t1();

    bool parse(QString& content, DataTypeDocument* doc);

    template <typename Iterator>
    struct roman : boost::spirit::qi::grammar<Iterator, unsigned()>
    {
        roman() : roman::base_type(start)
        {
            using qi::eps;
            using qi::double_;
            using qi::lexeme;
            using qi::_val;
            using ascii::char_;
            using qi::_1;

            start = List;
            List = -KeyValue >> *(+WhiteSpace >> KeyValue) >> *WhiteSpace;
            KeyValue = *(WhiteSpace) >> Key[&gotKey] >> +WhiteSpace >> Value[&gotValue] ;
            Key = (char_("a-zA-Z")[_val += _1] >> *char_("a-zA-Z0-9_")[_val += _1]);
            Value = -Sign[_val += _1] >> +char_("0-9")[_val += _1] >> -(( char_('.')[_val += _1] >> +char_("0-9")[_val += _1]))
                    | String[_val = _1]
                    | char_('[')[beginList] >> *WhiteSpace >> List >> *WhiteSpace>> char_(']')[endList] ;
            String = lexeme[char_('"') >> *((char_ - '"') | char_('/'))[_val += _1] >> char_('"')];
            Sign = (char_('+')|char_('-'))[_val += _1];
            WhiteSpace = ascii::space;
        }

        boost::spirit::qi::rule<Iterator, unsigned()> start;
        boost::spirit::qi::rule<Iterator> List, KeyValue ;
        boost::spirit::qi::rule<Iterator, std::string()> Key, Value, String, Sign;
        boost::spirit::qi::rule<Iterator> WhiteSpace/*, Comments*/;
    };
}
}
// // #include "dotrenderop.h"
//
// #include <boost/throw_exception.hpp>
// #include <boost/spirit/include/classic_core.hpp>
// #include <boost/spirit/include/classic_distinct.hpp>
// #include <boost/spirit/include/classic_loops.hpp>
//
// #include <QPoint>
// #include <QColor>
// #include <QPair>
// #include <QVector>
//
// #include <map>
// #include <list>
// #include <string>
// #include <sstream>
/*
class GraphDocument;

bool parse(const std::string& str, GraphDocument* gd);

void gotid(char const* first, char const* last);
void dump(char const* first, char const* last);
void strict(char const* first, char const* last);
void undigraph(char const* first, char const* last);
void digraph(char const* first, char const* last);
void graphid(char const* first, char const* last);
void attrid(char const* first, char const* last);
void subgraphid(char const* first, char const* last);
void valid(char const* first, char const* last);
void addattr(char const* first, char const* last);
void pushAttrListC(char const c);
void popAttrListC(char const c);
void pushAttrList(char const* first, char const* last);
void popAttrList(char const* first, char const* last);
void createsubgraph(char const);
void createnode(char const* first, char const* last);
void setgraphattributes(char const* first, char const* last);
void setsubgraphattributes(char const* first, char const* last);
void setnodeattributes(char const* first, char const* last);
void setattributedlist(char const* first, char const* last);
void checkedgeop(char const* first, char const* last);
void edgebound(char const* first, char const* last);
void createedges(char const* first, char const* last);
void incrz(char const);
void decrz(char const);
void finalactions(char const* first, char const* last);

bool parse_point(char const* str, QPoint& p);
bool parse_real(char const* str, double& d);
bool parse_integers(char const* str, std::vector<int>& v);
bool parse_spline(char const* str, QVector< QPair< float, float > >& points);
void init_op();
void valid_op(char const* first, char const* last);
bool parse_renderop(const std::string& str, DotRenderOpVec& arenderopvec);
bool parse_numeric_color(char const* str, QColor& c);

struct DotGrammar : public boost::spirit::classic::grammar<DotGrammar>
{
  template <typename ScannerT>
  struct definition
  {
    definition(DotGrammar const& self);

    boost::spirit::classic::rule<ScannerT> graph, ID, tag, stmt_list, stmt, attr_stmt,
    attr_list, a_list, edge_stmt, edgeop,
    edgeRHS, node_stmt, node_id,
    port, subgraph, compass_pt;

    boost::spirit::classic::rule<ScannerT> const& start() const
    {
      return graph;
    }
  };

};*/

#endif



