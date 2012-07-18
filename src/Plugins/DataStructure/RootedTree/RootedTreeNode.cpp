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

#include "RootedTreeNode.h"
#include "DataStructure.h"
#include "Pointer.h"

#include <KDebug>
#include "RootedTreeStructure.h"

DataPtr RootedTreeNode::create(DataStructurePtr parent, int uniqueIdentifier, int dataType)
{
    return Data::create<RootedTreeNode>(parent, uniqueIdentifier, dataType);
}

RootedTreeNode::RootedTreeNode(DataStructurePtr parent, int uniqueIdentifier, int dataType):
    Data(parent, uniqueIdentifier, dataType), m_nChilds(-1)
{
}


RootedTreeNode::~RootedTreeNode()
{

}

quint32 RootedTreeNode::numberOfChilds() const
{
    if (m_nChilds != -1) {
        return m_nChilds;
    }
    if (dataStructure()->property("ChildCount").isValid()) {
        return dataStructure()->property("ChildCount").toUInt();
    }
    return 2;
}

void RootedTreeNode::setNumberOfChilds(const qint32 number)
{
    if (number != m_nChilds){
        for (qint32 i = number; i < m_nChilds; ++i) {
            setChild(DataPtr(), i);
        }
        m_nChilds = number;
    }
}


DataList RootedTreeNode::children() const
{
    DataList list;
    foreach (PointerPtr ptr, out_pointers())
        if (ptr->property("TreeEdge").isValid() && ptr->property("TreeEdge") != -1) {
            list << ptr->to();
        }
    return list;
}


PointerPtr RootedTreeNode::addRigthChild(DataPtr child) const
{
    return setChild(child, numberOfChilds()-1);
}

DataPtr RootedTreeNode::rightChild() const
{
    return child(numberOfChilds()-1);
}


PointerPtr RootedTreeNode::addLeftChild(DataPtr child) const
{
    return setChild(child, 0);
}


DataPtr RootedTreeNode::leftChild() const
{
    return child(0);
}


PointerPtr RootedTreeNode::setNodeParent(DataPtr parent) const
{
    foreach (PointerPtr p, pointers(nodeParent())) {
        if (p->property("TreeEdge").isValid() && p->property("TreeEdge").toInt() == -1){
            p->remove();
        }
    }
    if (parent.get()){
        PointerPtr ptr = dataStructure()->addPointer(this->getData(), parent);
        ptr->setProperty("TreeEdge", -1);
        return ptr;
    }
    return PointerPtr();
}


DataPtr RootedTreeNode::nodeParent() const
{
    foreach (PointerPtr ptr, out_pointers()) {
        if (ptr->property("TreeEdge").isValid() && ptr->property("TreeEdge").toInt() < 0) {
            return ptr->to();
        }
    }
    return DataPtr();
}


DataPtr RootedTreeNode::child(const quint32 i) const
{
    if (i >= numberOfChilds()) {
        return DataPtr();
    }
    foreach (PointerPtr ptr, out_pointers()) {
        if (ptr->property("TreeEdge").isValid() && ptr->property("TreeEdge").toUInt() == i) {
            return ptr->to();
        }
    }
    return DataPtr();
}


PointerPtr RootedTreeNode::setChild(DataPtr c, quint32 idx) const{

    if (idx < numberOfChilds()){
        foreach (PointerPtr p, pointers(child(idx))) {
            p->remove();
        }
//      child(idx)->remove();
        if (c.get()){
            PointerPtr ptr = dataStructure()->addPointer(this->getData(), c->getData());
            ptr->setProperty("TreeEdge", idx);
            return ptr;
        }
    }
    return PointerPtr();
}


QScriptValue RootedTreeNode::add_child(RootedTreeNode* childNode, quint32 idx) const
{
    if (idx < numberOfChilds() && childNode){
        PointerPtr pointer = setChild(childNode->getData(), idx);
        if (!pointer) {
            return pointer->scriptValue();
        }
    }
    setChild(DataPtr(), idx);
    return QScriptValue();
}


QScriptValue RootedTreeNode::add_left_child(RootedTreeNode* child) const
{
    if (!child){
        addLeftChild(DataPtr());
    } else {
        PointerPtr pointer = addLeftChild(child->getData());
        if (!pointer) {
            return pointer->scriptValue();
        }
    }
    return QScriptValue();
}


QScriptValue RootedTreeNode::add_right_child(RootedTreeNode* child) const
{
    if (!child){
        addRigthChild(DataPtr());
    } else {
        PointerPtr pointer = addRigthChild(child->getData());
        if (!pointer) {
            return pointer->scriptValue();
        }
    }
    return QScriptValue();
}


QScriptValue RootedTreeNode::add_node_parent(RootedTreeNode* parentNode) const
{
    if (!parentNode){
        setNodeParent(DataPtr());
    } else {
        PointerPtr pointer = setNodeParent(parentNode->getData());
        if (!pointer) {
            return pointer->scriptValue();
        }
    }
    return QScriptValue();
}


QScriptValue RootedTreeNode::child_at(quint32 idx) const
{
    DataPtr data = child(idx);
    if (!data) {
        return data->scriptValue();
    }
    return QScriptValue();
}


QScriptValue RootedTreeNode::left_child() const
{
    DataPtr data = leftChild();
    if (data) {
        return data->scriptValue();
    }
    return QScriptValue();
}


QScriptValue RootedTreeNode::right_child() const
{
    DataPtr data = rightChild();
    if (data) {
        return data->scriptValue();
    }
    return QScriptValue();
}


QScriptValue RootedTreeNode::node_parent() const
{
    DataPtr data = nodeParent();
    if (data) {
        return data->scriptValue();
    }
    return QScriptValue();
}


QScriptValue RootedTreeNode::children_list() const
{
    QScriptValue value =  dataStructure()->engine()->newArray();
    foreach (const DataPtr &child, children()) {
        value.property("push").call(value, QScriptValueList() << child->scriptValue());
    }
    return value;
}


void RootedTreeNode::adjustPosition()
{
    DataPtr parent = nodeParent();
    const QRectF size = dataStructure()->document()->size();
    if (parent){
        qreal adjust = 0.0;
        foreach (PointerPtr p, nodeParent()->pointers(this->getData())){
            if (p->property("TreeEdge").toInt() >= 0){
                adjust = p->property("TreeEdge").toReal()/(numberOfChilds() - 1);
                break;
            }
        }
        const qreal vSize = dataStructure()->property("NodeSize").toReal();
        const qreal hSize = vSize * qPow(2, height()) * 0.5;
        qreal posX = parent->x() - hSize * numberOfChilds()/2.0 +
                     hSize * numberOfChilds() * adjust;
        qreal posY = parent->y() + vSize;
        setPos(posX, posY);
    } else if (qobject_cast<RootedTreeStructure*>(dataStructure().get())->rootNode().get() == this) {
        setPos(size.center().x(), size.top() + 100);
    }
}


qint8 RootedTreeNode::height() const
{
    qint8 l = 0, r = 0;
    if (leftChild().get())
        l = 1 + qobject_cast<RootedTreeNode*>(leftChild().get())->height();
    if (rightChild().get())
        r = 1 + qobject_cast<RootedTreeNode*>(rightChild().get())->height();
    return l < r?
               r:
               l;
}

