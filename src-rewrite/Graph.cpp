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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include "Graph.h"
#include "Edge.h"
#include "Node.h"
#include <QPointF>
#include <KLocale>

Graph::Graph ( int type, QObject *parent ) : QObject ( parent )
{
	setName ( i18n ( "untitled" ) );
	setColor ( Qt::black );
	setProperty ( "graph_type", type );
}

void Graph::setName ( const QString& n )
{
	_name = n;
}

void Graph::addNode ( QPointF pos )
{
	Node *n = new Node ( this );
	n -> setXY ( pos.x(), pos.y() );
	_nodes.append ( n );
}

void Graph::addEdge ( int nodeIndex1, int nodeIndex2 )
{
	Node *n1 = _nodes[nodeIndex1];
	Node *n2 = _nodes[nodeIndex1];
	addEdge ( n1, n2 );
}

void Graph::addEdge ( Node *from, Node *to )
{
	Edge *e = new Edge ( this );
	e->setFrom ( from );
	e->setTo ( to );
	from -> addTo ( to );
	if ( property ( "graph_type" ) != DigraphType )
	{
		to -> addTo ( from );
	}
	_edges.append ( e );
}

void Graph::setColor ( const QColor& c )
{
	_color = c;
}

bool Graph::isSimple()
{
	return ( property ( "graph_type" ) == SimpleType );
}

bool Graph::isMultigraph()
{
	return ( property ( "graph_type" ) == MultigraphType );
}

bool Graph::isDigraph()
{
	return ( property ( "graph_type" ) == DigraphType );
}

bool Graph::isNull()
{
	if ( _edges.size() == 0 ) return true;
}

bool Graph::isComplete()
{
	int x = _nodes.size() - 1;
	foreach ( Node *n, _nodes )
	{
		if ( n->getEdges().size() != x ) return false;
	}
	return true;
}

bool Graph::isCircular()
{
	foreach ( Node *n, _nodes )
	{
		if ( n->getEdges().size() != 2 ) return false;
	}
	return true;
}

bool Graph::isWheel()
{
	int x = ( _nodes.size() - 1 ) * 2;
	if ( _edges.size() != x ) return false;

	x = _nodes.size() - 1;
	int z = 0;
	foreach ( Node *n, _nodes )
	{
		if ( n->getEdges().size() == 3 ) continue;
		if ( n->getEdges().size() == x ) z += 1;
	}
	if ( z == 1 ) return true;
	return false;
}

bool Graph::isCubic()
{
	if ( !isComplete() ) return false;
	if ( _nodes.size() == 0 ) return false;
	if ( _nodes[0]->getEdges().size() != 3 ) return false;
	return true;
}

void Graph::makeComplement()
{

}

QList< QList< int > > incidencyMatrix()
{

}

QList< QList < int > > adjacencyMatrix()
{

}
