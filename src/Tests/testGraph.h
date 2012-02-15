/* 
    This file is part of Rocs.
    Copyright 2004-2010  Tomaz Canabrava <tomaz.canabrava@gmail.com>

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

#ifndef TESTGRAPH_H
#define TESTGRAPH_H

#include "Rocs_Typedefs.h"

#include <QObject>
#include <QVariant>
#include <QMap>

class Document;

class GraphTests : public QObject {
    Q_OBJECT
public:
    GraphTests();
    void createPentagon(bool directed = false);
    void create3x3(bool directed = false);
private slots:
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void createSimpleGraph();
    void manipulateSimpleGraph();
    void createDirectedGraph();
    void manipulateDirectedGraph();
    void saveTestFile();
    void loadTestFile();
//   void testKrossJs();
//   void testKrossPy();
//   void testKrossRb();
// 	void testKrossQtjs();
    void testQtScript();

private:
    Document *_graphDocument;
    QMap<QString,DataPtr> _data;
//     QVariantList Graphs;
};

#endif
