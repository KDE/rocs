/* This file is part of Rocs,
	 Copyright (C) 2008 by:
	 Tomaz Canabrava <tomaz.canabrava@gmail.com>

	 Rocs is free software; you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation; either version 2 of the License, or
	 (at your option) any later version.

	 Rocs is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with Step; if not, write to the Free Software
	 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef SUI_GRAPHVISUALEDITOR_H
#define SUI_GRAPHVISUALEDITOR_H

#include <QWidget>

class GraphDocument;
class Graph;
class Node;
class Edge;
class KComboBox;
class QGraphicsScene;
class GraphScene;
class KColorButton;

class GraphVisualEditor : public QWidget{
Q_OBJECT
public:
	/*! Default Constructor 
	\param parent the owner of this widget. */
	GraphVisualEditor(QWidget *parent = 0);

	GraphScene *scene() const;

public slots:
/*! set the current selected Graph. 
	\param graph the new active graph.
	*/
	void setGraph( Graph *graph);

	void setGraphDocument(GraphDocument *graphDocument);

private:
	void setupWidgets();
	/*! as the name says, draw a graph on scene.
	\param g the graph to be drawn. */
	void drawGraphOnScene( Graph *g );

	/*! as the name says, it removes the current GraphDocument 
	from the screen and releases it from the evil hands of GraphEditWidget.*/
	void releaseGraphDocument();

	GraphScene *_scene;
	GraphDocument *_graphDocument;
	Graph *_graph;

	QWidget *_spacingPropContainer;
	QWidget *_graphPropContainer;
	QWidget *_edgePropContainer;
	QWidget *_nodePropContainer;
	QWidget *_toolbar;
	KComboBox *_graphComboBox;
	KColorButton *_colorButton;

	qreal _topNode;
	qreal _bottomNode;
	qreal _leftNode;
	qreal _rightNode;

	/*! Actions */
	
};

#endif
