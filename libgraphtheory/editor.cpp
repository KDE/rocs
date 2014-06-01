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

#include "editor.h"
#include "typenames.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include "models/nodemodel.h"
#include "models/edgemodel.h"
#include "qtquickitems/nodeitem.h"
#include "qtquickitems/edgeitem.h"

#include <QObject>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickWidget>
#include <QDebug>
#include <QStandardPaths>

using namespace GraphTheory;

class GraphTheory::EditorPrivate {
public:
    EditorPrivate()
        : m_engine(0)
        , m_widget(0)
    {
    }

    ~EditorPrivate()
    {
        delete m_edgeModel;
        delete m_nodeModel;
    }

    QQmlEngine *m_engine;
    QQuickWidget *m_widget;
    GraphDocumentPtr m_document;
    EdgeModel *m_edgeModel;
    NodeModel *m_nodeModel;
};

Editor::Editor(QQmlEngine *engine)
    : QObject()
    , d(new EditorPrivate)
{
    d->m_engine = engine;
    d->m_nodeModel = new NodeModel();
    d->m_edgeModel = new EdgeModel();
}

Editor::~Editor()
{
    d->m_widget->deleteLater();
    d->m_engine->deleteLater();
}

GraphDocumentPtr Editor::graphDocument() const
{
    return d->m_document;
}

void Editor::setGraphDocument(GraphDocumentPtr document)
{
    d->m_document = document;
    d->m_nodeModel->setDocument(d->m_document);
    d->m_edgeModel->setDocument(d->m_document);
}

QQuickWidget * Editor::widget()
{
    if (d->m_widget) {
        return d->m_widget;
    }

    qmlRegisterType<GraphTheory::Node>("org.kde.rocs.graphtheory", 1, 0, "Node");
    qmlRegisterType<GraphTheory::Edge>("org.kde.rocs.graphtheory", 1, 0, "Edge");
    qmlRegisterType<GraphTheory::NodeItem>("org.kde.rocs.graphtheory", 1, 0, "NodeItem");
    qmlRegisterType<GraphTheory::EdgeItem>("org.kde.rocs.graphtheory", 1, 0, "EdgeItem");
    qmlRegisterType<GraphTheory::NodeModel>("org.kde.rocs.graphtheory", 1, 0, "NodeModel");
    qmlRegisterType<GraphTheory::EdgeModel>("org.kde.rocs.graphtheory", 1, 0, "EdgeModel");

    QUrl path = QUrl::fromLocalFile(
        QStandardPaths::locate(QStandardPaths::GenericDataLocation, "rocsgraphtheory/qml/Scene.qml"));
    QQmlComponent *component = new QQmlComponent(d->m_engine);
    component->loadUrl(path);
    if (!component->isReady() ) {
        qWarning() << ("%s", qPrintable(component->errorString()));
        return 0;
    }

    // register editor elements at context
    d->m_engine->rootContext()->setContextProperty("nodeModel", d->m_nodeModel);
    d->m_engine->rootContext()->setContextProperty("edgeModel", d->m_edgeModel);

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
    d->m_widget = new QQuickWidget(d->m_engine, 0);
    d->m_widget->setContent(path, component, topLevel);
    return d->m_widget;
}

void Editor::createNode(qreal x, qreal y)
{
    NodePtr node = Node::create(d->m_document);
    node->setX(x);
    node->setY(y);
}

void Editor::createEdge(Node *from, Node *to)
{
    if (!from || !to) {
        return;
    }
    if (!from->isValid() || !to->isValid()) {
        return;
    }
    Edge::create(from->self(), to->self());
}

void Editor::deleteNode(GraphTheory::Node *node)
{
    if (!node || !node->isValid()) {
        return;
    }
    node->destroy();
}

void Editor::deleteEdge(GraphTheory::Edge *edge)
{
    if (!edge || !edge->isValid()) {
        return;
    }
    edge->destroy();
}
