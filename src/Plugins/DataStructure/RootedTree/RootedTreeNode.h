/*
    This file is part of RootedTree (Rocs Plugin).
    Copyright 2012  Wagner Reck <wagner.reck@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ROOTEDTREENODE_H
#define ROOTEDTREENODE_H

#include "Data.h"
#include <boost/shared_ptr.hpp>

class RootedTreeNode : public Data
{
    Q_OBJECT

    Q_PROPERTY(quint32 numberOfChilds READ numberOfChilds WRITE setNumberOfChilds)
    Q_PROPERTY(QScriptValue leftChild READ left_child)
    Q_PROPERTY(QScriptValue rightChild READ right_child)
    Q_PROPERTY(QScriptValue nodeParent READ node_parent)

    qint32 m_nChilds;

public:
    static DataPtr create(DataStructurePtr parent, int uniqueIdentifier, int dataType);
    RootedTreeNode(DataStructurePtr parent, int uniqueIdentifier, int dataType);
    ~RootedTreeNode();

    qint8 height() const;

public slots:
    DataPtr leftChild() const;
    DataPtr rightChild() const;
    DataPtr nodeParent() const;
    /** return the i (0 to n-1) child */
    DataPtr child(const quint32 i) const;

    DataList children() const;

    PointerPtr addLeftChild(DataPtr child) const;
    PointerPtr addRigthChild(DataPtr child) const;
    PointerPtr setNodeParent(DataPtr parent) const;
    PointerPtr setChild(DataPtr child, quint32 idx) const;

    QScriptValue add_left_child(RootedTreeNode* child) const;
    QScriptValue add_right_child(RootedTreeNode* child) const;
    QScriptValue add_child(RootedTreeNode* child, quint32 idx) const;
    QScriptValue add_node_parent(RootedTreeNode* child) const;

    QScriptValue left_child() const;
    QScriptValue right_child() const;
    QScriptValue child_at(quint32 idx) const;
    QScriptValue node_parent() const;

    /**return the list of valid children */
    QScriptValue children_list() const;

    quint32 numberOfChilds() const;

    void setNumberOfChilds(const qint32 number);

    void adjustPosition();
};

Q_DECLARE_METATYPE(RootedTreeNode*)

#endif // ROOTEDTREENODE_H
