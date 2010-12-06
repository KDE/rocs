/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "ImporterExporterManager.h"
#include <KFileDialog>
#include <KLocalizedString>
#include "Plugins/FilePluginInterface.h"
#include <KDebug>
#include <PluginManager.h>
#include <KPushButton>
#include <kdebug.h>

#include <QFile>


#include <iostream>
#include <string>


using namespace Rocs;

ImporterExporterManager::ImporterExporterManager(QObject* parent): QObject(parent), _scriptToRun(QString())
{

}

bool ImporterExporterManager::exportFile(DataTypeDocument * doc) const
{
    QString ext;
    foreach ( FilePluginInterface *f, PluginManager::instance()->filePlugins() )
    {
        ext.append ( f->extensions().join ( "" ) );
    }
    ext.append ( i18n ( "*|All files" ) );


    KFileDialog exportDialog ( QString(),ext,qobject_cast< QWidget* >(parent()) );
    exportDialog.okButton()->setText ( i18n  ( "Export" ) );
    exportDialog.okButton()->setToolTip ( i18n ( "Export graphs to file" ) );
    if ( exportDialog.exec() != KDialog::Accepted )
    {
        return false;
    }

    kDebug() << "Exporting File..";
    if ( exportDialog.selectedFile() == "" )
    {
        return false;
    }

    ext = exportDialog.currentFilter().remove ( '*' );
    kDebug () <<" Selected to export: " << ext;
    QString file = exportDialog.selectedFile();
    if ( !file.endsWith ( ext) )
    {
        file.append ( ext );
    }

    FilePluginInterface * p = PluginManager::instance()->filePluginsByExtension ( ext );
    if ( !p )
    {
        kDebug() << "Cannot export file: " << file;
        return false;
    }

//     _mutex.lock();
    if ( !p->writeFile ( *doc, file ) )
    {
        kDebug() << "Error writing file: " << p->lastError();
        return false;
    }
    kDebug() << "File Exported!" << file;
    return true;

}
 DataTypeDocument* ImporterExporterManager::importFile()
{

    QString ext;

    foreach ( Rocs::FilePluginInterface *f, Rocs::PluginManager::instance()->filePlugins() ){
        ext.append ( f->extensions().join ( "" ) );
    }
    ext.append ( i18n ( "*|All files" ) );

    KFileDialog dialog ( QString(),ext, qobject_cast< QWidget* >(parent()) );
    dialog.setCaption ( i18n ( "Graph Files" ) );
    if ( !dialog.exec() ){
        return 0;
    }

    kDebug() << "Extensions:"<< ext;
    QString fileName = dialog.selectedFile();
    if ( fileName == "" ) return 0;

    int index = fileName.lastIndexOf ( '.' );
    Rocs::FilePluginInterface * f = 0;
    if ( index == -1 )
    {
        kDebug() << "Cant open file without extension.";
        return 0;
    }

    kDebug() << fileName.right ( fileName.count() - index );
    f = Rocs::PluginManager::instance()->filePluginsByExtension ( fileName.right ( fileName.count() - index ) );

    if ( !f ){
        kDebug() <<  "Cannot handle extension "<<  fileName.right ( 3 );
        return 0;
    }

    DataTypeDocument * gd = f->readFile ( fileName );
    if ( !gd ){
        kDebug() << "Error loading file" << fileName << f->lastError();
    }
   _scriptToRun = f->scriptToRun();
    return gd;
}

void Rocs::ImporterExporterManager::dialogExec()
{

}

bool Rocs::ImporterExporterManager::hasDialog()
{
  return false;
}
