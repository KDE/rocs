/*
    This file is part of Rocs.
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>

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

#include "IncludeManager.h"
#include <QDebug>
#include <QFile>
#include <KLocalizedString>
#include <QDir>
#include <KDebug>
#include <KGlobal>
#include <kstandarddirs.h>

IncludeManager::IncludeManager()
{
    addPath(KGlobal::dirs()->findDirs("appdata", "examples"));

//     QStringList list = Settings::includePath();
//
//     while(!list.isEmpty()){
//       addPath(list.last());
//       list.removeLast();
//     }
//     kDebug() << _tempPath;

}
void IncludeManager::initialize(const QStringList& tempPath)
{
    _tempPath.clear();
    _wasIncluded.clear();

    addPath(KGlobal::dirs()->findDirs("appdata", "examples"));
    addPath(tempPath);
}


QString IncludeManager::include(const QString& script, const QString& actualPath, const QString &filename)
{
    int pos;
    bool inComment = false;
    if (!actualPath.isEmpty()) {
        _actualDir = QDir(actualPath); // try the path of saved file
        _tempPath << _actualDir;
        if (!seekFile(filename).isNull()) {
            _wasIncluded << seekFile(filename);
        }
    } else {
        if (!_tempPath.isEmpty()) {
            _actualDir = _tempPath.last(); // not disponible the path to saved file, use the first path from list
        } else {
            _actualDir = QDir(); //No path in list? use application path!
        }

    }
    QStringList lines = script.split('\n');
    for (int i = 0; i < lines.count(); ++i) {
        if (lines[i].indexOf("//") != -1) {
            /*lines[i] = */
            lines[i].truncate(lines[i].indexOf("//"));//Ignores after '//'
        }
        if (inComment) {
            if ((pos = lines[i].indexOf("*/")) != -1) {
                lines[i].remove(0, pos + 2);
                inComment = false;
            } else {
                lines.removeAt(i);
                --i;
                continue;
            }
        }
        while ((pos = lines[i].indexOf("/*")) != -1) {
            int pos2;
            if ((pos2 = lines[i].indexOf("*/", pos + 2)) != -1) {
                lines[i].remove(pos, pos2 - pos + 2);
            } else {
                lines[i].remove(pos, lines[i].count());

                inComment = true;
            }
        }
        if (lines[i].isEmpty()) {
            lines.removeAt(i);
            --i;
            continue;
        }
        QRegExp reg("^\\s*include\\s*\\(\\s*.*.js\\s*\\)");
        if (lines[i].indexOf(reg) != -1) {
            QString ret = processInclude(reg.cap());
            lines[i].replace(reg.cap(), ret);
        }

    }
    QString str = lines.join("\n");
    return str;

}

QString IncludeManager::processInclude(QString arg1)
{
    QString fileContent;
    QString file;
    int pos;
    QString path;
    if (arg1.indexOf('(') != -1) {
        file = arg1.replace(')', '(').section('(', 1, 1).trimmed();
        // To avoid more ifs-elses
        fileContent = QString("debug(\"%1\")").arg(i18n("Cannot open file %1.", file));
        // Add the path first
        if ((pos = file.lastIndexOf('/')) != -1) {  //add the path of file to list
            path = file.left(pos + 1);
//                 QString filename = file.right(file.length() - pos +1);
            if (!path.startsWith(QDir::rootPath())) {
                path.prepend(_actualDir.absolutePath() + '/');
            }
            _tempPath << QDir(path);
        }
//         then, try to open
        if (!_actualDir.exists(file)) {
            file = seekFile(file);
        } else
            file = _actualDir.absoluteFilePath(file);


        if (!file.isEmpty()) {
            if (!checkIfWasIncluded(file)) {
                _wasIncluded << file;
                QFile fp(file);
                if (fp.open(QFile::ReadOnly | QFile::Text)) {
                    fileContent = fp.readAll();
                    fileContent = include(fileContent);
                }
            } else {
                return QString();
            }
        }
    } else {
        fileContent = QString("debug(\"%1\")").arg(i18n("Invalid include directive: %1. Cannot find file in directive.", arg1));
    }

    return fileContent;
}


QString IncludeManager::seekFile(const QString & arg1)
{

    if (arg1.isEmpty()) {
        return QString();
    }
    if (arg1.indexOf('/') != -1) {
        QDir dir(arg1.section('/', 0, -2));
        if (dir.isAbsolute() && dir.exists(arg1))
            return arg1;
    }

    for (int count = _tempPath.count() - 1; count >= 0; -- count) {
        if (_tempPath.at(count).exists(arg1.trimmed())) {
            return _tempPath.at(count).absoluteFilePath(arg1.trimmed());
        }
    }
    return QString();
}

void IncludeManager::addPath(const QString& str)
{
    QString tmp = !str.endsWith('/') ? str + '/' : str;

    if (!tempPath().contains(tmp)) {
        _tempPath << QDir(tmp);
    }
}


void IncludeManager::addPath(const QStringList& str)
{
    foreach(const QString & s, str) {

        QDir dir(s);
        if (!_tempPath.contains(dir)) {
            _tempPath.append(dir);
        }
    }
}

QStringList const IncludeManager::tempPath() const
{
    QStringList list;
    foreach(const QDir & dir, _tempPath) {
        list << dir.path() + '/';
    }
    return list;
}
