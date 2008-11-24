/* This file is part of Step, modified to work on Rocs.
   Copyright (C) 2007 Vladimir Kuznetsov <ks.vladimir@gmail.com> (Original Author)
   Tomaz Canabrava - Rocs Modifications

   Step and Rocs are free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Step and Rocs are distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef SUI_PALETTEBARDOCKWIDGET_H
#define SUI_PALETTEBARDOCKWIDGET_H

#include <QDockWidget>
#include <QList>

class QVBoxLayout;
class QToolButton;
class QScrollArea;
class QAction;
class QActionGroup;
class PaletteLayout;
class GraphScene;
class AbstractAction;
class KActionCollection;
class GraphDocument;
class Graph;

/*! \brief this class holds all the buttons of the pallete.
this is a container class for use with AbstractAction classes that will create
a item for each AbstractAction instance on the menu.
*/
class PaletteBarDockWidget : public QDockWidget{
  Q_OBJECT
  public:
    /*!   default constructor 
      \param parent the MainWindow
      \param flags the window flags.
    */
    explicit PaletteBarDockWidget ( QWidget* parent = 0, Qt::WindowFlags flags = 0 );

    /*! this method will configure the action pallette inserting 
    a button for each action inside of the collection.
    \param collection the collection of the KActions. */
    void setActionCollection(KActionCollection *colletion);

  public slots:
    /*! changes the GraphDocument and relink all actions to this new document. 
    \param document the new document that all actions will point to.
    FIXME: remove this? it's doing nothing.
    */
    void setGraphDocument( GraphDocument *document);

    /*! changes the active graph and relink all actions to this new graph. 
    \param graph the new graph that all actions will point to. 
    FIXME: remove this? it's doing nothing.
    */
    void setGraph( Graph* graph);

    /*! shows or hides text on buttons.
    \param b true to show, false to hide.
    */
    void showButtonTextToggled ( bool b );   
    
    /*! creates a toolbutton and places it on the pallete.
    \param action the action of with the button will take it's functionalies. */

    void createToolButton ( QAction* action , bool checked);

  protected:
    
    QScrollArea     *_scrollArea;
    QWidget         *_widget;
    PaletteLayout   *_layout;

    KActionCollection *_actionCollection;
    QActionGroup *_actionGroup;
    QList<QToolButton*> _toolButtons;
};

#endif
