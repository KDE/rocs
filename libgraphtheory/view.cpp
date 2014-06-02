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

#include "view.h"
#include "edgetype.h"
#include "nodetype.h"
#include "edge.h"
#include "models/nodemodel.h"
#include "models/edgemodel.h"
#include "qtquickitems/nodeitem.h"
#include "qtquickitems/edgeitem.h"

#include <KDeclarative/KDeclarative>

#include <QObject>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickWidget>
#include <QStandardPaths>
#include <QDebug>

using namespace GraphTheory;

class GraphTheory::ViewPrivate {
public:
    ViewPrivate()
    {
    }

    ~ViewPrivate()
    {
        delete m_edgeModel;
        delete m_nodeModel;
    }

    GraphDocumentPtr m_document;
    EdgeModel *m_edgeModel;
    NodeModel *m_nodeModel;
};


View::View(QWidget *parent)
    : QQuickWidget(parent)
    , d(new ViewPrivate)
{

    // prepare QML engine to be globally used
    KDeclarative::KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine());
    kdeclarative.setupBindings();

    d->m_nodeModel = new NodeModel();
    d->m_edgeModel = new EdgeModel();

    qmlRegisterType<GraphTheory::Node>("org.kde.rocs.graphtheory", 1, 0, "Node");
    qmlRegisterType<GraphTheory::Edge>("org.kde.rocs.graphtheory", 1, 0, "Edge");
    qmlRegisterType<GraphTheory::NodeItem>("org.kde.rocs.graphtheory", 1, 0, "NodeItem");
    qmlRegisterType<GraphTheory::EdgeItem>("org.kde.rocs.graphtheory", 1, 0, "EdgeItem");
    qmlRegisterType<GraphTheory::NodeModel>("org.kde.rocs.graphtheory", 1, 0, "NodeModel");
    qmlRegisterType<GraphTheory::EdgeModel>("org.kde.rocs.graphtheory", 1, 0, "EdgeModel");

    QUrl path = QUrl::fromLocalFile(
        QStandardPaths::locate(QStandardPaths::GenericDataLocation, "rocsgraphtheory/qml/Scene.qml"));
    QQmlComponent *component = new QQmlComponent(engine());
    component->loadUrl(path);
    if (!component->isReady() ) {
        qWarning() << ("%s", qPrintable(component->errorString()));
        return;
    }

    // register editor elements at context
    engine()->rootContext()->setContextProperty("nodeModel", d->m_nodeModel);
    engine()->rootContext()->setContextProperty("edgeModel", d->m_edgeModel);

    // create rootObject after context is set up
    QObject *topLevel = component->create();

    // connections to QML signals
    connect(topLevel, SIGNAL(createNode(qreal,qreal)),
            this, SLOT(createNode(qreal,qreal)));
    connect(topLevel, SIGNAL(createEdge(GraphTheory::Node*, GraphTheory::Node*)),
            this, SLOT(createEdge(GraphTheory::Node*, GraphTheory::Node*)));
    connect(topLevel, SIGNAL(deleteNode(GraphTheory::Node*)),
            this, SLOT(deleteNode(GraphTheory::Node*)));
    connect(topLevel, SIGNAL(deleteEdge(GraphTheory::Edge*)),
            this, SLOT(deleteEdge(GraphTheory::Edge*)));

    // create widget
    setContent(path, component, topLevel);
}

View::~View()
{

}

void View::setGraphDocument(GraphDocumentPtr document)
{
    d->m_document = document;
    d->m_nodeModel->setDocument(d->m_document);
    d->m_edgeModel->setDocument(d->m_document);
}

GraphDocumentPtr View::graphDocument() const
{
    return d->m_document;
}

void View::createNode(qreal x, qreal y)
{
    NodePtr node = Node::create(d->m_document);
    node->setX(x);
    node->setY(y);
}

void View::createEdge(Node *from, Node *to)
{
    if (!from || !to) {
        return;
    }
    if (!from->isValid() || !to->isValid()) {
        return;
    }
    Edge::create(from->self(), to->self());
}

void View::deleteNode(GraphTheory::Node *node)
{
    if (!node || !node->isValid()) {
        return;
    }
    node->destroy();
}

void View::deleteEdge(GraphTheory::Edge *edge)
{
    if (!edge || !edge->isValid()) {
        return;
    }
    edge->destroy();
}
