/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef EDGETYPESTYLE_H
#define EDGETYPESTYLE_H

#include "graphtheory_export.h"
#include "typenames.h"
#include <QObject>
#include <QSharedPointer>

namespace GraphTheory
{

class EdgeTypeStylePrivate;

/**
 * \class Edge
 */
class GRAPHTHEORY_EXPORT EdgeTypeStyle : public QObject
{
    Q_OBJECT
public:
    EdgeTypeStyle();
    ~EdgeTypeStyle() override;
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
    Q_DISABLE_COPY(EdgeTypeStyle)
    const QScopedPointer<EdgeTypeStylePrivate> d;
};
}

#endif
