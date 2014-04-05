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

#ifndef EDGETYPE_H
#define EDGETYPE_H

#include "libgraphtheoryexport.h"
#include "typenames.h"

#include <QObject>
#include <QSharedPointer>

namespace GraphTheory
{

class EdgeTypePrivate;

/**
 * \class Edge
 */
class GRAPHTHEORY_EXPORT EdgeType : public QObject
{
    Q_OBJECT

public:
    enum Direction {
        Unidirectional = 0,
        Bidirectional = 1
    };

    /**
     * Creates a new EdgeType.
     *
     * @return a EdgeType object
     */
    static EdgeTypePtr create();

    /** Destroys the edge type */
    virtual ~EdgeType();

    /**
     * @return direction of edges of this type
     */
    EdgeType::Direction direction() const;

    /**
     * Set direction for edges of this type. Setting this, emits signal
     * directionChanged(@p direction).
     *
     * @param direction the direction for edges of this type
     */
    void setDirection(EdgeType::Direction direction);

Q_SIGNALS:
    void directionChanged(EdgeType::Direction direction);

protected:
    EdgeType();

private:
    Q_DISABLE_COPY(EdgeType)
    const QScopedPointer<EdgeTypePrivate> d;
};
}

#endif
