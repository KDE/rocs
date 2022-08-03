/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2010 Wagner Reck <wagner.reck@gmail.com>
    SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "testdotfileformat.h"
#include "../dotfileformat.h"
#include "../dotgrammar.h"
#include "fileformats/fileformatinterface.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include "logging_p.h"
#include <string>
#include <QTest>
#include <QUrl>
#include <QTemporaryFile>

using namespace GraphTheory;

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

void DotFileFormatTest::checkNodes(GraphDocumentPtr document, QList<QString> nodeNames)
{
    const NodeList nodes = document->nodes();
    for (const NodePtr &node : nodes) {
        qCDebug(GRAPHTHEORY_FILEFORMAT) << "test NODE " << node->dynamicProperty("name").toString();
    }

    for (const NodePtr &node : nodes) {
        QString name = node->dynamicProperty("name").toString();
        int index = nodeNames.indexOf(name);
        if (index == -1) {
            qCDebug(GRAPHTHEORY_FILEFORMAT) << "Node "<< name << " was created unnecessarily.";
        }
        QVERIFY(index != -1);
        nodeNames.removeAt(index);
    }
    QVERIFY(nodeNames.isEmpty());
}

void DotFileFormatTest::init()
{

}

void DotFileFormatTest::simpleGraphParsing()
{
    GraphDocumentPtr document = GraphDocument::create();
    QVERIFY(DotParser::parse(simple, document));
    QVERIFY(document->nodes().count() == 5);
    QVERIFY(document->edges().count() == 2);
}

void DotFileFormatTest::parseSubgraphs()
{
    GraphDocumentPtr document = GraphDocument::create();
    QVERIFY(DotParser::parse(subgraph, document));
}

void DotFileFormatTest::parseFileER()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("undirected/ER.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
    GraphDocumentPtr document = importer.graphDocument();
    document->nodeTypes().first()->addDynamicProperty("label");

    // Check that all of the node names were imported, and that there are no extras.
    QList<QString> nodeNames;
    nodeNames << "course" << "institute" << "student" << "name0" << "name1" << "name2" << "code" << "grade" << "number" << "C-I" << "S-C" << "S-I";
    checkNodes(document, nodeNames);

    // Check the numbers of edges
    QVERIFY(document->edges().count() == 12);

    // Check that a pointer has the correct label & that the node labels work.
    const NodeList nodes = document->nodes();
    NodePtr from, to;
    for (const NodePtr &node : nodes) {
        QString name = node->dynamicProperty("label").toString();
        if (name == "student") {
            from = node;
        }
        if (name == "S-C") {
            to = node;
        }
        if (name == "name0" || name == "name1" || name == "name2") {
            QVERIFY(node->dynamicProperty("label").toString() == "name");
        }
    }
//     EdgePtr connection = from->edges(to).at(0); //FIXME API missing
//     QVERIFY(connection->property("label") == "m");
}

void DotFileFormatTest::parseFileHeawood()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("undirected/Heawood.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileNgk104()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("undirected/ngk10_4.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFilePetersen()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("undirected/Petersen.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileProcess()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("undirected/process.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
    GraphDocumentPtr document = importer.graphDocument();

    // check that all of the node names were imported, and that there are no extras
    QStringList nodeNames;
    nodeNames << "run" << "intr" << "runbl" << "kernel" << "zombie" << "sleep" << "swap" << "runmem" << "runswap" << "new";
    checkNodes(document, nodeNames);
    // Check the numbers of pointers
    QVERIFY(document->edges().count() == 13);
}

void DotFileFormatTest::parseFileAbstract()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/abstract.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileAlf()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/alf.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileArrows()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/arrows.gv"));
    importer.readFile();
    QEXPECT_FAIL("", "File contains invalid identifiers with underbar at beginning.", Continue);
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileAwilliams()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/awilliams.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileClust()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/clust.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileClust1()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/clust1.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileClust2()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/clust2.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileClust3()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/clust3.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileClust4()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/clust4.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileClust5()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/clust5.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileCrazy()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/crazy.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileCtext()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/ctext.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileDfa()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/dfa.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileFig6()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/fig6.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileFsm()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/fsm.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
    GraphDocumentPtr document = importer.graphDocument();

    // check that all of the node names were imported, and that there are no extras
    QStringList nodeNames;
    nodeNames << "LR_0" << "LR_1" << "LR_2" << "LR_3" << "LR_4" << "LR_5" << "LR_6" << "LR_7" << "LR_8";
    checkNodes(document, nodeNames);

    // Check the numbers of pointers
    QVERIFY(document->edges().count() == 14);

    // Check that a pointer has the correct label and that the shapes are correct.
    const NodeList nodes = document->nodes();
    QVERIFY(nodes.length() == 9); // Make the test quit because the parser has too many LR nodes.
    NodePtr from, to;
    for (const NodePtr &node : nodes) {
        QString name = node->property("name").toString();
        if (name == "LR_0") {
            from = node;
            QVERIFY(node->property("shape").toString() == "doublecircle");
        }
        if (name == "LR_2") {
            to = node;
            QVERIFY(node->property("shape").toString() == "circle");
        }
    }
//     EdgePtr connection = from->edges(to).at(0); //FIXME API missing
//     QVERIFY(connection->property("label") == "SS(B)");
}

