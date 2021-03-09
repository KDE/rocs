/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef NODETYPESTYLE_H
#define NODETYPESTYLE_H

#include "graphtheory_export.h"
#include "typenames.h"
#include <QObject>
#include <QSharedPointer>

namespace GraphTheory
{

class NodeTypeStylePrivate;

/**
 * \class Node
 */
class GRAPHTHEORY_EXPORT NodeTypeStyle : public QObject
{
    Q_OBJECT
public:
    NodeTypeStyle();
    ~NodeTypeStyle();
    void setColor(const QColor &color);
    QColor color() const;
    void setVisible(bool visible);
    bool isVisible() const;
    void setPropertyNamesVisible(bool visible);
    bool isPropertyNamesVisible() const;

Q_SIGNALS:
    void changed();
    void colorChanged(QColor);
    void visibilityChanged(bool);
    void propertyNamesVisibilityChanged(bool);

private:
    Q_DISABLE_COPY(NodeTypeStyle)
    const QScopedPointer<NodeTypeStylePrivate> d;
};
}

#endif
