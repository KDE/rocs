/*
    This file is part of Rocs.
    Copyright 2010  Wagner Reck <wagner.reck@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INCLUDEMANAGER_H
#define INCLUDEMANAGER_H
#include <QObject>
#include <QStringList>
#include <QDir>

#include "rocslib_export.h"

class ROCSLIB_EXPORT IncludeManager
{
private:
    QList <QDir> _tempPath;
    QString processInclude(QString arg1);
    QStringList _wasIncluded;
    QDir _actualDir;

public:
    IncludeManager();
    ~IncludeManager() { }

    QString include(const QString& script, const QString& actualPath = QString(), const QString& filename = QString());

    QStringList const tempPath() const;

    /** check if the file was included before. It's avoid cyclic includes.
    filename should be path to file not only the name of file.
    */
    bool checkIfWasIncluded(const QString &file) {
        return _wasIncluded.contains(file);
    }
    /** @brief Try find the fileName in the paths' list. If found, returns an absolute path to the file, otherwise return QString().
     *  In the case of fileName be a absolute path to file, the fileName is returned.
     */
    QString seekFile(const QString& arg1);

    /** insert additional path to seek files.*/
    void addPath(const QString& str);
    void addPath(const QStringList & str);

    void initialize(const QStringList& tempPath = QStringList());

};

#endif // INCLUDEMANAGER_H
