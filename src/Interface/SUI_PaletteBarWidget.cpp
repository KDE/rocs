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

#include "SUI_PaletteBarWidget.h"

#include <KAction>
#include <QEvent>
#include <QToolButton>
#include <QActionGroup>
#include <QScrollArea>
#include <KLocale>
#include <KIcon>
#include <KDebug>


#include "SUI_MainWindow.h"
#include "SUI_PaletteScrollArea.h"
#include "SUI_PaletteLayout.h"
#include "SUI_Separator.h"
#include "SUI_GraphScene.h"

#include "action_AbstractAction.h"


#include "graphDocument.h"
#include "graph.h"

#include <KActionCollection>

#include "settings.h" //! AUTO GENERATED!  ( *Hate* this u_u )

PaletteBarWidget::PaletteBarWidget ( QWidget* parent ) 
  : QWidget (parent){
  setObjectName ( "palleteBar" );
   
  

  _scrollArea = new PaletteScrollArea ( this );
  _scrollArea->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
  _scrollArea->setFrameShape ( QFrame::NoFrame );

  _widget = new QWidget ( _scrollArea );
  _layout = new PaletteLayout ( _widget );
  _layout->setSpacing ( 0 );
  _layout->setOneLine ( Settings::showButtonText() );

  _scrollArea->setWidget ( _widget );
  _scrollArea->setMinimumWidth ( _widget->minimumSizeHint().width() );

  QVBoxLayout* topLayout = new QVBoxLayout ( this );
  topLayout->addWidget ( _scrollArea );
  
  QAction* showText = new QAction ( i18n ( "Show text" ), this );
  showText->setCheckable ( true );
  showText->setChecked ( Settings::showButtonText() );
  QObject::connect ( showText, SIGNAL ( toggled ( bool ) ), this, SLOT ( showButtonTextToggled ( bool ) ) );

  _widget->addAction ( showText );
  _widget->setContextMenuPolicy ( Qt::ActionsContextMenu );
  _actionGroup = new QActionGroup(this);
  _actionGroup -> setExclusive(true);
  
}

void PaletteBarWidget::setActionCollection(KActionCollection *collection){
  _actionCollection = collection;
  QList<QAction*> actions = collection->actions();
  QAction *action_pointer = actions.takeFirst();
  createToolButton(action_pointer, true);
  foreach(QAction *action, actions){
     createToolButton(action, false);
  }
}

void PaletteBarWidget::createToolButton ( QAction* action, bool checked ){
  QToolButton* button = new QToolButton (  );
  button->setToolButtonStyle ( Settings::showButtonText() ? Qt::ToolButtonTextBesideIcon : Qt::ToolButtonIconOnly );
  button->setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Fixed );
  button->setAutoRaise ( true );
  button->setChecked(checked);
  button->setIconSize ( QSize ( 22,22 ) );
  button->setDefaultAction ( action );
  _actionGroup->addAction( action );
  _toolButtons.append ( button );
  _layout->addWidget ( button );
}

void PaletteBarWidget::showButtonTextToggled ( bool b ){
  Settings::setShowButtonText ( b );
  Settings::self()->writeConfig();
  foreach ( QToolButton* button, _toolButtons ){
    button->setToolButtonStyle ( b ? Qt::ToolButtonTextBesideIcon : Qt::ToolButtonIconOnly );
  }
  _layout->setOneLine ( b );
  _scrollArea->setMinimumWidth ( _widget->minimumSizeHint().width() );
}


void PaletteBarWidget::setGraph( Graph* ){
  kDebug() << "Got the Graph but did nothing with it.";
  return;
}

void PaletteBarWidget::setGraphDocument( GraphDocument *){
  kDebug() << "Got the Graph Document but did nothing with it.";
  return;
}
