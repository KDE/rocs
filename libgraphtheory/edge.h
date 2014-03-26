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

#ifndef EDGE_H
#define EDGE_H

#include "libgraphtheoryexport.h"

#include <QObject>
#include <QSharedPointer>

namespace GraphTheory
{

class Edge;
class EdgePrivate;
typedef QSharedPointer<Edge> EdgePtr;

/**
 * \class Edge
 */
class GRAPHTHEORY_EXPORT Edge : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new Edge.
     *
     * @return a Edge object
     */
    static EdgePtr create();

    /** Destroys the edge */
    virtual ~Edge();

protected:
    Edge();

private:
    Q_DISABLE_COPY(Edge)
    const QScopedPointer<EdgePrivate> d;
    void setQpointer(EdgePtr q);
};
}

#endif
