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

#ifndef GRAPH_H
#define GRAPH_H

#include <QList>
#include <QVariant>
#include <QColor>
#include <QString>

class EdgeItem;
class NodeItem;
class QString;

/*! 
	\brief the Graph itself
	This is a container class for Nodes and Edges and some helper methods to work with them.
	Also there are user-visible actions inside of this. those user-visible things should be placed 
	somewhere else in the future. 
*/
	
class Graph : public QObject
{
	Q_OBJECT
	public:
		/*! 
			default constructor 
			\param GraphType the type of the graph, can be Simple, Multigraph and Digraph.
		*/
		Graph ( int GraphType );

		/*! the possible graph types */
		enum Type{ SimpleType = QVariant::UserType + 1, MultigraphType ,DigraphType };

		/*! 
			check if the graph is Simple
			\return true if the graph is simple, false if it isn't.
		*/
		bool isSimple();

		/*! 
			check if the graph is MultiGraph 
			\return true if the graph is multi, false if it isn't.
		*/
		bool isMultigraph();

		/*! 
			check if the graph is Digraph.
			\return true if the graph is multi, false if it isn't.
		*/
		bool isDigraph();
		
		/*! 
			check if the graph is null 
			\return true if the graph is null, false if it isn't.
		*/
		bool isNull();

		/*! 
			check if the graph is complete. 
			\return true if the graph is complete, false if it isn't. 
		*/
		bool isComplete();

		/*! 
			Check if the graph is circular 
			\return true if the graph is circular, false if it isn't.
		*/
		bool isCircular();

		/*! 
			Check if the graph is a Whell 
			\return true if the graph is a Whell, false if it isn't.
		*/
		bool isWheel();

		/*! 
			Check if the graph is cubic. (each node has 3 vertices)
			\return true if the graph is cubic, false if it isn't.
		*/
		bool isCubic();

		/*! create the complement of the graph */
		void makeComplement();

		//! Change that to Eigen as soon as possible.
		QList< QList<int> > incidencyMatrix(); /*! create the incidency Matrix */
		QList< QList<int> > adjacencyMatrix(); /*! create the adjacency Matrix */
		
		QList<NodeItem*>& nodes();

	private:
		QColor  _color;   //! Color of the outlines of the Graph on the screen.
		QString _comment; //! Some Comment of the graph.
		QString _name;   //! Name of the Graph
		
		QList<EdgeItem*> _edges; //! List of edges
		QList<NodeItem*> _nodes; //! list of Nodes.
		


	public slots:
		/*! 
			change the Graph's name 
			\param name the new graph's name.
		*/
		void setName ( const QString& name ); 

		/*! 
			adds a new node on the graph 
			\param node The node to be inserted on the graph
		*/
		void addNode ( NodeItem *node );  

		/*! 
			connects 2 nodes with one edge 
			\param nodeIndex1 the first Index of a Node that will be connected.
			\param nodeIndex2 the second Index of a Node that will be connected.
		*/
		void addEdge ( int nodeIndex1, int nodeIndex2 ); 

		/*! 
			connects 2 nodes with one Edge 
			\param n1 the First node that will be connected.
			\param n2 the Second node that will be connected.
		
		*/
		void addEdge ( NodeItem *n1, NodeItem *n2 ); 

		/*! 
			change the graph's outline color 
			\param color the new color of the graph's outline.
		*/
		void setColor ( const QColor& c ); 
};

#endif
