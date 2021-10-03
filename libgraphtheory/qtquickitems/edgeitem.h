/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef EDGEITEM_H
#define EDGEITEM_H

#include "graphtheory_export.h"
#include "edge.h"
#include <QQuickItem>

class QSGNode;

namespace GraphTheory
{
class EdgeItemPrivate;

class EdgeItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(GraphTheory::Edge * edge READ edge WRITE setEdge NOTIFY edgeChanged)
    Q_PROPERTY(QPointF origin READ origin WRITE setOrigin)

public:
    explicit EdgeItem(QQuickItem *parent = nullptr);
    ~EdgeItem() override;
    Edge * edge() const;
    void setEdge(Edge *edge);
    /** translation of global origin (0,0) into scene coordinates **/
    QPointF origin() const;
    /** set translation of global origin (0,0) into scene coordinates **/
    void setOrigin(const QPointF &origin);

protected:
    QSGNode * updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;

Q_SIGNALS:
    void edgeChanged();

private Q_SLOTS:
    void updatePosition();
    void updateColor();
    void updateDirection();
    void updateVisibility();

private:
    Q_DISABLE_COPY(EdgeItem)
    const QScopedPointer<EdgeItemPrivate> d;
};
}

#endif
