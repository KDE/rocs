/*
    This file is part of Rocs.
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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


#ifndef PROJECT_H
#define PROJECT_H

#include <boost/scoped_ptr.hpp>
#include <QList>

class ProjectPrivate;
class QString;

/**
 * \class Project
 * \brief A "Project" object represents the compilation of file pointers that form a Rocs project.
 *
 * Use this class to maintain the set of all files belonging to the project. Thus, files specified
 * by the project file are a (not necessarily strict) superset of file currently displayed at
 * the main window widget.
 */
class Project
{

public:
    Project();
    virtual ~Project();

    void setProjectPath(QString directory);
    QString projectPath() const;

    void setName(QString name);
    QString name() const;

    void addCodeFile(QString file);
    void removeCodeFile(QString file);
    QList<QString> codeFiles() const;

    void addGraphDocumentFile(QString file);
    void removeGraphDocumentFile(QString file);
    QList<QString> graphDocumentFiles() const;

    void setJournalFile(QString file);
    QString journalFile() const;

private:
    boost::scoped_ptr<ProjectPrivate> d;
};

#endif // PROJECT_H
