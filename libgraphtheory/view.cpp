/*
 *  Copyright 2014-2015  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

#include "view.h"
#include "models/nodemodel.h"
#include "models/edgemodel.h"
#include "models/nodepropertymodel.h"
#include "models/edgepropertymodel.h"
#include "models/nodetypemodel.h"
#include "models/edgetypemodel.h"
#include "qtquickitems/nodeitem.h"
#include "qtquickitems/edgeitem.h"
#include "dialogs/nodeproperties.h"
#include "dialogs/edgeproperties.h"
#include "logging_p.h"

#include <QQmlApplicationEngine>
#include <KLocalizedContext>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QPointer>
#include <QStandardPaths>

using namespace GraphTheory;

class GraphTheory::ViewPrivate {
public:
    ViewPrivate()
        : m_edgeModel(new EdgeModel())
        , m_nodeModel(new NodeModel())
        , m_edgeTypeModel(new EdgeTypeModel())
        , m_nodeTypeModel(new NodeTypeModel)
    {
    }

    ~ViewPrivate()
    {
        delete m_edgeModel;
        delete m_nodeModel;
        delete m_edgeTypeModel;
        delete m_nodeTypeModel;
    }

    GraphDocumentPtr m_document;
    EdgeModel *m_edgeModel;
    NodeModel *m_nodeModel;
    EdgeTypeModel *m_edgeTypeModel;
    NodeTypeModel *m_nodeTypeModel;
};


View::View(QWidget *parent)
    : QQuickWidget(parent)
    , d(new ViewPrivate)
{
    setResizeMode(QQuickWidget::SizeRootObjectToView);

    // workaround for QTBUG-40765
    qApp->setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    // prepare i18n
    auto context = new KLocalizedContext(this);
    context->setTranslationDomain("libgraphtheory");
    engine()->rootContext()->setContextObject(context);

    qmlRegisterType<GraphTheory::Node>("org.kde.rocs.graphtheory", 1, 0, "Node");
    qmlRegisterType<GraphTheory::Edge>("org.kde.rocs.graphtheory", 1, 0, "Edge");
    qmlRegisterType<GraphTheory::NodeType>("org.kde.rocs.graphtheory", 1, 0, "NodeType");
    qmlRegisterType<GraphTheory::EdgeType>("org.kde.rocs.graphtheory", 1, 0, "EdgeType");
    qmlRegisterType<GraphTheory::NodeItem>("org.kde.rocs.graphtheory", 1, 0, "NodeItem");
    qmlRegisterType<GraphTheory::EdgeItem>("org.kde.rocs.graphtheory", 1, 0, "EdgeItem");
    qmlRegisterType<GraphTheory::NodeModel>("org.kde.rocs.graphtheory", 1, 0, "NodeModel");
    qmlRegisterType<GraphTheory::EdgeModel>("org.kde.rocs.graphtheory", 1, 0, "EdgeModel");
    qmlRegisterType<GraphTheory::NodePropertyModel>("org.kde.rocs.graphtheory", 1, 0, "NodePropertyModel");
    qmlRegisterType<GraphTheory::EdgePropertyModel>("org.kde.rocs.graphtheory", 1, 0, "EdgePropertyModel");
    qmlRegisterType<GraphTheory::NodeTypeModel>("org.kde.rocs.graphtheory", 1, 0, "NodeTypeModel");
    qmlRegisterType<GraphTheory::EdgeTypeModel>("org.kde.rocs.graphtheory", 1, 0, "EdgeTypeModel");

    QUrl path = QUrl("qrc:/libgraphtheory/qml/Scene.qml");
    QQmlComponent *component = new QQmlComponent(engine());
    component->loadUrl(path);
    if (!component->isReady() ) {
        qCWarning(GRAPHTHEORY_GENERAL) << component->errorString();
        return;
    }

    // register editor elements at context
    engine()->rootContext()->setContextProperty("nodeModel", d->m_nodeModel);
    engine()->rootContext()->setContextProperty("edgeModel", d->m_edgeModel);
    engine()->rootContext()->setContextProperty("nodeTypeModel", d->m_nodeTypeModel);
    engine()->rootContext()->setContextProperty("edgeTypeModel", d->m_edgeTypeModel);

    // create rootObject after context is set up
    QObject *topLevel = component->create();

    // connections to QML signals
    connect(topLevel, SIGNAL(createNode(qreal,qreal,int)),
            this, SLOT(createNode(qreal,qreal,int)));
    connect(topLevel, SIGNAL(createEdge(GraphTheory::Node*,GraphTheory::Node*,int)),
            this, SLOT(createEdge(GraphTheory::Node*,GraphTheory::Node*,int)));
    connect(topLevel, SIGNAL(deleteNode(GraphTheory::Node*)),
            this, SLOT(deleteNode(GraphTheory::Node*)));
    connect(topLevel, SIGNAL(deleteEdge(GraphTheory::Edge*)),
            this, SLOT(deleteEdge(GraphTheory::Edge*)));
    connect(topLevel, SIGNAL(showNodePropertiesDialog(GraphTheory::Node*)),
            this, SLOT(showNodePropertiesDialog(GraphTheory::Node*)));
    connect(topLevel, SIGNAL(showEdgePropertiesDialog(GraphTheory::Edge*)),
            this, SLOT(showEdgePropertiesDialog(GraphTheory::Edge*)));

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
    d->m_nodeTypeModel->setDocument(d->m_document);
    d->m_edgeTypeModel->setDocument(d->m_document);
}

GraphDocumentPtr View::graphDocument() const
{
    return d->m_document;
}

void View::createNode(qreal x, qreal y, int typeIndex)
{
    Q_ASSERT(typeIndex >= 0);
    Q_ASSERT(typeIndex < d->m_nodeTypeModel->rowCount());
    NodePtr node = Node::create(d->m_document);
    node->setType(d->m_nodeTypeModel->type(typeIndex));
    node->setX(x);
    node->setY(y);
}

void View::createEdge(Node *from, Node *to, int typeIndex)
{
    Q_ASSERT(typeIndex >= 0);
    Q_ASSERT(typeIndex < d->m_edgeTypeModel->rowCount());
    if (!from || !to) {
        return;
    }
    if (!from->isValid() || !to->isValid()) {
        return;
    }
    EdgePtr edge = Edge::create(from->self(), to->self());
    edge->setType(d->m_edgeTypeModel->type(typeIndex));
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

void View::showNodePropertiesDialog(Node *node)
{
    QPointer<NodeProperties> dialog = new NodeProperties();
    dialog->setData(node->self());
    dialog->show(); // workaround: scene-drag not working with modal dialogs
}

void View::showEdgePropertiesDialog(Edge *edge)
{
    QPointer<EdgeProperties> dialog = new EdgeProperties();
    dialog->setData(edge->self());
    dialog->show(); // workaround: scene-drag not working with modal dialogs
}
