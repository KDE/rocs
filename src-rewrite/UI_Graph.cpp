/***************************************************************************
 *   Copyright (C) 2005-2006 by Tomaz Canabrava and Ugo Sangiori           *
 *   tomaz.canabrava@gmail.com                                             *
 *   ugorox@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "UI_Graph.h"
#include "Graph.h"

GraphItem::GraphItem()
{
}

void GraphItem::showIndexes ( bool show )
{
	_showIndex = show;
}

void GraphItem::showNames ( bool show )
{
	_showNames = show;
}

void GraphItem::showLengths ( bool show )
{
	_showLength = show;
}

bool GraphItem::showLengthGlobal()
{
	return _showLength;
}

bool GraphItem::showIndexGlobal()
{
	return _showIndex;
}

bool GraphItem::showNamesGlobal()
{
	return _showNames;
}

void GraphItem::createNodeItem(Node *n)
{

}
void GraphItem::createEdgeItem(Edge *e)
{

}