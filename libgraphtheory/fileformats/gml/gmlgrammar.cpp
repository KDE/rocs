/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2010 Wagner Reck <wagner.reck@gmail.com>
    SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "gmlgrammar.h"
#include "gmlgrammarhelper.h"
#include "logging_p.h"
#include <QVariant>

#define KGV_MAX_ITEMS_TO_LOAD std::numeric_limits<size_t>::max()
#define BOOST_SPIRIT_DEBUG 1

using namespace GraphTheory;

// workaround for linking boost
namespace boost
{
void throw_exception(std::exception const &e)
{
    qCCritical(GRAPHTHEORY_FILEFORMAT) << "Exception:" << e.what();
}
}

namespace GmlParser
{

GmlGrammarHelper *phelper = nullptr;
std::string lastKey = std::string();
QObject *lastInserted = nullptr;

void beginList()
{
    phelper->startList(QString::fromStdString(lastKey));
}

void endList()
{
    phelper->endList();
}

void gotKey(const std::string &key)
{
    lastKey = key.c_str();
    //   QString k = key.c_str();
    //   if (k.compare("dataType", Qt::CaseInsensitive) == 0){
    //     qCDebug(GRAPHTHEORY_FILEFORMAT) << "create a graph";
    //     actualGraph =  document->addGraph("new");
    //     lastInserted = actualGraph;
    //   }else if (k.compare("node", Qt::CaseInsensitive) == 0){
    //     qCDebug(GRAPHTHEORY_FILEFORMAT) << "create a node";
    //     actualNode = actualGraph->addNode("new");
    //     lastInserted = actualNode;
    //   }else if (k.compare("edge", Qt::CaseInsensitive) == 0){
    //     qCDebug(GRAPHTHEORY_FILEFORMAT) << "create a edge";
    //   }else {
    //     qCDebug(GRAPHTHEORY_FILEFORMAT) << "Process attribute " << k;
    //     lastKey = key;
    //   }
}

void gotValue(const std::string &Value)
{
    if (Value.empty()) {
        return; // end of the list.
    } else {
        phelper->setAttribute(QString::fromStdString(lastKey), QString::fromStdString(Value));
        //     if (lastInserted){
        if (!lastInserted) {
            qCCritical(GRAPHTHEORY_FILEFORMAT) << "Cannot specify data node value: internal error";
            return;
        }
        if (lastKey == "id" && lastInserted) {
            lastInserted->setProperty("name", Value.c_str());
            phelper->nodeMap.insert(QString::fromStdString(Value), phelper->currentNode);
        }
        //       lastInserted->setProperty(lastKey.c_str(), Value.c_str());
        //     }else{
        //       qCDebug(GRAPHTHEORY_FILEFORMAT) << "ignoring keyvalue: "<< lastKey.c_str() << Value.c_str();
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

bool parse(const QString &content, GraphDocumentPtr document)
{
    QString tmpContent = content;
    unsigned result;
    phelper = new GmlGrammarHelper;
    phelper->document = document;
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
