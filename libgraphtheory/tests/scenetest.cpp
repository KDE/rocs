/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "typenames.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include "editor.h"
#include "view.h"

#include <KDeclarative/KDeclarative>
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

    // test data
    GraphDocumentPtr document = GraphDocument::create();
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

    Editor editor;
    editor.setGraphDocument(document);
    QQuickWidget *widget = document->createView(0);
    widget->show();

    rc = app.exec();
    document->destroy();
    return rc;
}
