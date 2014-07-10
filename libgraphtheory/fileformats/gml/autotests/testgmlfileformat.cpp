/*
    This file is part of Rocs.
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>

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

#include "testgmlfileformat.h"
#include "../gmlgrammar.h"
#include "../gmlfileformat.h"
#include "typenames.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include <QTest>
#include <QDebug>
#include <QUrl>

using namespace GraphTheory;

void TestGmlFileFormat::parseTest()
{
    // create importer plugin
    GmlFileFormat importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("example.gml"));
    importer.readFile();
    QVERIFY(importer.hasError() == false);
}

void TestGmlFileFormat::serializeTest()
{
    GraphDocumentPtr document = GraphDocument::create();
    QMap<QString, NodePtr> dataList;

    // Creates a simple Graph with 5 data elements and connect them with pointers.
    dataList.insert("a", Node::create(document));
    dataList["a"]->setDynamicProperty("label", "first node");
    dataList.insert("b", Node::create(document));
    dataList["b"]->setDynamicProperty("label", "b");
    dataList.insert("c", Node::create(document));
    dataList["c"]->setDynamicProperty("label", "c");
    dataList.insert("d", Node::create(document));
    dataList["d"]->setDynamicProperty("label", "d");
    dataList.insert("e", Node::create(document));
    dataList["e"]->setDynamicProperty("label", "e");

    Edge::create(dataList["a"], dataList["b"])->setDynamicProperty("label", "test value");
    Edge::create(dataList["b"], dataList["c"]);
    Edge::create(dataList["c"], dataList["d"]);
    Edge::create(dataList["d"], dataList["e"]);
    Edge::create(dataList["e"], dataList["a"]);

    // create exporter plugin
    GmlFileFormat serializer(this, QList<QVariant>());
    serializer.setFile(QUrl::fromLocalFile("test.gml"));
    serializer.writeFile(document);
    QVERIFY(serializer.hasError() == false);
}

QTEST_MAIN(TestGmlFileFormat)
