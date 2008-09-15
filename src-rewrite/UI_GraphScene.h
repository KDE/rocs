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

class GraphScene : public QGraphicsScene
{
	Q_OBJECT
	public:
		GraphScene ( QObject* parent, GraphCollection *item );
		void changeActiveGraph(Graph *g);
		void createGraphItem(Graph*);
		void removeGraphItem(int pos);
		QRectF boudingRect() const;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
		
		enum{ SingleSelection, // Selects only one node or edge
		      SquareSelection, // Create a Square that will select everything inside.
		      Move,            // Move the selected item(s)
		      AddNode,         // Adds a not at the Click pos.
		      AddEdge,        // Add an edge between 2 nodes.
		      MakeKGraph,      // Make a complete graph with n nodes.
		      MakeCGraph,      // Make a Circular graph with n nodes.
		      MakeWGraph       // Make a Whell Graph with n nodes.
	    };

	private:
		QRectF coords;
		GraphCollection *_graphCollection;
		QList<GraphItem*> _graphCollectionItems;
		Graph  *_activeGraph;

		int action;
		void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

		QGraphicsItem *tmpItem; //! pointer for the line that's being draw right now.
		MainWindow *mainWindow;

    public slots:
    /* ######################
       #   Script Control
       ###################### */

	//	void createScriptFunctions(const QString& program);
   // void insertEdge(Node *begin, Node *end, int arrowType = 0);
		void insertNode(QPointF pos);

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
   void createCGraph(QPointF pos);*/
//    int createConcentricNodes(int k = 0, qreal radius = 0, QPointF pos = QPointF()); 
};
#endif
