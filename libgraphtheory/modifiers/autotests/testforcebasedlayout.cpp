/*
 *  SPDX-FileCopyrightText: 2020 Dilson Almeida Guimarães <dilsonguim@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
