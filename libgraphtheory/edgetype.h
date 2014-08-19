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
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)

public:
    enum Direction {
        Unidirectional = 0,
        Bidirectional = 1
    };

    /**
     * Creates a new edge type and registers it at @p document.
     *
     * @param document  the GraphDocument containing this edge type
     * @return a EdgeType object
     */
    static EdgeTypePtr create(GraphDocumentPtr document);

    /** Destroys the edge type */
    virtual ~EdgeType();

    /**
     * Destroys the edge type object and removes it from the document.
     */
    void destroy();

    /**
     * The valid state returns @c true if the object is created and can be used, @c false if it is
     * not yet created or about to be removed
     *
     * @return the validity state of the object
     */
    bool isValid() const;

    /**
     * @return direction of edges of this type
     */
    EdgeType::Direction direction() const;

    /**
     * @return dynamic properties registered for this edge type
     */
    QStringList dynamicProperties() const;

    /**
     * Add dynamic property @c property to list of dynamic properties.
     * Signal dynamicPropertyAdded(@c property) is emitted afterwards
     */
    void addDynamicProperty(const QString &property);

    /**
     * Remove dynamic property @c property from list of dynamic properties.
     * Signal dynamicPropertyRemoved(@c property) is emitted afterwards
     */
    void removeDynamicProperty(const QString &property);

    /**
     * Set direction for edges of this type. Setting this, emits signal
     * directionChanged(@p direction).
     *
     * @param direction the direction for edges of this type
     */
    void setDirection(EdgeType::Direction direction);

    /**
     * Set user visible name of node type to @p name.
     */
    void setName(const QString &name);

    /**
     * @return user visible name of node type
     */
    QString name() const;

    /**
     * If the id value is invalid, -1 is returned.
     *
     * @return edge type identifier
     */
    int id() const;

    /**
     * set id of edge type to @c id
     */
    void setId(int id);

    /**
     * @return color of edge type
     */
    QColor color() const;

    /**
     * set color of edge type to @c color
     */
    void setColor(const QColor &color);

    /**
     * Debug method that tracks how many edge objects exist.
     *
     * @return number of edge type objects
     */
    static uint objects()
    {
        return objectCounter;
    }

Q_SIGNALS:
    void directionChanged(EdgeType::Direction direction);
    void idChanged(int id);
    void colorChanged(const QColor &color);
    void dynamicPropertyAboutToBeAdded(const QString &property, int);
    void dynamicPropertyAdded();
    void dynamicPropertiesAboutToBeRemoved(int, int);
    void dynamicPropertyRemoved(const QString &property);
    void nameChanged(const QString &name);

protected:
    EdgeType();

private:
    Q_DISABLE_COPY(EdgeType)
    const QScopedPointer<EdgeTypePrivate> d;
    void setQpointer(EdgeTypePtr q);
    static uint objectCounter;
};
}

#endif
