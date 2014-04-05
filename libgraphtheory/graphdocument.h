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

#ifndef GRAPHDOCUMENT_H
#define GRAPHDOCUMENT_H

#include "libgraphtheoryexport.h"
#include "typenames.h"
#include "node.h"
#include "edgetype.h"
#include "nodetype.h"

#include <QObject>
#include <QSharedPointer>
#include <QList>

namespace GraphTheory
{

class GraphDocumentPrivate;

/**
 * \class GraphDocument
 */
class GRAPHTHEORY_EXPORT GraphDocument : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new GraphDocument.
     *
     * @return a GraphDocument object
     */
    static GraphDocumentPtr create();

    /** Destroys the document */
    virtual ~GraphDocument();

    /**
     * List of registered edge types. The list is never empty and the first element is the
     * default EdgeType.
     *
     * @return the list of edge types
     */
    QList<EdgeTypePtr> edgeTypes() const;

    /**
     * List of registered node types. The list is never empty and the first element is the
     * default NodeType.
     *
     * @return the list of edge types
     */
    QList<NodeTypePtr> nodeTypes() const;

protected:
    GraphDocument();

private:
    Q_DISABLE_COPY(GraphDocument)
    const QScopedPointer<GraphDocumentPrivate> d;
    void setQpointer(GraphDocumentPtr q);
};
}

#endif
