/*
    This file is part of Rocs.
    Copyright 2010  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

/*
 * GraphViz dot Graph parsing grammar implemented with boost Spirit
 */

#ifndef GML_GRAMMAR_H
#define GML_GRAMMAR_H

#include "typenames.h"
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <iostream>
#include <string>

class Document;
class QString;

namespace GmlParser
{
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;


void gotKey(const std::string &key);

void gotValue(const std::string &Value);

void beginList();

void endList();

void t();
void t1();

bool parse(const QString &content, GraphTheory::GraphDocumentPtr document);

template <typename Iterator>
struct roman : boost::spirit::qi::grammar<Iterator, unsigned()> {
    roman() : roman::base_type(start) {
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
    Value = -Sign[_val += _1] >> +char_("0-9")[_val += _1] >> -((char_('.')[_val += _1] >> +char_("0-9")[_val += _1]))
            | String[_val = _1]
            | char_('[')[beginList] >> *WhiteSpace >> List >> *WhiteSpace >> char_(']')[endList] ;
    String = lexeme[char_('"') >> *((char_ - '"') | char_('/'))[_val += _1] >> char_('"')];
    Sign = (char_('+') | char_('-'))[_val += _1];
    WhiteSpace = ascii::space;
}

boost::spirit::qi::rule<Iterator, unsigned()> start;
boost::spirit::qi::rule<Iterator> List, KeyValue ;
boost::spirit::qi::rule<Iterator, std::string()> Key, Value, String, Sign;
boost::spirit::qi::rule<Iterator> WhiteSpace;
};
}

#endif
