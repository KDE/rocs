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

#ifndef EDGEWRAPPER_H
#define EDGEWRAPPER_H

#include "libgraphtheoryexport.h"
#include "typenames.h"
#include "node.h"
#include "graphdocument.h"

#include <QtScript>
#include <QObject>
#include <QColor>

namespace GraphTheory
{
class EdgeWrapperPrivate;
class DocumentWrapper;

/**
 * \class EdgeWrapper
 * Wraps EdgePtr to be accessible via QtScript. All properties of the node object are available
 * as commong QObject properties.
 */
class EdgeWrapper : public QObject
{
    Q_OBJECT

public:
    EdgeWrapper(EdgePtr edge, DocumentWrapper *documentWrapper);

    virtual ~EdgeWrapper();

    /**
     * If the id value is invalid, -1 is returned.
     *
     * @return node identifier
     */
    int id() const;

    /** reimplemented from QObject **/
    virtual bool event(QEvent* e);

public Q_SLOTS:
    void updateDynamicProperties();

Q_SIGNALS:
    void colorChanged(const QColor &color);

private:
    Q_DISABLE_COPY(EdgeWrapper)
    const EdgePtr m_edge;
    const DocumentWrapper *m_documentWrapper;
};
}

Q_DECLARE_METATYPE(QList<GraphTheory::EdgeWrapper*>)

#endif