void DotFileFormatTest::parseFileKW91()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/KW91.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileLatin1()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/Latin1.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileNaN()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/NaN.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileGrammar()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/grammar.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileHashtable()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/hashtable.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileHondaTokoro()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/honda-tokoro.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileJapanese()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/japanese.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileJcctree()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/jcctree.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileJsort()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/jsort.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileLdbxtried()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/ldbxtried.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileLongflat()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/longflat.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileMike()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/mike.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileNhg()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/nhg.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileOldarrows()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/oldarrows.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFilePgram()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/pgram.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFilePm2way()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/pm2way.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFilePmpipe()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/pmpipe.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFilePolypoly()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/polypoly.gv"));
    importer.readFile();
    QEXPECT_FAIL("", "Not parsing with unknown reason: need to investigate further.", Continue);
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileProc3d()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/proc3d.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFilePsfonttest()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/psfonttest.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileRecord2()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/record2.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileRecords()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/records.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileRowe()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/rowe.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileRussian()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/russian.gv"));
    importer.readFile();

    QEXPECT_FAIL("", "Parsing of Cyrillic characters for identifiers not yet supported.", Continue);
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileSdh()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/sdh.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileShells()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/shells.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileStates()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/states.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileStructs()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/structs.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileSwitch()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/switch.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileTable()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/table.gv"));
    importer.readFile();

    QEXPECT_FAIL("", "Parsing of interleaved XML tags not implemented", Continue);
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileTrain11()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/train11.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileTrapeziumlr()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/trapeziumlr.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileTree()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/tree.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileTriedds()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/triedds.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileTry()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/try.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileUnix()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/unix.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
    GraphDocumentPtr document = importer.graphDocument();

    // check that all of the node names were imported, and that there are no extra nodes
    QStringList nodeNames;
    nodeNames << "5th Edition" << "6th Edition" << "PWB 1.0" << "LSX" << "1 BSD" << "Mini Unix"
        << "Wollongong" << "Interdata" << "Unix/TS 3.0" << "PWB 2.0" << "7th Edition"
        << "8th Edition" << "32V" << "V7M" << "Ultrix-11" << "Xenix" << "UniPlus+" << "9th Edition"
        << "2 BSD" << "2.8 BSD" << "2.9 BSD" << "3 BSD" << "4 BSD" << "4.1 BSD" << "4.2 BSD"
        << "4.3 BSD" << "Ultrix-32" << "PWB 1.2" << "USG 1.0" << "CB Unix 1" << "USG 2.0" <<
        "CB Unix 2" << "CB Unix 3" << "Unix/TS++" << "PDP-11 Sys V" << "USG 3.0" << "Unix/TS 1.0"
        << "TS 4.0" << "System V.0" << "System V.2" << "System V.3";
    checkNodes(document, nodeNames);

    // check number of pointers
    QVERIFY(document->edges().count() == 49);
}

void DotFileFormatTest::parseFileUnix2()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/unix2.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileViewfile()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/viewfile.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::parseFileWorld()
{
    // create importer plugin
    DotFileFormat importer(this, KPluginMetaData(), QVariantList());
    importer.setFile(QUrl::fromLocalFile("directed/world.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

void DotFileFormatTest::writeAndParseTest()
{
    GraphDocumentPtr document = GraphDocument::create();
    QMap<QString, NodePtr> nodes;

    // create simple graph with two nodes and one edge
    nodes.insert("a", Node::create(document));
    nodes.insert("b", Node::create(document));
    Edge::create(nodes["a"], nodes["b"]);

    // create testfile
    DotFileFormat dotFormat(this, KPluginMetaData(), QVariantList());
    QTemporaryFile testFile;
    testFile.open();
    dotFormat.setFile(QUrl::fromLocalFile(testFile.fileName()));
    dotFormat.writeFile(document);

    // load document and test
    dotFormat.readFile();
    QVERIFY(!dotFormat.hasError());
    document = dotFormat.graphDocument();
    QCOMPARE(document->nodes().count(), 2);
    QCOMPARE(document->edges().count(), 1);
}

QTEST_MAIN(DotFileFormatTest)
