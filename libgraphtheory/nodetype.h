/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef NODETYPE_H
#define NODETYPE_H

#include "graphtheory_export.h"
#include "typenames.h"
#include <QObject>
#include <QSharedPointer>
#include <QStringList>

namespace GraphTheory
{

class NodeTypePrivate;
class NodeTypeStyle;

/**
 * \class Node
 */
class GRAPHTHEORY_EXPORT NodeType : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)

public:
    /**
     * Creates a new node type and registers it at @p document.
     *
     * @param document  the GraphDocument containing this node type
     * @return a NodeType object
     */
    static NodeTypePtr create(GraphDocumentPtr document);

    /** Destroys the edge type */
    ~NodeType() override;

    /**
     * @return shared pointer to object
     */
    NodeTypePtr self() const;

    /**
     * Destroys the node type object and removes it from the document.
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
     * @return the GraphDocument that contains this node type
     */
    GraphDocumentPtr document() const;

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
     * @return node type identifier
     */
    int id() const;

    /**
     * set id of node type to @c id
     */
    void setId(int id);

    /**
     * @return style object of the edge type
     */
    NodeTypeStyle *style() const;

    /**
     * @return dynamic properties registered for this node type
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
     * Debug method that tracks how many node objects exist.
     *
     * @return number of node type objects
     */
    static uint objects()
    {
        return objectCounter;
    }

Q_SIGNALS:
    void nameChanged(const QString &name);
    void idChanged(int id);
    void colorChanged(const QColor &color);
    void dynamicPropertyAboutToBeAdded(const QString &property, int);
    void dynamicPropertyAdded();
    void dynamicPropertiesAboutToBeRemoved(int, int);
    void dynamicPropertyRemoved(const QString &property);
    void dynamicPropertyRenamed(const QString &oldProperty, const QString &newProperty);
    void dynamicPropertyChanged(int index);

protected:
    NodeType();

private:
    Q_DISABLE_COPY(NodeType)
    const QScopedPointer<NodeTypePrivate> d;
    void setQpointer(NodeTypePtr q);
    static uint objectCounter;
};
}

#endif
