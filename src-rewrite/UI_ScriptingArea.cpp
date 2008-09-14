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
#include <ktexteditor/document.h>
#include <ktexteditor/view.h>
#include <ktexteditor/editor.h>
#include <KMessageBox>
#include <KXMLGUIFactory>

#include "UI_ScriptingArea.moc"

ScriptingArea::ScriptingArea ( MainWindow *parent ) : QWidget ( parent )
{

	_txtDebug              = 0;
	_btnClearDebug         = 0;
	_tabDebug              = 0;
	_tabScriptLayout       = 0;
	_tabScript             = 0;
	_txtEditScriptView     = 0;
	_tabWidget             = 0;
	_txtEditScriptDocument = 0;

	_mainWindow = parent;

	setObjectName ( "scriptingArea" );

	createTabs( );
	createDesignLayout( );
}

void ScriptingArea::createTabs( )
{
	_tabWidget = new KTabWidget ( this );  _tabWidget -> setTabPosition ( QTabWidget::South );

	_tabScript = new QWidget( );  _tabWidget -> addTab ( _tabScript, i18n ( "Untitled" ) );
	_tabDebug  = new QWidget( );  _tabWidget -> addTab ( _tabDebug, i18n ( "Debug" ) );

	_btnClearDebug = new KPushButton ( _tabDebug );  _btnClearDebug -> setText ( i18n ( "clear" ) );

	_txtDebug = new QTextBrowser ( _tabDebug );
	_tabWidget -> setCurrentIndex ( 0 );


}

KTextEditor::View* ScriptingArea::view()
{
	return _txtEditScriptView;
}

void ScriptingArea::setDocument ( KTextEditor::Document *d )
{
	if ( _txtEditScriptView != 0 )
	{
		_mainWindow->guiFactory()->removeClient ( _txtEditScriptView );
		delete _txtEditScriptView;
		_txtEditScriptView = 0;
	}

	_txtEditScriptDocument = d;
	_txtEditScriptView  = qobject_cast<KTextEditor::View*> ( d->createView ( this ) );
	_tabScriptLayout -> addWidget ( _txtEditScriptView );
	_mainWindow->guiFactory()->addClient ( _txtEditScriptView );
}

void ScriptingArea::createDesignLayout( )
{
	QSpacerItem* horizontalSpacer_2 = new QSpacerItem ( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );

	QVBoxLayout *verticalLayout = new QVBoxLayout ( this );
	QVBoxLayout *verticalLayout_2 = new QVBoxLayout ( _tabDebug );
	_tabScriptLayout = new QVBoxLayout ( _tabScript );

	QHBoxLayout *horizontalLayout_2 = new QHBoxLayout( );

	verticalLayout_2 -> addWidget ( _txtDebug );

	horizontalLayout_2 -> addWidget ( _btnClearDebug );
	horizontalLayout_2 -> addItem ( horizontalSpacer_2 );
	verticalLayout_2   -> addLayout ( horizontalLayout_2 );
	verticalLayout     -> addWidget ( _tabWidget );

}

const QString ScriptingArea::getScriptText()
{
	return _txtEditScriptDocument->text();
}
