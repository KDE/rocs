/*
    This file is part of Rocs.
    Copyright 2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2013  Andreas Cord-Landwehr <cordlandwehr@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LISTSTRUCTURE_H
#define LISTSTRUCTURE_H

#include "rocs_liststructure_export.h"
#include <DataStructure.h>
#include <boost/shared_ptr.hpp>

class ListNode;
namespace Rocs
{
class ROCS_LISTSTRUCTURE_EXPORT ListStructure : public DataStructure
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)

public:
    using DataStructure::remove;
    using DataStructure::createPointer;
    using DataStructure::createData;

    static DataStructurePtr create(Document *parent);
    static DataStructurePtr create(DataStructurePtr other, Document *parent);

    explicit ListStructure(Document* parent = 0);
    virtual ~ListStructure();
    void importStructure(DataStructurePtr other);

    void remove(DataPtr n);
    void remove(PointerPtr e);

    /**
     * Internal method to create new graph edge.
     * Use \see Pointer::create(...) for creating new edges.
     * \param from is the origin of the new edge
     * \param to is the target of the new edge
     * \param pointerType is the type of this edge, defaults to 0
     * \return the created edge as PointerPtr
     */
    PointerPtr createPointer(DataPtr from, DataPtr to, int pointerType);

    /**
     * Internal method to create new graph node.
     * Use \see Data::create(...) for creating new nodes.
     * \param name is the name of the node
     * \param dataType is the type of this node, defaults to 0
     * \return the created node as DataPtr
     */
    DataPtr createData(const QString& name, int dataType);

    /**
     * \return head node of linked list
     */
    Q_INVOKABLE QScriptValue head();

    /**
     * Set head node of linked list.
     * \param node is the
     */
    Q_INVOKABLE void setHead(Data *headNode);

    /**
     * Creates a new data element and return it. If the specified data type is not registered,
     * no data element is created.
     * \param type is the data type of the created node
     * \return script value for the new node
     */
    Q_INVOKABLE QScriptValue createNode(int type);

    /**
     * Creates a new data element and return it.
     * \return script value for the new node
     */
    Q_INVOKABLE QScriptValue createNode();

    /**
     * Use head();
     * \deprecated
     */
    Q_INVOKABLE QScriptValue begin();

    /**
     * Use setHead(node);
     * \deprecated
     */
    Q_INVOKABLE void setBegin(Data* node);

private slots:
    void arrangeNodes();

private:
    void init();

    boost::shared_ptr<ListNode> m_begin;
    QParallelAnimationGroup* m_animationGroup;

    bool m_building;
};
}

#endif // LISTSTRUCTURE_H
