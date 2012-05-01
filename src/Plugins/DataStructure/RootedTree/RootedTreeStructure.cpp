/*
    This file is part of RootedTree (Rocs Plugin).
    Copyright 2012  Wagner Reck <wagner.reck@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "RootedTreeStructure.h"
#include "Data.h"
#include "Pointer.h"
#include "Document.h"
#include "RootedTreeNode.h"


#include <boost/shared_ptr.hpp>

#include <QScriptEngine>

#include <KMessageBox>
#include "KDebug"


QScriptValue rootedTreeNodeToScriptValue(QScriptEngine */*engine*/, RootedTreeNode* const &in)
{ return in->scriptValue(); }

void rootedTreeNodeFromScriptValue(const QScriptValue &object, RootedTreeNode* &out)
{ out = qobject_cast<RootedTreeNode*>(object.toQObject()); }


namespace boost { void throw_exception( std::exception const & ) {} }

DataStructurePtr RootedTreeStructure::create(Document *parent) {
    return DataStructure::create<RootedTreeStructure>(parent);
}

DataStructurePtr RootedTreeStructure::create(DataStructurePtr other, Document *parent) {
    boost::shared_ptr<RootedTreeStructure> ds = boost::static_pointer_cast<RootedTreeStructure>(RootedTreeStructure::create(parent));
    
    ds->importStructure(other);
    return ds;
}

RootedTreeStructure::RootedTreeStructure ( Document* parent ) :
    DataStructure ( parent ), m_rootNode(0)
{
  addDynamicProperty("NodeSize", 100);
  addDynamicProperty("ChildCount", 2);
  addDynamicProperty("PointersRegion", 0.3);
  setShowAllPointers(false);
}

void RootedTreeStructure::importStructure(DataStructurePtr other)
{
    QSet <Data*> visited;
    QQueue<DataPtr> queue;
    QHash <Data*, DataPtr> fromOtherToNew;
    
    foreach(DataPtr p, other->dataList()){
        if (visited.contains(p.get()))
            continue;
        queue.enqueue(p);
        DataPtr newdata = addData(p->name(), 0);
        newdata->setColor(p->color());
        newdata->setValue(p->value());
        newdata->setX(p->x());
        newdata->setY(p->y());
        newdata->setWidth(p->width());
        if (!rootNode().get())
            set_root_node(qobject_cast< RootedTreeNode* >(newdata.get()));
        fromOtherToNew.insert(p.get(), newdata);
        while(!queue.isEmpty()){
            DataPtr n = queue.dequeue();
            if (!visited.contains(n.get())){
                visited.insert(n.get());
            }
                int childCount = 0;
                RootedTreeNode* newdataRootedNode = qobject_cast< RootedTreeNode* >(fromOtherToNew.value(n.get()).get());
                //Set the number of Max number of childrem
                newdataRootedNode->setNumberOfChilds(n->adjacent_data().count());
                foreach(boost::shared_ptr <Data > c, n->adjacent_data()){
                    if (!visited.contains(c.get())){
                        visited.insert(c.get());
                        queue.enqueue(c);
                        DataPtr childdata = addData(c->name(), 0);
                        childdata->setColor(c->color());
                        childdata->setValue(c->value());
                        childdata->setX(c->x());
                        childdata->setY(c->y());
                        childdata->setWidth(c->width());
                        fromOtherToNew.insert(c.get(), childdata);
                        
                        //set the child at childCount position
                        newdataRootedNode->setChild(childdata, childCount++);
                        //Set the parent
                        RootedTreeNode* rooted = qobject_cast< RootedTreeNode* >(childdata.get());
                        rooted->setNodeParent(DataPtr(newdataRootedNode));   
                    }
                }
                //Set the correct number of childrem (2 if there is no child.
                newdataRootedNode->setNumberOfChilds(childCount == 0 ? 2:childCount);
        }
    }
}


RootedTreeStructure::~RootedTreeStructure() {
}



QScriptValue RootedTreeStructure::add_data(const QString& name) {
    DataPtr n = addData(name, 0);
    n->setEngine(engine());
    return n->scriptValue();
}


///FIXME addeding multiple pointers.
PointerPtr RootedTreeStructure::addPointer(DataPtr from, DataPtr to, int dataType) {
    PointerPtr ptr = DataStructure::addPointer(from, to, dataType);
    if (ptr.get() && from->property("ClickPosition").isValid()){
        QPointF pos = from->property("ClickPosition").toPointF();
        const qreal pointersSize = property("PointersRegion").toReal();
        const qreal size = property("NodeSize").toReal();
        if (pos.y() <= size * pointersSize){
            ptr->setProperty("TreeEdge", -1);//Add as parent;
            kDebug() << "Added as parent";
        }
        else if (pos.y() >= size * (1+pointersSize)){
	    RootedTreeNode * fromNode = qobject_cast<RootedTreeNode*>(from.get());
            const qint16 childCount = fromNode->numberOfChilds();
            const qreal division = ((childCount * pointersSize) > 1 ?
                                    childCount * pointersSize * size:
                                    size) / childCount;
            qint32 childPos = static_cast<qint32> (pos.x()/division);
            ptr->setProperty("TreeEdge", childPos);
        }
    }
    return ptr;
}

DataPtr RootedTreeStructure::addData(QString name, int dataType)
{
    boost::shared_ptr<RootedTreeNode> n = boost::static_pointer_cast<RootedTreeNode>(
	RootedTreeNode::create(getDataStructure(), generateUniqueIdentifier(), dataType) );
    n->setName(name);
    return addData(n);
}


QScriptValue RootedTreeStructure::root_node() const
{
    if (m_rootNode)
        return m_rootNode->scriptValue();
    return QScriptValue();
}

void RootedTreeStructure::set_root_node(RootedTreeNode* node)
{
    m_rootNode = node;
}

DataPtr RootedTreeStructure::rootNode() const
{
    if (m_rootNode)
        return m_rootNode->getData();
    return DataPtr();
}

void RootedTreeStructure::setEngine(QScriptEngine* engine)
{
    DataStructure::setEngine(engine);
    qScriptRegisterMetaType<RootedTreeNode*>(engine, rootedTreeNodeToScriptValue, rootedTreeNodeFromScriptValue);
}


bool RootedTreeStructure::isShowingAllPointers() const{
  return m_showPointers;
}


void RootedTreeStructure::setShowAllPointers(const bool v)
{
  if (v != m_showPointers){
    m_showPointers = v;
    emit showPointersChanged(m_showPointers);
    emit changed();
  }
}

