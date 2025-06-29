/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef EDGETYPE_H
#define EDGETYPE_H

#include "graphtheory_export.h"
#include "typenames.h"
#include <QObject>
#include <QSharedPointer>

namespace GraphTheory
{

class EdgeTypePrivate;
class EdgeTypeStyle;

/**
 * \class Edge
 */
class GRAPHTHEORY_EXPORT EdgeType : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)

public:
    enum Direction { Unidirectional = 0, Bidirectional = 1 };

    /**
     * Creates a new edge type and registers it at @p document.
     *
     * @param document  the GraphDocument containing this edge type
     * @return a EdgeType object
     */
    static EdgeTypePtr create(GraphDocumentPtr document);

    /** Destroys the edge type */
    ~EdgeType() override;

    /**
     * @return shared pointer to object
     */
    EdgeTypePtr self() const;

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
     * @return the GraphDocument that contains this edge type
     */
    GraphDocumentPtr document() const;

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
     * Rename dynamic property @c oldProperty to @c newProperty.
     * Signals dynamicPropertyChanged(index) and dynamicPropertyRenamed(@c oldProperty, @c oldProperty)
     * are emitted afterwards
     */
    void renameDynamicProperty(const QString &oldProperty, const QString &newProperty);

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
     * @return style object of the edge type
     */
    EdgeTypeStyle *style() const;

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
    void dynamicPropertyAboutToBeAdded(const QString &property, int);
    void dynamicPropertyAdded();
    void dynamicPropertiesAboutToBeRemoved(int, int);
    void dynamicPropertyRemoved(const QString &property);
    void dynamicPropertyRenamed(const QString &oldProperty, const QString &newProperty);
    void dynamicPropertyChanged(int index);
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
