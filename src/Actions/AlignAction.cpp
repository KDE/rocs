/*
    This file is part of Rocs.
    Copyright 2008  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008  Ugo Sangiori <ugorox@gmail.com>
    Copyright 2011  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "AlignAction.h"
#include <KIcon>
#include "GraphVisualEditor.h"
#include "GraphScene.h"
#include "generics.h"
#include "DataStructure.h"
#include "Pointer.h"
#include "DataItem.h"
#include "Core/Modifiers/Topology.h"

#include <KDebug>

#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random/linear_congruential.hpp>
#include <QPair>
#include <boost/graph/graph_concepts.hpp>

namespace boost {
void throw_exception( std::exception const & ) {} } // do noop on exception

AlignAction::AlignAction(const QString& tooltip, AlignAction::Orientation o, GraphScene *scene, QWidget *parent)
        : KAction(KIcon(), tooltip, parent) 
{
    m_orientation = o;
    _graphScene = scene;
    connect(this, SIGNAL(triggered()), this, SLOT(align()));
    switch (o) {
    case Left :
        setIcon(KIcon("rocsalignleft"));
        break;
    case Right :
        setIcon(KIcon("rocsalignright"));
        break;
    case Top :
        setIcon(KIcon("rocsaligntop"));
        break;
    case Bottom :
        setIcon(KIcon("rocsalignbottom"));
        break;
    case HCenter :
        setIcon(KIcon("rocsalignhmiddle"));
        break;
    case VCenter :
        setIcon(KIcon("rocsalignvmiddle"));
        break;
    case Circle :
        setIcon(KIcon("rocsaligncircle"));
        break;
    case MinCutTree :
        setIcon(KIcon("rocsaligntree"));
        break;
    }
}

void AlignAction::setDataStructure(DataStructurePtr dataStructure) {
    _dataStructure = dataStructure; 
}

void AlignAction::unsetDataStructure(DataStructurePtr dataStructure) {
    _dataStructure.reset();
}


void AlignAction::align() {
    DataList dataList;
    if (!_dataStructure) {
        QList<QGraphicsItem*> itemList = _graphScene->selectedItems();
        foreach(QGraphicsItem *i, itemList) {
            if ( DataItem *dataItem = qgraphicsitem_cast<DataItem*>(i) ) {
                dataList.append( dataItem->data() );
            }
        }
    } else {
        dataList = _dataStructure->dataList();
    }

    if (dataList.size() < 1) return;
    _graphScene->setHideEdges(true);
    switch (m_orientation) {
    case Left :
    case VCenter :
        qSort(dataList.begin(), dataList.end(),  leftLessThan);
        alignX(dataList);
        break;
    case Bottom :
    case HCenter :
        qSort(dataList.begin(), dataList.end(), bottomLessThan);
        alignY(dataList);
        break;
    case Right :
        qSort(dataList.begin(), dataList.end(), rightLessThan);
        alignX(dataList);
        break;
    case Top :
        qSort( dataList.begin(), dataList.end(), topLessThan);
        alignY(dataList);
        break;
    case Circle :
        alignCircle(dataList);
        break;
    case MinCutTree :
        alignMinCutTree(dataList);
        break;
    }

    _graphScene->setHideEdges(false);
}

void AlignAction::alignY(DataList dataList) {
    qreal final = dataList[0]->y();

    if (m_orientation == VCenter || m_orientation == HCenter) {
        qreal otherSide = dataList[dataList.size()-1]->y();
        final = (final + otherSide) / 2;
    }
    foreach(DataPtr data, dataList) {
        data->setY(final);
    }
}

void AlignAction::alignX(DataList dataList) {
    qreal final = dataList[0]->x();
    if (m_orientation == VCenter || m_orientation == HCenter) {
        qreal otherSide = dataList[dataList.size()-1]->x();
        final = (final + otherSide) / 2;
    }
    foreach(DataPtr data, dataList) {
        data->setX(final);
    }
}

void AlignAction::alignCircle(DataList dataList) {
    Topology topology = Topology();
    topology.applyCircleAlignment(dataList);
}

void AlignAction::alignMinCutTree(DataList dataList) {
    Topology topology = Topology();
    topology.applyMinCutTreeAlignment(dataList);
}
