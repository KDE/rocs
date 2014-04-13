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

#include "qtquickitems/nodeitem.h"
#include "typenames.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include "models/nodemodel.h"

#include <QApplication>
#include <QObject>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QDebug>
#include <QStandardPaths>

using namespace GraphTheory;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<GraphTheory::NodeItem>("org.kde.rocs.graphtheory", 1, 0, "NodeItem");
    qmlRegisterType<GraphTheory::NodeModel>("org.kde.rocs.graphtheory", 1, 0, "NodeModel");

    int rc = 0;

    QQmlEngine engine;
    QQmlComponent *component = new QQmlComponent(&engine);

    QObject::connect(&engine, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

    QUrl path = QUrl::fromLocalFile(QStandardPaths::locate(QStandardPaths::GenericDataLocation, "rocsgraphtheory/qml/Scene.qml"));
    component->loadUrl(path);

    if (!component->isReady() ) {
        qWarning() << ("%s", qPrintable(component->errorString()));
        return -1;
    }

    // test data
    GraphDocumentPtr document = GraphDocument::create();
    NodePtr from = Node::create(document);
    NodePtr to = Node::create(document);
    to->setX(100);
    to->setY(100);
    to->setColor(Qt::green);
    EdgePtr edge = Edge::create(from, to);
    NodeModel *nodeModel = new NodeModel();
    nodeModel->setDocument(document);
    engine.rootContext()->setContextProperty("nodeModel", nodeModel);

    QObject *topLevel = component->create();
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    QSurfaceFormat surfaceFormat = window->requestedFormat();
    window->setFormat(surfaceFormat);
    window->show();

    rc = app.exec();

    delete component;
    nodeModel->deleteLater();
    document->destroy();
    return rc;
}
