/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef EDGEWRAPPER_H
#define EDGEWRAPPER_H

#include "graphdocument.h"
#include "graphtheory_export.h"
#include "kernel.h"
#include "node.h"
#include "typenames.h"
#include <optional>
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
 * Wraps EdgePtr to be accessible via QtScript. All properties of the node object are available
 * as common QObject properties.
 */
class EdgeWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)

public:
    EdgeWrapper(EdgePtr edge, DocumentWrapper *documentWrapper);

    ~EdgeWrapper() override;

    EdgePtr edge() const;

    QJSValue toScriptValue(QJSEngine *engine);

    /**
     * retrieve dynamic property values from script value and release association
     * @note QJSValue is owned by JS engine
     */
    void releaseScriptValue();

    /**
     * @return EdgeType::id of corresponding node
     */
    int type() const;

    /**
     * Set EdgeType of corresponding node by specifying its ID by @p typeId.
     * If @p typeId does not name the ID of any NodeType, the type is not changed.
     */
    void setType(int typeId);

    Q_INVOKABLE GraphTheory::NodeWrapper *from() const;
    Q_INVOKABLE GraphTheory::NodeWrapper *to() const;
    Q_INVOKABLE bool directed() const;

    /** reimplemented from QObject **/
    bool event(QEvent *e) override;

public Q_SLOTS:
    void updateDynamicProperties();

Q_SIGNALS:
    void message(const QString &messageString, GraphTheory::MessageType type) const;
    void colorChanged(const QColor &color);
    void typeChanged();

private:
    Q_DISABLE_COPY(EdgeWrapper)
    const EdgePtr m_edge;
    const DocumentWrapper *m_documentWrapper{nullptr};
    std::optional<QJSValue> m_scriptValue;
};
}

Q_DECLARE_METATYPE(GraphTheory::EdgeWrapper *)

#endif
