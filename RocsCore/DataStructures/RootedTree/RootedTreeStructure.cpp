/*
    This file is part of RootedTree (Rocs Plugin).
    Copyright 2012  Wagner Reck <wagner.reck@gmail.com>

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

DataStructurePtr RootedTreeStructure::create(Document *parent)
{
    return DataStructure::create<RootedTreeStructure>(parent);
}


DataStructurePtr RootedTreeStructure::create(DataStructurePtr other, Document *parent)
{
    boost::shared_ptr<RootedTreeStructure> ds = boost::static_pointer_cast<RootedTreeStructure>(create(parent));
    ds->importStructure(other);
    return ds;
}


RootedTreeStructure::RootedTreeStructure ( Document* parent ) :
    DataStructure ( parent ), m_rootNode(0)
{
    addDynamicProperty("NodeSize", 100);
    addDynamicProperty("ChildCount", 2);
    addDynamicProperty("PointersRegion", 0.3);
    m_showPointers = false;
}


void RootedTreeStructure::importStructure(DataStructurePtr other)
{
    QSet <Data*> visited; // used to track which data elements were already processed
    QQueue<DataPtr> queue; // list of unprocessed data elements
    QHash <Data*, DataPtr> fromOtherToNew; // mapping from old to new data elements

    //FIXME only default data type considered
    foreach(DataPtr dataOther, other->dataList(0)){
        if (visited.contains(dataOther.get())) {
            continue;
        }
        queue.enqueue(dataOther);

        // add data element to tree and register mapping
        DataPtr dataTree = createData(dataOther->property("name").toString(), 0);
        dataTree->setColor(dataOther->color());
        dataTree->setProperty("value", dataOther->property("value").toString());
        dataTree->setX(dataOther->x());
        dataTree->setY(dataOther->y());
        dataTree->setWidth(dataOther->width());
        fromOtherToNew.insert(dataOther.get(), dataTree);

        // if no tree root is present, set this data element as root
        if (!rootNode()) {
            set_root_node(qobject_cast<RootedTreeNode*>(dataTree.get()));
        }

        // process all already met unprocessed data elements
        while(!queue.isEmpty()) {
            DataPtr n = queue.dequeue();
            if (!visited.contains(n.get())) {
                visited.insert(n.get());
            }
            int childCount = 0;
            RootedTreeNode* newdataRootedNode = qobject_cast< RootedTreeNode* >(fromOtherToNew.value(n.get()).get());
            newdataRootedNode->setNumberOfChilds(n->adjacentDataList().count()); //FIXME here it goes wrong!

            // iterate all neighbors and process them if not already elements in tree
            foreach (DataPtr adjacentData, n->adjacentDataList()) {
                if (!visited.contains(adjacentData.get())){
                    visited.insert(adjacentData.get());
                    queue.enqueue(adjacentData);
                    DataPtr childdata = createData(adjacentData->property("name").toString(), 0);
                    childdata->setColor(adjacentData->color());
                    childdata->setProperty("value", adjacentData->property("value").toString());
                    childdata->setX(adjacentData->x());
                    childdata->setY(adjacentData->y());
                    childdata->setWidth(adjacentData->width());
                    fromOtherToNew.insert(adjacentData.get(), childdata);

                    //set the child at childCount position
                    newdataRootedNode->setChild(childdata, childCount++);
                    //Set the parent
                    RootedTreeNode* rooted = qobject_cast< RootedTreeNode* >(childdata.get());
                    rooted->setNodeParent(newdataRootedNode->getData());
                }
            }
            // set the correct number of childrem (2 if there is no child)
            newdataRootedNode->setNumberOfChilds(childCount == 0 ? 2:childCount);
        }
    }
}


RootedTreeStructure::~RootedTreeStructure()
{
}


QScriptValue RootedTreeStructure::add_data(const QString& name)
{
    DataPtr n = createData(name, 0);
    n->setEngine(engine());
    return n->scriptValue();
}


///FIXME addeding multiple pointers.
PointerPtr RootedTreeStructure::createPointer(DataPtr from, DataPtr to, int dataType)
{
    PointerPtr ptr = DataStructure::createPointer(from, to, dataType);
    if (ptr && from->property("ClickPosition").isValid()){
        QPointF pos = from->property("ClickPosition").toPointF();
        const qreal pointersSize = property("PointersRegion").toReal();
        const qreal size = property("NodeSize").toReal();
        if (isShowingAllPointers()){
            qint8 treeEdge = -1;
            if (pos.y() > size * pointersSize ){
                RootedTreeNode * fromNode = qobject_cast<RootedTreeNode*>(from.get());

                const qint16 childCount = fromNode->numberOfChilds();
                const qreal division = ((childCount * pointersSize) > 1 ?
                                        childCount * pointersSize * size:
                                        size) / childCount;
                treeEdge = static_cast<qint32> (pos.x()/division);
            }
            foreach (const PointerPtr &p, from->outPointerList()) {
                if (p != ptr && p->property("TreeEdge").isValid() && p->property("TreeEdge").toInt() == treeEdge){
                    p->remove();
                }
            }
            ptr->setProperty("TreeEdge", treeEdge);//Add as parent;
        }else{
            //Need a fix to this click position
            to->setProperty("ClickPosition", QVariant());
            from->setProperty("ClickPosition", QVariant());
            //add the data as one of the children (first null child)
            bool found = false;
            RootedTreeNode * fromNode = qobject_cast<RootedTreeNode*>(from.get());
            for (quint32 i = 0; i < fromNode->numberOfChilds(); ++i){
                if (!fromNode->child(i)){
                    found = true;
                    ptr->setProperty("TreeEdge", i);
                    break;
                }
            }
            //If from data is full of children, increase number of children
            if (!found){
                fromNode->setNumberOfChilds(fromNode->numberOfChilds() + 1);
                ptr->setProperty("TreeEdge", fromNode->numberOfChilds()-1);
            }

            // Set from as parent of to
            RootedTreeNode * toNode = qobject_cast<RootedTreeNode*>(to.get());
            toNode->setNodeParent(from);
        }
    }
    return ptr;
}

DataPtr RootedTreeStructure::createData(const QString& name, int dataType)
{
    Q_ASSERT(document()->dataTypeList().contains(dataType));
    boost::shared_ptr<RootedTreeNode> n = boost::static_pointer_cast<RootedTreeNode>(
        RootedTreeNode::create(getDataStructure(), generateUniqueIdentifier(), dataType) );
    n->setProperty("name", name);
    addData(n);
    return n;
}


QScriptValue RootedTreeStructure::root_node() const
{
    if (m_rootNode) {
        return m_rootNode->scriptValue();
    }
    return QScriptValue();
}


void RootedTreeStructure::set_root_node(RootedTreeNode* node)
{
    m_rootNode = node;
}


DataPtr RootedTreeStructure::rootNode() const
{
    if (m_rootNode) {
        return m_rootNode->getData();
    }
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
	qDebug() << "Changing!";
        m_showPointers = v;
        emit showPointersChanged(m_showPointers);
        emit changed();
    }
}

