/* 
    This file is part of Rocs.
    Copyright 2004-2010  Tomaz Canabrava <tomaz.canabrava@gmail.com>
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

#ifndef TESTDATASTRUCTURE_H
#define TESTDATASTRUCTURE_H

#include "Rocs_Typedefs.h"

#include <QObject>
#include <QVariant>
#include <QMap>

class Document;

class TestDataStructure : public QObject {
    Q_OBJECT
public:
    TestDataStructure();
    void createPentagon(bool directed = false);
    void create3x3(bool directed = false);
private slots:
    void cleanup();
    void initTestCase();
    void dataAddDeleteTest();
    void pointerAddDeleteTest();
    void cleanupTestCase();
    void createSimpleGraph();
//     void manipulateSimpleGraph();
//     void createDirectedGraph();
//     void manipulateDirectedGraph();
//     void saveTestFile();
//     void loadTestFile();
//     void testQtScript();

private:
    Document *_graphDocument;
    QMap<QString,DataPtr> _data;
};

#endif
