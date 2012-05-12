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

#include "RootedTreeNodeItem.h"
#include "RootedTreeNode.h"
#include <boost/shared_ptr.hpp>
#include <QPainter>
#include <QGraphicsScene>
#include "DataStructure.h"


RootedTreeNodeItem::RootedTreeNodeItem(DataPtr n) : DataItem(n){


}

RootedTreeNodeItem::~RootedTreeNodeItem(){};

QRectF RootedTreeNodeItem::boundingRect() const
{
    if (data() && data()->dataStructure()){
        const qreal size = data()->dataStructure()->property("NodeSize").toReal();
        if (data()->dataStructure()->property("ShowAllPointers").toBool()) {
            RootedTreeNode* node = qobject_cast<RootedTreeNode*>(data().get());
            const qreal pointersSize = data()->dataStructure()->property("PointersRegion").toReal();
            const qint16 childCount = node->numberOfChilds();
            return QRectF(0,0,(childCount * pointersSize) > 1 ?
                                childCount * pointersSize * size:
                                size,
                        size * 1.6);
        } else {
            return DataItem::boundingRect(); /*QRectF(0,0,size, size );*/
        }

    }
    return QRectF();
}


void RootedTreeNodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    RootedTreeNode * node = qobject_cast< RootedTreeNode* >(data().get());
    if (!node || !node->dataStructure()) {
        return;
    }

    const qreal size  = node->dataStructure()->property("NodeSize").toReal();
    qDebug() << "multipointer? " << data()->dataStructure()->property("ShowAllPointers").toBool();
    if (data()->dataStructure()->property("ShowAllPointers").toBool()){
      
        const qreal pointersSize = node->dataStructure()->property("PointersRegion").toReal();
        qint16 childCount = node->numberOfChilds();

        const qreal width = (childCount * pointersSize) > 1 ?
                            childCount * pointersSize * size:
                            size;

        painter->drawRoundedRect(0,0, width, size * 1.6, width*0.01,size *0.01);
        painter->drawLine(0,size*pointersSize, width, size*pointersSize);
        painter->drawLine(0,size*(1+pointersSize), width, size*(1+pointersSize));

        const qreal division = width / childCount;
        if (!node->child(0)){//!node->right())
            painter->drawLine(division*0.2, size*1.55, division*0.8, size*(1+pointersSize+0.05));
        }
        for (int i = 1; i < childCount; ++i){
            painter->drawLine(division*i,size*(1+pointersSize), division*i, size*1.6);
            if (!node->child(i))//!node->right(qobject_cast<RootedTreeNode*>))
                painter->drawLine(division*i+division*0.2, size*1.55, division*i+division*0.9, size*(1+pointersSize+0.05));
        }
        if (!(node->nodeParent().get())) {
            painter->drawLine(width*0.3, size*0.25, width*0.7, size*0.05);
        }
    } else {
        DataItem::paint(painter,option, widget);
    }
//         painter->drawEllipse(boundingRect());
}
