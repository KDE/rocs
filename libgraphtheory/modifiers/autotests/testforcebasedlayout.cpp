/*
 *  Copyright 2020  Dilson Almeida Guimarães <dilsonguim@gmail.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "testforcebasedlayout.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include "editor.h"
#include "modifiers/topology.h"
#include <QTest>
#include <QUrl>
#include <QDirIterator>
#include <QString>

#include <iostream>

using namespace GraphTheory;

void ForceBasedLayoutTest::findGraphFiles(const QString& path, QVector<QUrl>& files)
{
    QDirIterator graphFilesIterator(path, QDir::Files);
    while (graphFilesIterator.hasNext()) {
        files.push_back(QUrl::fromLocalFile(graphFilesIterator.next()));
    }
}

void ForceBasedLayoutTest::testMargin_data()
{
    QTest::addColumn<QUrl>("file");
    QTest::addColumn<qreal>("margin");
    QTest::addColumn<qreal>("nodeRadius");
    
    QVector<qreal> marginValues = {0., 10.};
    QVector<qreal> nodeRadiusValues = {5., 10.};
    
    QVector<QUrl> files; 
    findGraphFiles("graphs", files);
    
    for (const QUrl& file : files) {
        for (const qreal margin : marginValues) {
            for (const qreal nodeRadius : nodeRadiusValues) {
                QString entryName = QString("%1#%2#%3").arg(file.path()).arg(margin).arg(nodeRadius);

                QTest::newRow(entryName.toLocal8Bit().constData()) << file << margin << nodeRadius;
            }
        }
    }
}

void ForceBasedLayoutTest::testMargin()
{
    constexpr qreal EPS = 1.e-4;

    Editor editor;

    QFETCH(QUrl, file);
    QFETCH(qreal, margin);
    QFETCH(qreal, nodeRadius);
    
    GraphDocumentPtr document = editor.openDocument(file);
    
    Topology::applyForceBasedLayout(document, nodeRadius, margin, 1., 1., 1., true, 0);

    foreach(NodePtr node, document->nodes()) {
        QVERIFY(node->x() - nodeRadius >= margin - EPS);
        QVERIFY(node->y() - nodeRadius >= margin - EPS);
    }
}

QTEST_MAIN(ForceBasedLayoutTest)
