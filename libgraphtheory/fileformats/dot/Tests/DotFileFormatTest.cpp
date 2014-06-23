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
#include "Data.h"
#include "Pointer.h"
#include <DataStructure.h>
#include <DataStructureBackendManager.h>
#include <QDebug>
#include <QUrl>

static const std::string simple = "digraph simple {a_2 -> b; c; d -> e /* error -> comment*/}";

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

void DotFileFormatTest::checkNodes(DataStructurePtr dataStructure, QList<QString> nodeNames)
{
    QList<DataPtr> dataList = dataStructure->dataList(0);

//     foreach(const DataPtr &node, dataList) {
//         qDebug() << node->property("name").toString();
//     }

    foreach(const DataPtr &node, dataList) {
        QString name = node->property("name").toString();
        int index = nodeNames.indexOf(name);
        if (index == -1) {
            qDebug() << "Node "<< name << " was created unnecessarily.";
        }
        QVERIFY(index != -1);
        nodeNames.removeAt(index);
    }
    QVERIFY(nodeNames.isEmpty());
}

void DotFileFormatTest::init()
{
    // test for graph data structure plugin
    if (DataStructureBackendManager::self().backends().count() == 0) {
        QFAIL("No plugin of DS, no way to continue!");
    }
    DataStructureBackendInterface *pl = DataStructureBackendManager::self().backend("Graph");
    QVERIFY2(pl,"Could create data structure of type Graph");
}

void DotFileFormatTest::simpleGraphParsing()
{
    Document doc("testSimple");
    QVERIFY(DotParser::parse(simple, &doc));

    QVERIFY(doc.dataStructures().count() == 1);
    if (doc.dataStructures().count() == 0) {
        return;
    }

    DataStructurePtr graph = doc.dataStructures().at(0);
    QVERIFY(graph->dataList(0).count() == 5);
    QVERIFY(graph->pointers(0).count() == 2);
}


void DotFileFormatTest::parseSubgraphs()
{
    Document doc("testSubgraphs");
    QVERIFY(DotParser::parse(subgraph, &doc));
}


void DotFileFormatTest::parseFileER()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("undirected/ER.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
    Document *doc = importer.graphDocument();
    DataStructurePtr dataStructure = doc->activeDataStructure();

    // Check that all of the node names were imported, and that there are no extras.
    QList<QString> nodeNames;
    nodeNames << "course" << "institute" << "student" << "name0" << "name1" << "name2" << "code" << "grade" << "number" << "C-I" << "S-C" << "S-I";
    checkNodes(dataStructure, nodeNames);

    // Check the numbers of pointers
    QVERIFY(dataStructure->pointers(0).count() == 12);

    // Check that a pointer has the correct label & that the node labels work.
    QList<DataPtr> dataList = dataStructure->dataList(0);
    DataPtr start;
    DataPtr end;
    foreach(const DataPtr &node, dataList) {
        QString name = node->property("name").toString();
        if (name == "student") {
            start = node;
        }
        if (name == "S-C") {
            end = node;
        }
        if (name == "name0" || name == "name1" || name == "name2") {
            QVERIFY(node->property("label").toString() == "name");
        }
    }
    PointerPtr betweenPtr = start->pointerList(end).at(0);
    QVERIFY(betweenPtr->property("label") == "m");
}


