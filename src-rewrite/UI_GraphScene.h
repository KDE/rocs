#ifndef UI_GRAPHSCENE_H
#define UI_GRAPHSCENE_H

/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <QGraphicsScene>

class MainWindow;
class GraphCollection;
class QGraphicsSceneMouseEvent;
class Graph;
class GraphItem;
class Node;

/*! 
\brief The graph drawing Area 

This class holds all the nodes, edges and stuff that will appear on the screen.
it also needs to respond to all actions created on the pallete bar 
and invockable from the Script Side.

*/

class GraphScene : public QGraphicsScene
{
	Q_OBJECT
	public:
		/*! 
		default constructor 
		\param parent the MainObject that holds this container.
		\param item all the Graphs of the file.
		*/

		GraphScene ( QObject* parent, GraphCollection *item );
		
		/*! 
		sets or changes the current active graph 
		\param g the new current active graph
		*/
		void changeActiveGraph(Graph *g);

		/*! 
		changes the action that will happen when the user clicks on the screen 
		\param action the new action that will be performed.
		*/
		void setAction(int action);

		/*! 
		gets the active graph 
		\return the active graph
		*/
		Graph *graph();

	private:
		
		GraphCollection *_graphCollection; /*! All The Graphs of the Graph File*/
		
		// QList<GraphItem*> _graphCollectionItems;
		Graph  *_activeGraph; /*! The active graph being edited */

		int _action; /*! The current action being performed on the active graph */

		/*! 
		this will happen when the user clicks on the screen 
		\param mouseEvent the MouseEvent send by Qt
		*/
		void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
		
		/*! 
		this will happen when the user clicks on the screen 
		\param mouseEvent the MouseEvent send by Qt
		*/
		void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
		
		/*! 
		this will happen when the user clicks on the screen 
		\param mouseEvent the MouseEvent send by Qt
		*/
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

		QGraphicsItem *tmpItem; /*! pointer for the line that's being draw right now. */
		MainWindow *mainWindow; /*! the MainWindow */

	public slots:
    /* ######################
       #   Script Control
       ###################### */

		//void createScriptFunctions(const QString& program);
		// void insertEdge(Node *begin, Node *end, int arrowType = 0);
		
		// void createGraphItem(Graph* g);
		// void removeGraphItem(int pos);
		
		//void createNodeItem(Node* n);

		/* UI Control */
		//    void allignVLeft();
		//    void allignVRight();
		//    void allignVCenter();
		//    void allignHTop();
		//    void allignHBottom();
		//    void allignHCenter();

		/*   void group();
		void ungroup();
		void removeFromGroup();
		void setStartNode();
		void setEndNode();
		void changeSelectedNodeColor(QColor c);
		void removeSelection();
		void removeEdge(Edge *e);
		void removeNode(Node *n);
		void removeGroup(Group *g);
		void toogleIndexVisibility();
		void createKGraph(QPointF pos);
		void createCGraph(QPointF pos);
		int createConcentricNodes(int k = 0, qreal radius = 0, QPointF pos = QPointF()); 
		*/
	signals:
		void executeAction(int action, QPointF pos);
};
#endif
