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

#include "GmlGrammar.h"

#include "GmlGraphParsingHelper.h"
#include "Document.h"

#include <QDebug>


#define KGV_MAX_ITEMS_TO_LOAD std::numeric_limits<size_t>::max()
#define BOOST_SPIRIT_DEBUG 1

// workaround for linking boost
void boost::throw_exception(std::exception const & e)
{
}

namespace GmlParser
{

GmlGraphParsingHelper* phelper = 0;
std::string lastKey = "";
QObject * lastInserted = 0;
DataStructurePtr actualdataType;
DataPtr actualNode;
PointerPtr actualEdge;
Document *document = 0;

void beginList()
{
    phelper->startList(QString::fromStdString(lastKey));
}

void endList()
{
    phelper->endList();
}

void gotKey(const std::string& key)
{
    lastKey = key.c_str();
//   QString k = key.c_str();
//   if (k.compare("dataType", Qt::CaseInsensitive) == 0){
//     qDebug() << "create a graph";
//     actualGraph =  document->addGraph("new");
//     lastInserted = actualGraph;
//   }else if (k.compare("node", Qt::CaseInsensitive) == 0){
//     qDebug() << "create a node";
//     actualNode = actualGraph->addNode("new");
//     lastInserted = actualNode;
//   }else if (k.compare("edge", Qt::CaseInsensitive) == 0){
//     qDebug() << "create a edge";
//   }else {
//     qDebug() << "Process atribute " << k;
//     lastKey = key;
//   }

}

void gotValue(const std::string& Value)
{
    if (Value.empty()) {
        return; //end of the list.
    } else {
        phelper->setAttribute(QString::fromStdString(lastKey), QString::fromStdString(Value));
//     if (lastInserted){
    if (!lastInserted) {
        qCritical() << "Cannot specify data node value: internal error";
        return;
    }
    if (lastKey == "id" && lastInserted){
        lastInserted->setProperty("name", Value.c_str());
        phelper->dataMap.insert(QString::fromStdString(Value), phelper->actualNode);
    }
//       lastInserted->setProperty(lastKey.c_str(), Value.c_str());
//     }else{
//       qDebug() << "ignoring keyvalue: "<< lastKey.c_str() << Value.c_str();
//     }
    }
}


void t()
{
    std::cout << "Found whitespace.\n";
}
void t1(const std::string &key)
{
    std::cout << "Found " << key << ".\n";
}

bool parse(const QString& content, Document * doc)
{
    QString tmpContent = content;
    unsigned result;
    phelper = new GmlGraphParsingHelper;
    phelper->gd = doc;
    typedef std::string::const_iterator iterator_type;
    typedef GmlParser::roman<iterator_type> roman;

    roman roman_parser; // Our grammar

    int index;
    while ((index = tmpContent.indexOf('#')) != -1) {
        tmpContent.remove(index, tmpContent.indexOf('\n', index) - index);
    }
    std::string str = tmpContent.toStdString();
    iterator_type iter = str.begin();
    iterator_type end = str.end();

    bool r = parse(iter, end, roman_parser, result);

    if (r && iter == end) {
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded\n";
        std::cout << "result = " << result << std::endl;
        std::cout << "-------------------------\n";
    } else {
        std::string rest(iter, end);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \": " << rest << "\"\n";
        std::cout << "-------------------------\n";
    }

    delete phelper;
    return r;
}
}
