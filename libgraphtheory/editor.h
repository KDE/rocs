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
 *  License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef EDITOR_H
#define EDITOR_H

#include "graphtheory_export.h"
#include "graphdocument.h"
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
    virtual ~Editor();
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
