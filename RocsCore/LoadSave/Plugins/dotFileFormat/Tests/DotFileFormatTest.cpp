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
#include <DataStructureBackendManager.h>
#include <KDebug>

static const std::string simple = "digraph simple {a -> b; c; d -> e /* error -> comment*/}";

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
    if (DataStructureBackendManager::self()->backends().count() == 0) {
        QFAIL("No plugin of DS, no way to continue!");
    }
    DataStructurePluginInterface * pl = DataStructureBackendManager::self()->backend("Graph");
    QVERIFY2(pl,"Could create data structure of type Graph");
}

void DotFileFormatTest::simpleGraphParsing()
{
    Document doc("testSimple");
    QVERIFY(DotParser::parse(simple, &doc));
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
    importer.setFile(KUrl::fromLocalFile("undirected/ER.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileHeawood()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("undirected/Heawood.gv"));
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


void DotFileFormatTest::parseFileAbstract()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/abstract.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileAlf()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/alf.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileArrows()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/arrows.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileAwilliams()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/awilliams.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileClust()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/clust.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileClust1()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/clust1.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileClust2()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/clust2.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileClust3()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/clust3.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileClust4()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/clust4.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileClust5()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/clust5.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileCrazy()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/crazy.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileCtext()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/ctext.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileDfa()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/dfa.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileFig6()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/fig6.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileFsm()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/fsm.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileKW91()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/KW91.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileLatin1()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/Latin1.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileNaN()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/NaN.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileGrammar()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/grammar.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileHashtable()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/hashtable.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileHondaTokoro()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/honda-tokoro.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileJapanese()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/japanese.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileJcctree()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/jcctree.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileJsort()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/jsort.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileLdbxtried()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/ldbxtried.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileLongflat()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/longflat.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileMike()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/mike.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileNhg()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/nhg.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileOldarrows()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/oldarrows.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFilePgram()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/pgram.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFilePm2way()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/pm2way.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFilePmpipe()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/pmpipe.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFilePolypoly()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/polypoly.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileProc3d()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/proc3d.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFilePsfonttest()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/psfonttest.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileRecord2()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/record2.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileRecords()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/records.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileRowe()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/rowe.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileRussian()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/russian.gv"));
    importer.readFile();

    QEXPECT_FAIL("", "Parsing of cyrillic characters for identifiers not yet supported.", Continue);
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileSdh()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/sdh.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileShells()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/shells.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileStates()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/states.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileStructs()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/structs.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileSwitch()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/switch.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileTable()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/table.gv"));
    importer.readFile();

    QEXPECT_FAIL("", "Parsing of interleaved XML tags not implemented", Continue);
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileTrain11()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/train11.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileTrapeziumlr()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/trapeziumlr.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileTree()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/tree.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileTriedds()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/triedds.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileTry()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/try.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileUnix()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/unix.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileUnix2()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/unix2.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileViewfile()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/viewfile.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


void DotFileFormatTest::parseFileWorld()
{
    // create importer plugin
    DotFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("directed/world.gv"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}


QTEST_MAIN(DotFileFormatTest)
