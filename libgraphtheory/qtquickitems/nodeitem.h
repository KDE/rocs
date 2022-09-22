/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef NODEITEM_H
#define NODEITEM_H

#include "graphtheory_export.h"
#include "node.h"
#include <QQuickPaintedItem>

namespace GraphTheory
{
class NodeItemPrivate;

class NodeItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(GraphTheory::Node *node READ node WRITE setNode NOTIFY nodeChanged)
    Q_PROPERTY(QPointF origin READ origin WRITE setOrigin)
    Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted NOTIFY highlightedChanged)

public:
    explicit NodeItem(QQuickPaintedItem *parent = nullptr);
    ~NodeItem() override;
    Node *node() const;
    void setNode(Node *node);
    /** translation of global origin (0,0) into scene coordinates **/
    QPointF origin() const;
    /** set translation of global origin (0,0) into scene coordinates **/
    void setOrigin(const QPointF &origin);
    bool isHighlighted() const;
    void setHighlighted(bool highlight);
    /** reimplemented from QQuickPaintedItem **/
    void paint(QPainter *painter) override;
    /** reimplemented from QQuickItem **/
    bool contains(const QPointF &point) const override;

Q_SIGNALS:
    void nodeChanged();
    void highlightedChanged();

private Q_SLOTS:
    void updatePositionfromScene();
    void setGlobalPosition(const QPointF &globalPosition);
    void updateVisibility();

private:
    Q_DISABLE_COPY(NodeItem)
    const QScopedPointer<NodeItemPrivate> d;
};
}

#endif
