/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef EDGEWRAPPER_H
#define EDGEWRAPPER_H

#include "kernel.h"
#include "typenames.h"
#include <QColor>
#include <QObject>

class QEvent;

namespace GraphTheory
{
class EdgeWrapperPrivate;
class DocumentWrapper;
class NodeWrapper;

/**
 * \class EdgeWrapper
 * Wraps EdgePtr to be accessible via QtScript. All properties of the edge object are available
 * as common QObject properties.
 */
class EdgeWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(qreal weight READ weight WRITE setWeight NOTIFY weightChanged)

public:
    EdgeWrapper(EdgePtr edge, DocumentWrapper *documentWrapper);

    ~EdgeWrapper() override;

    EdgePtr edge() const;

    /**
     * @return EdgeType::id of corresponding edge
     */
    int type() const;

    /**
     * Set EdgeType of corresponding edge by specifying its ID by @p typeId.
     * If @p typeId does not name the ID of any EdgeType, the type is not changed.
     */
    void setType(int typeId);

    void setWeight(qreal weight);
    qreal weight() const;

    Q_INVOKABLE GraphTheory::NodeWrapper *from() const;
    Q_INVOKABLE GraphTheory::NodeWrapper *to() const;
    Q_INVOKABLE bool directed() const;

    /**
     * @brief update are set property of this edge
     * @param name the identifier of the property
     * @param value the value of the property
     */
    Q_INVOKABLE void setProperty(const QString &name, const QJSValue &value);

    /**
     * @return the property value identified by @p name
     */
    Q_INVOKABLE QJSValue property(const QString &name) const;

Q_SIGNALS:
    void message(const QString &messageString, Kernel::MessageType type) const;
    void colorChanged(const QColor &color);
    void typeChanged();
    void weightChanged(qreal weight);

private:
    Q_DISABLE_COPY(EdgeWrapper)
    const EdgePtr m_edge;
    const DocumentWrapper *m_documentWrapper;
};
}

Q_DECLARE_METATYPE(GraphTheory::EdgeWrapper *)

#endif
