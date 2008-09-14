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

#include "Edge.h"
#include "Node.h"
#include "Graph.h"

Edge::Edge ( Graph *graph )
{
	_graph = graph;
	_to    = 0;
	_from  = 0;
}

bool  Edge::isConnected ( Node *n1, Node *n2 )
{
	if ( _graph->property ( "graph_type" ) != Graph::DigraphType )
	{
		if ( ( ( _from == n1 ) && ( _to == n2 ) ) || ( ( _from == n2 ) && ( _to == n1 ) ) )
		{
			return true;
		}
	}
	else
	{
		if ( ( _from == n1 ) && ( _to == n2 ) )
		{
			return true;
		}
	}
	return false;
}

void  Edge::setFrom ( Node *from )
{
	_from = from;
}

void  Edge::setTo ( Node *to )
{
	_to = to;
}

Node* Edge::from()
{
	return _from;
}

Node* Edge::to()
{
	return _to;
}

void Edge::setLength ( double d )
{
	_length = d;
}
