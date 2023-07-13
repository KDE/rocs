/*
 *  SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "transformedgeswidget.h"
#include "edge.h"
#include "graphdocument.h"
#include "modifiers/topology.h"

#include <KLocalizedString>
#include <QDebug>
#include <QMap>
#include <QPair>

using namespace GraphTheory;

TransformEdgesWidget::TransformEdgesWidget(GraphDocumentPtr document, QWidget *parent)
    : QDialog(parent)
    , m_document(document)
{
    setWindowTitle(i18nc("@title:window", "Transform Edges"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    QWidget *widget = new QWidget(this);
    ui = new Ui::TransformEdgesWidget;
    ui->setupUi(widget);
    mainLayout->addWidget(widget);

    connect(ui->buttons, &QDialogButtonBox::accepted, this, &TransformEdgesWidget::accept);
    connect(ui->buttons, &QDialogButtonBox::rejected, this, &TransformEdgesWidget::reject);
    connect(this, &QDialog::accepted, this, &TransformEdgesWidget::transform);
}

TransformEdgesWidget::~TransformEdgesWidget()
{
    delete ui;
}

void TransformEdgesWidget::transform()
{
    if (ui->radioButtonMakeComplete->isChecked()) {
        makeComplete();
    }
    if (ui->radioButtonEraseEdges->isChecked()) {
        removeAllEdges();
    }
    if (ui->radioButtonReverseEdges->isChecked()) {
        reverseAllEdges();
    }
    if (ui->radioButtonMakeSpanningtree->isChecked()) {
        makeSpanningTree();
    }
    if (ui->radioButtonEraseSelfEdges->isChecked()) {
        removeAllSelfEdges();
    }
}

void TransformEdgesWidget::makeComplete()
{
    // TODO only default edge type considered
    const auto edges = m_document->edges();
    for (const EdgePtr &e : edges) {
        e->destroy();
    }

    for (int i = 0; i < m_document->nodes().size() - 1; ++i) {
        for (int j = i + 1; j < m_document->nodes().size(); ++j) {
            Edge::create(m_document->nodes().at(i), m_document->nodes().at(j));
            if (m_document->edgeTypes().first()->direction() == EdgeType::Unidirectional) {
                Edge::create(m_document->nodes().at(j), m_document->nodes().at(i));
            }
        }
    }
}

void TransformEdgesWidget::removeAllEdges()
{
    // TODO only default edge type considered
    const auto edges = m_document->edges();
    for (const EdgePtr &e : edges) {
        e->destroy();
    }
}

void TransformEdgesWidget::reverseAllEdges()
{
    const auto edgeTypes = m_document->edgeTypes();
    for (const EdgeTypePtr &type : edgeTypes) {
        if (type->direction() == EdgeType::Bidirectional) {
            continue;
        }
        QList<QPair<NodePtr, NodePtr>> newEdges;
        const auto edges = m_document->edges(type);
        for (const EdgePtr &e : edges) {
            newEdges << QPair<NodePtr, NodePtr>(e->to(), e->from());
            e->destroy();
        }

        for (int i = 0; i < newEdges.count(); ++i) {
            EdgePtr edge = Edge::create(newEdges[i].first, newEdges[i].second);
            edge->setType(type);
        }
    }
}

qreal TransformEdgesWidget::makeSpanningTree()
{
    NodeList nodes = m_document->nodes();
    int n = nodes.size();

    // the resulting spanning tree (MST)
    QList<QPair<int, int>> MST;

    /*
     * distance[i] denotes the distance between node i and the minimum spanning
     * tree; initially this distance is infinity. Note that if i is already element
     * in MST distance[i] is only a temporary variable and its value is undefined.
     */
    QMap<int, qreal> distance;
    for (int i = 0; i < nodes.size(); i++) {
        distance[i] = std::numeric_limits<unsigned int>::max();
    }

    /*
     * Indicator variable that is true if node is in tree, false otherwise.
     * Initially all nodes are marked to be not in MST.
     */
    QMap<int, bool> inTree;
    for (int i = 0; i < nodes.size(); i++) {
        inTree[i] = false;
    }

    /* weight[i][j] denotes distance between nodes i and j. If no
     * path is present between i and j in the previous tree the weight
     * must be set to 0.
     */
    QMap<QPair<int, int>, qreal> weight;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j)
                weight[QPair<int, int>(i, j)] = 0;

            EdgeList out;
            out = nodes[i]->edges();

            for (int k = 0; k < out.size(); k++) {
                if (out[k]->to() == nodes[j]) {
                    if (!out[k]->property("value").toString().isEmpty())
                        weight[QPair<int, int>(i, j)] = out[k]->property("value").toDouble();
                    else
                        weight[QPair<int, int>(i, j)] = 1;
                }
            }
        }
    }

    /*
     * successor[i] denotes the index of the node, to which i must be
     * linked to in order to get distance distance[i]
     */
    QMap<int, int> successor;

    // start with first graph node
    inTree[0] = true;

    // update distances
    for (int i = 0; i < n; ++i) {
        if ((weight[QPair<int, int>(0, i)] != 0) && (distance[i] > weight[QPair<int, int>(0, i)])) {
            distance[i] = weight[QPair<int, int>(0, i)];
            successor[i] = 0;
        }
    }

    qreal total = 0;
    for (int treeSize = 1; treeSize < n; treeSize++) {
        // Find node with the smallest distance to the tree
        int min = -1;
        for (int i = 0; i < n; ++i) {
            if (inTree[i] == false) {
                if ((min == -1) || (distance[min] > distance[i])) {
                    min = i;
                }
            }
        }

        // add node to tree
        MST << QPair<int, int>(successor[min], min);
        inTree[min] = 1;
        total += distance[min];

        // update distances
        for (int i = 0; i < n; ++i) {
            if ((weight[QPair<int, int>(min, i)] != 0) && (distance[i] > weight[QPair<int, int>(min, i)])) {
                distance[i] = weight[QPair<int, int>(min, i)];
                successor[i] = min;
            }
        }
    }

    removeAllEdges();

    // refill with MST edges
    for (int i = 0; i < MST.size(); i++) {
        EdgePtr edge = Edge::create(nodes[MST[i].first], nodes[MST[i].second]);

        if (weight[QPair<int, int>(MST[i].first, MST[i].second)] != 1) {
            QString value;
            value.setNum(weight[QPair<int, int>(MST[i].first, MST[i].second)]);
            edge->setDynamicProperty("value", value);
        }
    }

    return total;
}

void TransformEdgesWidget::removeAllSelfEdges()
{
    const auto edges = m_document->edges();
    for (const EdgePtr &e : edges) {
        if (e->to() == e->from()) {
            e->destroy();
        }
    }
}

#include "moc_transformedgeswidget.cpp"
