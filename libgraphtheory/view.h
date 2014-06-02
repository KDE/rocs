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

#ifndef VIEW_H
#define VIEW_H

#include "libgraphtheoryexport.h"
#include "typenames.h"
#include "editor.h"
#include "graphdocument.h"
#include "node.h"
#include "edgetype.h"
#include "nodetype.h"

#include <QObject>
#include <QQuickWidget>
#include <QSharedPointer>
#include <QList>

namespace GraphTheory
{

class ViewPrivate;

/**
 * \class View
 */
class GRAPHTHEORY_EXPORT View : public QQuickWidget
{
    Q_OBJECT

public:
    View(QWidget *parent);
    virtual ~View();
    void setGraphDocument(GraphDocumentPtr document);
    GraphDocumentPtr graphDocument() const;

private Q_SLOTS:
    void createNode(qreal x, qreal y);
    void createEdge(GraphTheory::Node *from, GraphTheory::Node *to);
    void deleteNode(GraphTheory::Node *node);
    void deleteEdge(GraphTheory::Edge *edge);

Q_SIGNALS:

private:
    const QScopedPointer<ViewPrivate> d;
};
}

#endif
