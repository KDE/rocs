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

#ifndef INCLUDEMANAGER_H
#define INCLUDEMANAGER_H
#include <QObject>
#include <QStringList>
#include <QDir>

class IncludeManager {
    private:
        QList <QDir> _tempPath;
        QString processInclude ( QString arg1 );
        QString seekFile ( const QString& arg1 );
        QStringList _wasIncluded;
        QDir _actualDir;

    public:
        IncludeManager();
        ~IncludeManager(){ }

        QString include(const QString& script, const QString& actualPath = QString(), const QString& filename = QString());

        QStringList const tempPath() const;

        /** check if the file was included before. It's avoid cyclic includes.
        filename should be path to file not only the name of file.
        */
        bool checkIfWasIncluded(const QString &file){
	  return _wasIncluded.contains(file);
        }

        /** insert aditional path to seek files.*/
        void addPath(const QString& str);
        void addPath(const QStringList & str);

};

#endif // INCLUDEMANAGER_H