void DotFileFormatTest::parseFileHeawood()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("undirected/Heawood.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileNgk104()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("undirected/ngk10_4.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFilePetersen()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("undirected/Petersen.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileProcess()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("undirected/process.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
    Document* doc = importer.graphDocument();
    DataStructurePtr dataStructure = doc->activeDataStructure();
    // Check that all of the node names were imported, and that there are no extras.
    QList<QString> nodeNames;
    nodeNames << "run" << "intr" << "runbl" << "kernel" << "zombie" << "sleep" << "swap" << "runmem" << "runswap" << "new";
    checkNodes(dataStructure, nodeNames);
    // Check the numbers of pointers
    QVERIFY(dataStructure->pointers(0).count() == 13);
}


void DotFileFormatTest::parseFileAbstract()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/abstract.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileAlf()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/alf.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileArrows()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/arrows.gv"));
    importer.readFile();
    QEXPECT_FAIL("", "File contains invalid identifiers with underbar at beginning.", Continue);
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileAwilliams()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/awilliams.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileClust()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/clust.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileClust1()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/clust1.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileClust2()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/clust2.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileClust3()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/clust3.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileClust4()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/clust4.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileClust5()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/clust5.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileCrazy()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/crazy.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileCtext()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/ctext.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileDfa()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/dfa.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileFig6()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/fig6.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileFsm()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/fsm.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
    Document *doc = importer.graphDocument();
    DataStructurePtr dataStructure = doc->activeDataStructure();

    // Check that all of the node names were imported, and that there are no extras.
    QList<QString> nodeNames;
    nodeNames << "LR_0" << "LR_1" << "LR_2" << "LR_3" << "LR_4" << "LR_5" << "LR_6" << "LR_7" << "LR_8";
    checkNodes(dataStructure, nodeNames);

    // Check the numbers of pointers
    QVERIFY(dataStructure->pointers(0).count() == 14);

    // Check that a pointer has the correct label and that the shapes are correct.
    QList<DataPtr> dataList = dataStructure->dataList(0);
    QVERIFY(dataList.length() == 9); // Make the test quit because the parser has too many LR nodes.
    DataPtr start;
    DataPtr end;
    foreach(const DataPtr &node, dataList) {
        QString name = node->property("name").toString();
        if (name == "LR_0") {
            start = node;
            QVERIFY(node->property("shape").toString() == "doublecircle");
        }
        if (name == "LR_2") {
            end = node;
            QVERIFY(node->property("shape").toString() == "circle");
        }
    }
    PointerPtr betweenPtr = start->pointerList(end).at(0);
    QVERIFY(betweenPtr->property("label") == "SS(B)");
}


void DotFileFormatTest::parseFileKW91()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/KW91.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileLatin1()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/Latin1.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileNaN()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/NaN.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileGrammar()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/grammar.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileHashtable()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/hashtable.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileHondaTokoro()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/honda-tokoro.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileJapanese()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/japanese.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileJcctree()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/jcctree.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileJsort()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/jsort.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileLdbxtried()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/ldbxtried.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileLongflat()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/longflat.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileMike()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/mike.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileNhg()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/nhg.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileOldarrows()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/oldarrows.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFilePgram()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/pgram.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFilePm2way()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/pm2way.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFilePmpipe()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/pmpipe.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFilePolypoly()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/polypoly.gv"));
    importer.readFile();
    QEXPECT_FAIL("", "Not parsing with unknown reason: need to investigate further.", Continue);
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileProc3d()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/proc3d.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFilePsfonttest()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/psfonttest.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileRecord2()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/record2.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileRecords()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/records.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileRowe()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/rowe.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileRussian()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/russian.gv"));
    importer.readFile();

    QEXPECT_FAIL("", "Parsing of cyrillic characters for identifiers not yet supported.", Continue);
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileSdh()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/sdh.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileShells()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/shells.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileStates()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/states.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileStructs()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/structs.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileSwitch()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/switch.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileTable()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/table.gv"));
    importer.readFile();

    QEXPECT_FAIL("", "Parsing of interleaved XML tags not implemented", Continue);
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileTrain11()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/train11.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileTrapeziumlr()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/trapeziumlr.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileTree()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/tree.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileTriedds()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/triedds.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileTry()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/try.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileUnix()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/unix.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
    Document *doc = importer.graphDocument();
    DataStructurePtr dataStructure = doc->activeDataStructure();

    // check that all of the node names were imported, and that there are no extra nodes
    QList<QString> nodeNames;
    nodeNames << "5th Edition" << "6th Edition" << "PWB 1.0" << "LSX" << "1 BSD" << "Mini Unix"
        << "Wollongong" << "Interdata" << "Unix/TS 3.0" << "PWB 2.0" << "7th Edition"
        << "8th Edition" << "32V" << "V7M" << "Ultrix-11" << "Xenix" << "UniPlus+" << "9th Edition"
        << "2 BSD" << "2.8 BSD" << "2.9 BSD" << "3 BSD" << "4 BSD" << "4.1 BSD" << "4.2 BSD"
        << "4.3 BSD" << "Ultrix-32" << "PWB 1.2" << "USG 1.0" << "CB Unix 1" << "USG 2.0" <<
        "CB Unix 2" << "CB Unix 3" << "Unix/TS++" << "PDP-11 Sys V" << "USG 3.0" << "Unix/TS 1.0"
        << "TS 4.0" << "System V.0" << "System V.2" << "System V.3";
    checkNodes(dataStructure, nodeNames);

    // check number of pointers
    QVERIFY(dataStructure->pointers(0).count() == 49);
}


void DotFileFormatTest::parseFileUnix2()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/unix2.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileViewfile()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/viewfile.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileWorld()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("directed/world.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


QTEST_MAIN(DotFileFormatTest)
