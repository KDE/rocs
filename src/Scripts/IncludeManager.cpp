/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "IncludeManager.h"
#include <QDebug>
#include <QFile>
#include <KLocalizedString>
#include <QDir>
#include <KDebug>
#include <settings.h>

IncludeManager::IncludeManager() {
  QStringList list = Settings::includePath();
    addPath(list);
}


QString IncludeManager::include ( const QString& script, const QString& actualPath ) {

  if (!actualPath.isEmpty()){
    _actualDir = QDir(actualPath); // try the path of saved file
  }else{
    if(_tempPath.count() > 0){
        _actualDir = _tempPath.first(); // not disponible the path to saved file, use the first path from list
    }else{
      _actualDir = QDir(); //No path in list? use application path!
    }

  }
    QStringList lines = script.split('\n');
//     qDebug() << lines;
    for (int i = 0; i < lines.count(); ++i){
        if (!lines[i].startsWith("//")){

          QRegExp reg("^\\s*include\\s*\\(\\s*.*.js\\s*\\)");
          if (lines[i].indexOf(reg)!= -1){
//           if (lines[i].contains("include", Qt::CaseInsensitive)){

              QString ret = processInclude(reg.cap());
              lines[i].replace(reg.cap(),ret);
          }
        }
    }
    QString str = lines.join("\n");
//     qDebug() << str;
    return str;

}

QString IncludeManager::processInclude ( QString arg1 ) {
    QString fileContent;
    QString file;
    int pos;
    if (arg1.indexOf('(') != -1){
        file=arg1.replace(')', '(').section('(',1,1);
        // To avoid more ifs-elses
        fileContent = i18n("debug(\"Cannot open file %1.\")").arg(file);
        if (!_actualDir.exists(file)){
            QString filename = file;
            if ((pos = file.lastIndexOf('/')) != -1){
                QString path = file.left(pos+1);
                QString filename = file.right(file.length() - pos +1);
                _tempPath << QDir(path);
            }
            file = seekFile(filename);
        }
        if ( !file.isEmpty() ) {
            if ( !checkIfWasIncluded ( file ) ) {
                _wasIncluded << file;
                QFile fp ( file );
                if ( fp.open ( QFile::ReadOnly|QFile::Text ) ) {
                    fileContent = fp.readAll();
                    fileContent = include ( fileContent );
                }
            }else{
              return QString();
            }
        }
    }else{
        fileContent = i18n("debug(\"Invalid include directive: %1. Can't find file in directive.\")").arg(arg1);
    }

    return fileContent;
}


QString IncludeManager::seekFile ( QString arg1 ) {
//     QList<QDir>::const_iterator  end = _tempPath.end();
//     QList<QDir>::const_iterator  begin = _tempPath.end();
//     QList<QDir>::iterator  iter = _tempPath.end();
    kDebug() << _tempPath.count();
    for (int count = _tempPath.count() - 1; count >= 0; -- count){
        if (_tempPath.at(count).exists(arg1)){
            return _tempPath.at(count).absoluteFilePath(arg1);
        }
    }
    return QString();
}

void IncludeManager::addPath ( QStringList& str )
{
    foreach ( QString s, str ) {
      QDir dir(s);
        if ( !_tempPath.contains ( dir ) ) {
            _tempPath.append ( dir );
        }
    }
}

