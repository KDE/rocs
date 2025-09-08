// SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "view.h"
#include "dialogs/edgeproperties.h"
#include "dialogs/nodeproperties.h"
#include "logging_p.h"
#include "org/kde/rocs/edgemodel.h"
#include "org/kde/rocs/edgepropertymodel.h"
#include "org/kde/rocs/edgetypemodel.h"
#include "org/kde/rocs/graph.h"
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
        : m_graph(q)
        , m_nodeModel(q)
        , m_edgeModel(q)
        , m_nodeTypeModel(q)
        , m_edgeTypeModel(q)
    {
        m_graph.setGraphDocument(m_document);
    }

    ~ViewPrivate() = default;

    GraphDocumentPtr m_document;
    Graph m_graph;
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

    const QUrl path = QUrl("qrc:/org/kde/rocs/Scene.qml");
    QQmlComponent *component = new QQmlComponent(engine());
    const QVariantMap initialViewProperties = QVariantMap{
        {"graph", QVariant::fromValue(&d->m_graph)},
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
    // connect(topLevel, SIGNAL(showNodePropertiesDialog(GraphTheory::NodeProxy *)), this, SLOT(showNodePropertiesDialog(GraphTheory::NodeProxy *)));
    // connect(topLevel, SIGNAL(showEdgePropertiesDialog(GraphTheory::EdgeProxy *)), this, SLOT(showEdgePropertiesDialog(GraphTheory::EdgeProxy *)));

    // create widget
    setContent(path, component, topLevel);
}

View::~View() = default;

void View::setGraphDocument(GraphDocumentPtr document)
{
    d->m_document = document;
    d->m_graph.setGraphDocument(d->m_document);
    d->m_nodeModel.setDocument(d->m_document);
    d->m_edgeModel.setDocument(d->m_document);
    d->m_nodeTypeModel.setDocument(d->m_document);
    d->m_edgeTypeModel.setDocument(d->m_document);
}

GraphDocumentPtr View::graphDocument() const
{
    return d->m_document;
}

// TODO move
// void View::showNodePropertiesDialog(NodeProxy *node)
// {
//     QPointer<NodeProperties> dialog = new NodeProperties();
//     dialog->setData(node->node()->self());
//     dialog->show(); // workaround: scene-drag not working with modal dialogs
// }

// void View::showEdgePropertiesDialog(EdgeProxy *edge)
// {
//     QPointer<EdgeProperties> dialog = new EdgeProperties();
//     dialog->setData(edge->edge()->self());
//     dialog->show(); // workaround: scene-drag not working with modal dialogs
// }

#include "moc_view.cpp"
