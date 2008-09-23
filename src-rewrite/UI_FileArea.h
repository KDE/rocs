#ifndef UI_FILEAREA_H
#define UI_FILEAREA_H

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
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */

#include <QWidget>
#include <QList>
#include <QVariant>

class MainWindow;
class QTreeWidget;
class QTreeWidgetItem;
class GraphCollection;

namespace KTextEditor
{
	class Document;
}

/*! 	
	\brief The Opened Files Area.
	This class is the Area where all the opened files are Stored
*/
class FileArea : public QWidget
{
	Q_OBJECT
	public:
		/*! 
			Default Constructor 
			\param parent the MainWindow that will hold the FileArea
		*/
		FileArea ( MainWindow *parent );
		
		/*! Creates a new Script File and append it to the TreeWidget */
		void createNewScript();
		
		/*! Creates a new Graph File and append it to the TreeWidget */
		void createNewGraph();
	
		/*! The types of files supported */
		enum {ScriptType = QVariant::UserType + 1, GraphType};

		/*! 
			gets the Script at a current position on the TreeWidget
			\return the document of the Script File at position pos 
		*/
		KTextEditor::Document* scriptAt ( int pos );

		/*! 
			gets the graphCollection
			\return the GraphCollection of the Graph File at position pos 
		*/
		GraphCollection* graphCollectionAt ( int pos );

	private:

		QTreeWidget* _treeWidget; /*!  The widget that holds all the items */
		/*! Those items here doesn't hold the Graphs or Scripts itself, but 
		   the position where they are on the QList _scripts and QList _graphCollections */

		QTreeWidgetItem* _scriptFolder; /*! the script folder inside of the TreeWidget */
		QTreeWidgetItem* _graphFolder; /*! Same as above, but for graphs */

		QList<KTextEditor::Document*> _scripts;	 /*! the list of the Opened Script Files */
		QList<GraphCollection*> _graphcollections; /*! The list of opened Graphs Files */

		MainWindow *_mainWindow; /*! The main Window */

		void  atualizeTreeWidget(); /*! When a item is added or removed, this is invocked */
};

#endif
