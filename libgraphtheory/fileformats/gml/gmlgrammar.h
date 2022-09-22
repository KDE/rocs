/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2010 Wagner Reck <wagner.reck@gmail.com>
    SPDX-FileCopyrightText: 2012 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

/*
 * GraphViz dot Graph parsing grammar implemented with boost Spirit
 */

#ifndef GML_GRAMMAR_H
#define GML_GRAMMAR_H

#include "typenames.h"
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi.hpp>

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

template<typename Iterator>
struct roman : boost::spirit::qi::grammar<Iterator, unsigned()> {
    roman()
        : roman::base_type(start)
    {
        using ascii::char_;
        using qi::_1;
        using qi::_val;
        using qi::double_;
        using qi::eps;
        using qi::lexeme;

        start = List;
        List = -KeyValue >> *(+WhiteSpace >> KeyValue) >> *WhiteSpace;
        KeyValue = *(WhiteSpace) >> Key[&gotKey] >> +WhiteSpace >> Value[&gotValue];
        Key = (char_("a-zA-Z")[_val += _1] >> *char_("a-zA-Z0-9_")[_val += _1]);
        Value = -Sign[_val += _1] >> +char_("0-9")[_val += _1] >> -((char_('.')[_val += _1] >> +char_("0-9")[_val += _1])) | String[_val = _1]
            | char_('[')[beginList] >> *WhiteSpace >> List >> *WhiteSpace >> char_(']')[endList];
        String = lexeme[char_('"') >> *((char_ - '"') | char_('/'))[_val += _1] >> char_('"')];
        Sign = (char_('+') | char_('-'))[_val += _1];
        WhiteSpace = ascii::space;
    }

    boost::spirit::qi::rule<Iterator, unsigned()> start;
    boost::spirit::qi::rule<Iterator> List, KeyValue;
    boost::spirit::qi::rule<Iterator, std::string()> Key, Value, String, Sign;
    boost::spirit::qi::rule<Iterator> WhiteSpace;
};
}

#endif
