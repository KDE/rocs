/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef EDITOR_H
#define EDITOR_H

#include "graphdocument.h"
#include "graphtheory_export.h"
#include <QObject>

class QQuickWidget;
class QQmlEngine;

namespace GraphTheory
{
class EditorPrivate;

class GRAPHTHEORY_EXPORT Editor : public QObject
{
    Q_OBJECT

public:
    explicit Editor();
    ~Editor() override;
    QList<GraphDocumentPtr> documents() const;
    GraphDocumentPtr createDocument();

    /**
     * Load document from @p documentUrl..
     * @return the loaded document
     */
    GraphDocumentPtr openDocument(const QUrl &documentUrl);

Q_SIGNALS:
    void documentCreated(GraphDocumentPtr document);

private:
    Q_DISABLE_COPY(Editor)
    const QScopedPointer<EditorPrivate> d;
};
}

#endif
