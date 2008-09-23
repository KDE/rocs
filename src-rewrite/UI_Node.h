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

#ifndef UI_NODE_H
#define UI_NODE_H

#include <QGraphicsItem>

class Graph;
class EdgeItem;
class Node;

/*!
\brief Node being displayed on the screen
This class holds the Node abstraction and the Node item on the screen,
It should be split in two separated classes in the future.
*/
class NodeItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
	public:
		/*!
		default constructor
		\param Graph the Graph that this node belongs
		*/
		NodeItem (Graph *Graph );

		/*! 
		needed for some calculations of the positioning on the screen
		\return the square-size of the item.
		*/
		QRectF bounds();
	
		/*! the Type of all nodes, so we can use qobject_cast<T*> */
		enum{Type = UserType + 1};

		/*! The flags that will tell what's happening on a particular node. */
		enum{ 	Default 	= 0,	// 0000000
			Selected 	= 1,	// 0000001
			Focused 	= 2,	// 0000010
			Dragged 	= 4,	// 0000100
			Begin 		= 8,	// 0801000
			End 		= 16,	// 0010000
			Name 		= 32,	// 0100000
			Index 		= 64	// 1000000
		};
		
		/*! 
		gets the type of the Node, const for every node.
		\return the type.  
		*/
		int type() const { return Type; }
		
		/*! 
		changes the index of this node 
		\param i the new index
		*/
		void setIndex(int i) { _index = i; }
		
		/*! 
		gets the index of this node
		\return index of the node.
		*/	
		int index() const {return _index; }

		/*! 
		adds a 'to' node to this node. This will make the 'to' node to be connected by an edge. to this one. 
		\param to the node to connect with this one.
		*/
		void addTo ( NodeItem *to );

		/*! change the name of this particular node. 
		\param name the new name of the node. */
		void setName ( const QString& name );

		/*! changes the internal color (but not the outline) of this particular node.
		\param color the new color of the node. */
		void setColor ( const QColor& color );

		/*! moves the node from the atual position to the new x,y.
		\param x the x coordinate
		\param y the y coordinate.
		*/
		void setXY ( qreal x, qreal y );

	protected:
		unsigned char _action; //! what's happening right now with this node.
		
		Graph *_graph; //! the graph that this node belongs to.
		QString _name; //! the name of this node.
		int _index;	//! the position of this node on the Graph's list.
		qreal _x;	//! the x position on the screen
		qreal _y;	//! the y position on the screen

		QList<NodeItem*> _connections; //! all nodes that this node connects, note that if this node is connected.
		QList<EdgeItem*> _edges;	//! all the edges that this node has.

		QColor _colorFocused; //! the focused color of the node
		QColor _colorDragged;	//! the dragged color of the node
		QColor _colorSelected;	//! the selected color of the node
		QColor _colorDefault;	//! the default color of the node

		//! Methods
		/*! calculates the shape of the node. 
		\return a QPainterPath representation of the node. 
		*/
		QPainterPath shape() const; 

		/*! calculate the bounding Rectangle of the node 
		\return the QRectF bounding rectangle
		*/
		QRectF boundingRect() const;

		/*! paints the node on the screen. 
		\param painter the painter pointer
		\param option the QStyleOptionGraphcsItem options
		\param widget the widget that will have an effect on it.
		*/
		void paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
		void paintDefault();  //! paint the node as default if action & 0000000
		void paintSelected(); //! paints the selected node  if action & 0000001
		void paintFocused();  //! paints the focused node   if action & 0000010
		void paintDragged();  //! paints the Dragged node   if action & 0000100
		void paintAsBegin();  //! paint the node as Begin   if action & 0001000
		void paintAsEnd();    //! paint the node as End     if action & 0010000
		void paintName();     //! Paint the node's name     if action & 0100000
		void paintIndex();    //! Paint the node Index      if action & 1000000

};
#endif
