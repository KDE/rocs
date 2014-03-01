/*
    This file is part of Rocs.
    Copyright 2008       Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
    Copyright 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
#include <QIcon>
#include "GraphVisualEditor.h"
#include "Scene/GraphScene.h"
#include "DataStructure.h"
#include "Pointer.h"
#include "Scene/DataItem.h"
#include "Modifiers/Topology.h"

#include <QDebug>

#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random/linear_congruential.hpp>
#include <QPair>
#include <boost/graph/graph_concepts.hpp>


AlignAction::AlignAction(const QString& tooltip, AlignAction::Orientation o, GraphScene* gc)
    : KAction(QIcon(), tooltip, gc)
{
    _graphScene = gc;
    setupOrientation(o);

    connect(this, SIGNAL(triggered()), this, SLOT(align()));
}

void AlignAction::registerData(DataList dataList)
{
    _registeredData = dataList;
}

void AlignAction::unsetData()
{
    _registeredData.clear();
}

void AlignAction::setupOrientation(AlignAction::Orientation o)
{
    _orientation = o;
    switch (o) {
    case Left :
        setIcon(QIcon("rocsalignleft"));
        break;
    case Right :
        setIcon(QIcon("rocsalignright"));
        break;
    case Top :
        setIcon(QIcon("rocsaligntop"));
        break;
    case Bottom :
        setIcon(QIcon("rocsalignbottom"));
        break;
    case HCenter :
        setIcon(QIcon("rocsalignhmiddle"));
        break;
    case VCenter :
        setIcon(QIcon("rocsalignvmiddle"));
        break;
    case Circle :
        setIcon(QIcon("rocsaligncircle"));
        break;
    case MinCutTree :
        setIcon(QIcon("rocsaligntree"));
        break;
    }
}


void AlignAction::align()
{
    DataList dataList;
    if (_registeredData.empty()) {
        QList<QGraphicsItem*> itemList = _graphScene->selectedItems();
        foreach(QGraphicsItem * i, itemList) {
            if (DataItem *dataItem = qgraphicsitem_cast<DataItem*>(i)) {
                dataList.append(dataItem->data());
            }
        }
    } else {
        dataList = _registeredData;
    }

    if (dataList.size() < 1) {
        return;
    }
    switch (_orientation) {
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
        qSort(dataList.begin(), dataList.end(), topLessThan);
        alignY(dataList);
        break;
    case Circle :
        alignCircle(dataList);
        break;
    case MinCutTree :
        alignMinCutTree(dataList);
        break;
    }

    unsetData();
}

void AlignAction::alignY(DataList dataList)
{
    qreal final = dataList[0]->y();

    if (_orientation == VCenter || _orientation == HCenter) {
        qreal otherSide = dataList[dataList.size() - 1]->y();
        final = (final + otherSide) / 2;
    }
    foreach(DataPtr data, dataList) {
        data->setY(final);
    }
}

void AlignAction::alignX(DataList dataList)
{
    qreal final = dataList[0]->x();
    if (_orientation == VCenter || _orientation == HCenter) {
        qreal otherSide = dataList[dataList.size() - 1]->x();
        final = (final + otherSide) / 2;
    }
    foreach(DataPtr data, dataList) {
        data->setX(final);
    }
}

void AlignAction::alignCircle(DataList dataList)
{
    Topology topology = Topology();
    topology.applyCircleAlignment(dataList);
}

void AlignAction::alignMinCutTree(DataList dataList)
{
    Topology topology = Topology();
    topology.applyMinCutTreeAlignment(dataList);
}
