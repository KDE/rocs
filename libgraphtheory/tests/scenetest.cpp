/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "typenames.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include "editor.h"
#include "view.h"
#include <QApplication>
#include <QObject>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickWidget>
#include <QDebug>
#include <QStandardPaths>

using namespace GraphTheory;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    int rc = 0;
    Editor editor;

    // test data
    GraphDocumentPtr document = editor.createDocument();
    document->edgeTypes().first()->addDynamicProperty("weight");
    document->nodeTypes().first()->addDynamicProperty("value");
    NodePtr from = Node::create(document);
    NodePtr to = Node::create(document);
    to->setX(100);
    to->setY(100);
    to->setDynamicProperty("value", "5");
    to->setColor(Qt::green);
    EdgePtr edge = Edge::create(from, to);
    edge->setDynamicProperty("weight", "3");

    QQuickWidget *widget = document->createView(0);
    widget->show();

    rc = app.exec();
    document->destroy();
    return rc;
}
