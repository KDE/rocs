// SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "view.h"
#include "dialogs/edgeproperties.h"
#include "dialogs/nodeproperties.h"
#include "logging_p.h"
#include "org/kde/rocs/edgemodel.h"
#include "org/kde/rocs/edgepropertymodel.h"
#include "org/kde/rocs/edgetypemodel.h"
#include "org/kde/rocs/nodemodel.h"
#include "org/kde/rocs/nodepropertymodel.h"
#include "org/kde/rocs/nodetypemodel.h"
#include <KLocalizedContext>
#include <QPointer>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QStandardPaths>

using namespace GraphTheory;

class GraphTheory::ViewPrivate
{
public:
    ViewPrivate(QObject *q)
        : m_nodeModel(q)
        , m_edgeModel(q)
        , m_nodeTypeModel(q)
        , m_edgeTypeModel(q)
    {
    }

    ~ViewPrivate() = default;

    GraphDocumentPtr m_document;
    NodeModel m_nodeModel;
    EdgeModel m_edgeModel;
    NodeTypeModel m_nodeTypeModel;
    EdgeTypeModel m_edgeTypeModel;
};

View::View(QWidget *parent)
    : QQuickWidget(parent)
    , d(new ViewPrivate(this))
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

    const QUrl path = QUrl("qrc:/org/kde/rocs/Scene.qml");
    QQmlComponent *component = new QQmlComponent(engine());
    const QVariantMap initialViewProperties = QVariantMap{
        {"edgeModel", QVariant::fromValue(&d->m_edgeModel)},
        {"nodeModel", QVariant::fromValue(&d->m_nodeModel)},
        {"edgeTypeModel", QVariant::fromValue(&d->m_edgeTypeModel)},
        {"nodeTypeModel", QVariant::fromValue(&d->m_nodeTypeModel)},
    };
    component->loadUrl(path);
    if (!component->isReady()) {
        qCWarning(GRAPHTHEORY_GENERAL) << component->errorString();
        return;
    }

    // create rootObject after context is set up
    QObject *topLevel = component->createWithInitialProperties(initialViewProperties);

    // connections to QML signals
    connect(topLevel, SIGNAL(createNode(qreal, qreal, int)), this, SLOT(createNode(qreal, qreal, int)));
    connect(topLevel, SIGNAL(createEdge(GraphTheory::Node *, GraphTheory::Node *, int)), this, SLOT(createEdge(GraphTheory::Node *, GraphTheory::Node *, int)));
    connect(topLevel, SIGNAL(deleteNode(GraphTheory::Node *)), this, SLOT(deleteNode(GraphTheory::Node *)));
    connect(topLevel, SIGNAL(deleteEdge(GraphTheory::Edge *)), this, SLOT(deleteEdge(GraphTheory::Edge *)));
    connect(topLevel, SIGNAL(showNodePropertiesDialog(GraphTheory::Node *)), this, SLOT(showNodePropertiesDialog(GraphTheory::Node *)));
    connect(topLevel, SIGNAL(showEdgePropertiesDialog(GraphTheory::Edge *)), this, SLOT(showEdgePropertiesDialog(GraphTheory::Edge *)));

    // create widget
    setContent(path, component, topLevel);
}

View::~View() = default;

void View::setGraphDocument(GraphDocumentPtr document)
{
    d->m_document = document;
    d->m_nodeModel.setDocument(d->m_document);
    d->m_edgeModel.setDocument(d->m_document);
    d->m_nodeTypeModel.setDocument(d->m_document);
    d->m_edgeTypeModel.setDocument(d->m_document);
}

GraphDocumentPtr View::graphDocument() const
{
    return d->m_document;
}

void View::createNode(qreal x, qreal y, int typeIndex)
{
    Q_ASSERT(typeIndex >= 0);
    Q_ASSERT(typeIndex < d->m_nodeTypeModel.rowCount());
    NodePtr node = Node::create(d->m_document);
    node->setType(d->m_nodeTypeModel.type(typeIndex));
    node->setX(x);
    node->setY(y);
}

void View::createEdge(Node *from, Node *to, int typeIndex)
{
    Q_ASSERT(typeIndex >= 0);
    Q_ASSERT(typeIndex < d->m_edgeTypeModel.rowCount());
    if (!from || !to) {
        return;
    }
    if (!from->isValid() || !to->isValid()) {
        return;
    }
    EdgePtr edge = Edge::create(from->self(), to->self());
    edge->setType(d->m_edgeTypeModel.type(typeIndex));
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

#include "moc_view.cpp"
