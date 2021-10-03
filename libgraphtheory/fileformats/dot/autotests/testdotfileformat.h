/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2010 Wagner Reck <wagner.reck@gmail.com>
    SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef TESTDOTFILEFORMAT_H
#define TESTDOTFILEFORMAT_H

#include <QObject>
#include "typenames.h"


class DotFileFormatTest : public QObject
{
    Q_OBJECT

 private:
    /**
     * Check the nodes of the imported graph
     * \param dataStructure the imported DataStructure
     * \param nodeNames the original names of the nodes
     */
    void checkNodes(GraphTheory::GraphDocumentPtr document, QList<QString> nodeNames);

private Q_SLOTS:
    void init();
    void simpleGraphParsing();
    void parseSubgraphs();

    // parsing tests for undirected example graphs
    void parseFileER();
    void parseFileHeawood();
    void parseFileNgk104();
    void parseFilePetersen();
    void parseFileProcess();

    // parsing tests for directed example graphs
    void parseFileAbstract();
    void parseFileAlf();
    void parseFileArrows();
    void parseFileAwilliams();
    void parseFileClust();
    void parseFileClust1();
    void parseFileClust2();
    void parseFileClust3();
    void parseFileClust4();
    void parseFileClust5();
    void parseFileCrazy();
    void parseFileCtext();
    void parseFileDfa();
    void parseFileFig6();
    void parseFileFsm();
    void parseFileKW91();
    void parseFileLatin1();
    void parseFileNaN();
    void parseFileGrammar();
    void parseFileHashtable();
    void parseFileHondaTokoro();
    void parseFileJapanese();
    void parseFileJcctree();
    void parseFileJsort();
    void parseFileLdbxtried();
    void parseFileLongflat();
    void parseFileMike();
    void parseFileNhg();
    void parseFileOldarrows();
    void parseFilePgram();
    void parseFilePm2way();
    void parseFilePmpipe();
    void parseFilePolypoly();
    void parseFileProc3d();
    void parseFilePsfonttest();
    void parseFileRecord2();
    void parseFileRecords();
    void parseFileRowe();
    void parseFileRussian();
    void parseFileSdh();
    void parseFileShells();
    void parseFileStates();
    void parseFileStructs();
    void parseFileSwitch();
    void parseFileTable();
    void parseFileTrain11();
    void parseFileTrapeziumlr();
    void parseFileTree();
    void parseFileTriedds();
    void parseFileTry();
    void parseFileUnix();
    void parseFileUnix2();
    void parseFileViewfile();
    void parseFileWorld();

    // parsing of exported files
    void writeAndParseTest();
};

#endif
