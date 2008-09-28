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

#ifndef GRAPHCOLLECTION_H
#define GRAPHCOLLECTION_H

#include <QObject>
#include <QList>
#include <QString>

class Graph;

/*! 
	\brief All the Graphs on a Graph File.

	This class holds a collection of Graphs, 
   	It's the main class that will form the Graph XML File, 
*/
class GraphCollection : public QObject
{
		Q_OBJECT

	public:
		/*! 
			default Constructor 
			\param name the Name of this graph File.
		*/
		GraphCollection ( const QString& name = "untitled" );
	
		/*! Creates a new empty graph inside of the collection */
		Graph* createNewGraph();

		/*! 
			Removes the graph
			\param index the index of the Graph to be removed.
		*/
		void removeGraph ( int index );

		/*! 
			return the graph 
			\param index the index of the Graph to be returned.
			\return the Graph that has the index 'index' on the graph's list.
		*/
		Graph* At ( int index );

		/*! 
			gets the name of the File 
			\return the name of the Graph File
		*/
		QString name() { return _name; }

		/*! 
			Sets the name of the file 
			\param name the new name of the file.
		*/
		void setName ( const QString& name ) { _name = name; }
		
		/*! 
			\return a list of graphs 
		*/
		QList<Graph*> graphs() const;
	signals:
		/*! 
			Emited when a new graph is created
			\param the new graph created.
		*/
		void graphCreated ( Graph *g );

		/*! 
			Emited when a existing graph is removed 
			\param index the index of the removed graph 
		*/
		void graphRemoved ( int index );

	private:
		QList<Graph*> _graphs; 	//! a list of graphs
		QList<bool> vibility;	//! the visibility of each graph
		QString _name;		//! name of the file.
};

#endif
