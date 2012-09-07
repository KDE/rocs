/*
    This file is part of Rocs.
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef DOTFILEFORMATTEST_H
#define DOTFILEFORMATTEST_H

#include <QtCore/QObject>


class DotFileFormatTest : public QObject
{
    Q_OBJECT

private slots:
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
};

#endif // DOTFILEFORMATTEST_H
