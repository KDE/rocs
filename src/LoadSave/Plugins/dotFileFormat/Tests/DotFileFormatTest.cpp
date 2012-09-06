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

#include "DotFileFormatTest.h"
#include <QTest>
#include <string>
#include <Document.h>
#include "../DotGrammar.h"
#include "../DotFileFormatPlugin.h"
#include <DataStructure.h>
#include <DataStructurePluginManager.h>
#include <DynamicPropertiesList.h>
#include <KDebug>

static const std::string simple = "digraph simple {a -> b; c; d -> e}";

static const std::string subgraph = "digraph trees {"
                                        "  subgraph t {"
                                        "    0 -> \"1\" [label = \"A\"];"
                                        "    0 -> \"2\" [label = \"B\"];"
                                        "  }"
                                        "  subgraph u {"
                                        "    Animal -> Cat [label = \"feline\"];"
                                        "    Animal -> Dot [label = \"canine\"];"
                                        "  }"
                                        "}";

void DotFileFormatTest::init()
{
    // test for graph data structure plugin
    if (DataStructurePluginManager::self()->pluginsList().count() == 0) {
        QFAIL("No plugin of DS, no way to continue!");
    }
    DataStructurePluginInterface * pl = DataStructurePluginManager::self()->plugin("Graph");
    QVERIFY2(pl,"Could create data structure of type Graph");
}

void DotFileFormatTest::simpleGraphParsing()
{
    Document doc("testSimple");
    QVERIFY(parse(simple, &doc));
}


void DotFileFormatTest::parseSubgraphs()
{
    Document doc("testSubgraphs");
    QVERIFY(parse(subgraph, &doc));
}


void DotFileFormatTest::parseFileER()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("undirected/ER.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileHeadwood()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("undirected/Headwood.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileNgk104()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("undirected/ngk10_4.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFilePetersen()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("undirected/Petersen.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileProcess()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("undirected/process.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

QTEST_MAIN(DotFileFormatTest)
