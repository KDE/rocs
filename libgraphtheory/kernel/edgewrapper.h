/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef EDGEWRAPPER_H
#define EDGEWRAPPER_H

#include "graphtheory_export.h"
#include "kernel.h"
#include "typenames.h"
#include "node.h"
#include "graphdocument.h"

#include <QObject>
#include <QColor>

class QEvent;

namespace GraphTheory
{
class EdgeWrapperPrivate;
class DocumentWrapper;
class NodeWrapper;

/**
 * \class EdgeWrapper
 * Wraps EdgePtr to be accessible via QtScript. All properties of the node object are available
 * as common QObject properties.
 */
class EdgeWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)

public:
    EdgeWrapper(EdgePtr edge, DocumentWrapper *documentWrapper);

    virtual ~EdgeWrapper();

    EdgePtr edge() const;

    /**
     * @return EdgeType::id of corresponding node
     */
    int type() const;

    /**
     * Set EdgeType of corresponding node by specifying its ID by @p typeId.
     * If @p typeId does not name the ID of any NodeType, the type is not changed.
     */
    void setType(int typeId);

    Q_INVOKABLE GraphTheory::NodeWrapper * from() const;
    Q_INVOKABLE GraphTheory::NodeWrapper * to() const;
    Q_INVOKABLE bool directed() const;

    /** reimplemented from QObject **/
    virtual bool event(QEvent *e) Q_DECL_OVERRIDE;

public Q_SLOTS:
    void updateDynamicProperties();

Q_SIGNALS:
    void message(const QString &messageString, Kernel::MessageType type) const;
    void colorChanged(const QColor &color);
    void typeChanged();

private:
    Q_DISABLE_COPY(EdgeWrapper)
    const EdgePtr m_edge;
    const DocumentWrapper *m_documentWrapper;
};
}

Q_DECLARE_METATYPE(GraphTheory::EdgeWrapper*)

#endif
