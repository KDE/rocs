/* This file is part of KGraphViewer.
   Copyright (C) 2006-2007 Gael de Chalendar <kleag@free.fr>

   KGraphViewer is free software; you can redistribute it and/or
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
 * DataTypeViz dot DataType parsing grammar implemented with boost Spirit
 */

#ifndef DOT_GRAMMAR_H
#define DOT_GRAMMAR_H

// #include "dotrenderop.h"

#include <boost/throw_exception.hpp>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_distinct.hpp>
#include <boost/spirit/include/classic_loops.hpp>

#include <QPoint>
#include <QColor>
#include <QPair>
#include <QVector>

#include <map>
#include <list>
#include <string>
#include <sstream>

class DataTypeDocument;

bool parse(const std::string& str, DataTypeDocument* gd);

void gotid(char const* first, char const* last);
void dump(char const* first, char const* last);
void strict(char const* first, char const* last);
void undidataType(char const* first, char const* last);
void didataType(char const* first, char const* last);
void dataTypeid(char const* first, char const* last);
void attrid(char const* first, char const* last);
void subdataTypeid(char const* first, char const* last);
void valid(char const* first, char const* last);
void addattr(char const* first, char const* last);
void pushAttrListC(char const c);
void popAttrListC(char const c);
void pushAttrList(char const* first, char const* last);
void popAttrList(char const* first, char const* last);
void createsubdataType(char const);
void createnode(char const* first, char const* last);
void setdataTypeattributes(char const* first, char const* last);
void setsubdataTypeattributes(char const* first, char const* last);
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
bool parse_renderop(const std::string& str/*, DotRenderOpVec& arenderopvec*/);
bool parse_numeric_color(char const* str, QColor& c);

struct DotGrammar : public boost::spirit::classic::grammar<DotGrammar> {
    template <typename ScannerT>
    struct definition {
        definition(DotGrammar const& self);

        boost::spirit::classic::rule<ScannerT> dataType, ID, tag, stmt_list, stmt, attr_stmt,
              attr_list, a_list, edge_stmt, edgeop,
              edgeRHS, node_stmt, node_id,
              port, subdataType, compass_pt;

        boost::spirit::classic::rule<ScannerT> const& start() const {
            return dataType;
        }
    };

};

#endif



