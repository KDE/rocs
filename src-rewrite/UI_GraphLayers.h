#ifndef UI_GRAPHLAYERS_H
#define UI_GRAPHLAYERS_H

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

#include <QWidget>
#include <QList>

class MainWindow;
class KPushButton;
class QImage;
class QTreeWidget;
class Graph;
class GraphCollection;

/*! 
	\brief The Graph-Layer widget.
	This Holds in a layer-way (like Photoshop Layers) the Graphs of the current opened Graph File 
*/
class GraphLayers : public QWidget
{
	public:
		 /*! 
		default constructor

		\param parent the MainWindow of the program.
		*/
		GraphLayers ( MainWindow* parent );
		
		/*! 
		Change the graph's collection what this widget will show.
		When the graph file is changed, this method must be invocked
 
		\param graphs the new graphCollection to be shown on this widget.
		*/
		void setGraphs(GraphCollection *graphs);
	
	public slots:
		/*! creates a new graph */
		void add();

		/*! duplicates the currently selected graph */
		void duplicate();

		/*! removes the currently selected graph */
		void remove();

	private:
		Graph *_activeGraph; /*! the currently selected Graph */
		QList<Graph*> _graphCollection; /*! All the Graphs */

		KPushButton *_btnAdd; /*! the 'add graph' button */
		KPushButton *_btnDuplicate; /*! the  'duplicate graph' button */
		KPushButton *_btnDelete;	/*! the 'delete graph' button */
		QTreeWidget *_centralArea;	/*! the tree view that will 
						store all the graphs indexes */
};

#endif
