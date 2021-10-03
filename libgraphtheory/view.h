/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef VIEW_H
#define VIEW_H

#include "graphtheory_export.h"
#include "typenames.h"
#include "editor.h"
#include "graphdocument.h"
#include "edge.h"
#include "node.h"
#include "edgetype.h"
#include "nodetype.h"

#include <QObject>
#include <QQuickWidget>
#include <QSharedPointer>
#include <QList>

namespace GraphTheory
{

class ViewPrivate;

/**
 * \class View
 */
class GRAPHTHEORY_EXPORT View : public QQuickWidget
{
    Q_OBJECT

public:
    explicit View(QWidget *parent);
    ~View() override;
    void setGraphDocument(GraphDocumentPtr document);
    GraphDocumentPtr graphDocument() const;

private Q_SLOTS:
    void createNode(qreal x, qreal y, int typeIndex);
    void createEdge(GraphTheory::Node* from, GraphTheory::Node* to, int typeIndex);
    void deleteNode(GraphTheory::Node *node);
    void deleteEdge(GraphTheory::Edge *edge);
    void showNodePropertiesDialog(GraphTheory::Node *node);
    void showEdgePropertiesDialog(GraphTheory::Edge *edge);

Q_SIGNALS:

private:
    const QScopedPointer<ViewPrivate> d;
};
}

#endif
