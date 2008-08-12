/* This file is part of Rocs, 
   Copyright ( C ) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   ( at your option ) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "UI_ScriptingArea.h"
#include "UI_MainWindow.h"
#include <QLabel>
#include <KTextEdit>
#include <KPushButton>
#include <KTabWidget>
#include <QTextBrowser>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <KLocale>


ScriptingArea::ScriptingArea( MainWindow *parent ) : QWidget( parent ) {
    setObjectName( "scriptingArea" );

   createTabs(  );
   createDesignLayout(  );
}

void ScriptingArea::createTabs(  ){
    _txtLine = new QLabel( this );    _txtLine   -> setText( "" );
    _txtColumn = new QLabel( this );  _txtColumn -> setText( "" );
    _txtFile = new QLabel( this );    _txtFile   -> setText( "" );

    _lblLine   = new QLabel( this ); _lblLine   -> setText( i18n( "Line: " ));
    _lblColumn = new QLabel( this ); _lblColumn -> setText( i18n( "Column: " ));
    _lblFile   = new QLabel( this ); _lblFile   -> setText( i18n( "File: " ));

    _tabWidget = new KTabWidget( this );  _tabWidget -> setTabPosition( QTabWidget::South );
    
    _tabScript = new QWidget(  );  _tabWidget -> addTab( _tabScript, i18n( "Untitled" ) );    
    _tabDebug  = new QWidget(  );  _tabWidget -> addTab( _tabDebug, i18n( "Debug" ) );


    _btnClearDebug = new KPushButton( _tabDebug );  _btnClearDebug -> setText( i18n( "clear" ));
    
    _txtEditScript = new KTextEdit( _tabScript );
    _txtDebug = new QTextBrowser( _tabDebug );
    
    _tabWidget -> setCurrentIndex( 0 );
}

void ScriptingArea::createDesignLayout(  )
{
    QSpacerItem *horizontalSpacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    QSpacerItem* horizontalSpacer_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );

    QVBoxLayout *verticalLayout = new QVBoxLayout( this );
    QHBoxLayout *horizontalLayout = new QHBoxLayout(  );
    QVBoxLayout *verticalLayout_3 = new QVBoxLayout( _tabScript );
    QVBoxLayout *verticalLayout_2 = new QVBoxLayout( _tabDebug );
    QHBoxLayout *horizontalLayout_2 = new QHBoxLayout(  );

    horizontalLayout -> addWidget( _lblLine );    
    horizontalLayout -> addWidget( _txtLine );
    horizontalLayout -> addWidget( _lblColumn );
    horizontalLayout -> addWidget( _txtColumn );
    horizontalLayout -> addWidget( _lblFile );
    horizontalLayout -> addWidget( _txtFile );
    horizontalLayout -> addItem( horizontalSpacer );

    verticalLayout -> addLayout( horizontalLayout );
    verticalLayout_3 -> addWidget( _txtEditScript );
    verticalLayout_2 -> addWidget( _txtDebug );
    
    horizontalLayout_2 -> addWidget( _btnClearDebug );
    horizontalLayout_2 -> addItem( horizontalSpacer_2 );
    verticalLayout_2 -> addLayout( horizontalLayout_2 );
    verticalLayout -> addWidget( _tabWidget );

}
