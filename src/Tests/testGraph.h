/***************************************************************************
 * main.cpp
 * This file is part of the KDE project
 * copyright (C)2004-2007 by Tomaz Canabrava (tomaz.canabrava@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/
#ifndef TESTGRAPH_H
#define TESTGRAPH_H

#include <QObject>
#include <QVariant>
#include "graphDocument.h"

class Graph;

class GraphTests : public QObject {
    Q_OBJECT
public:
    GraphTests();
    void createPentagon(bool directed = false);
    void create3x3(bool directed = false);
private slots:
    void init();

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
    DataTypeDocument _graphDocument;
    QVariantList Graphs;
};

#endif
