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

#include "rocslib_export.h"
#include "Rocs_Typedefs.h"

#include <boost/scoped_ptr.hpp>
#include <QList>

class KUrl;
class ProjectPrivate;
class QString;
class Document;

namespace KTextEditor
{
    class Document;
}

/**
 * \class Project
 * \brief A "Project" object represents the compilation of file pointers that form a Rocs project.
 *
 * Use this class to maintain the set of all files belonging to the project. Thus, files specified
 * by the project file are a (not necessarily strict) superset of file currently displayed at
 * the main window widget.
 */
class ROCSLIB_EXPORT Project
{

public:
    /**
     * Constructor for project that creates a new project with empty configuration.
     * A temporary project file is used and \see temporary() will return true until
     * project is saved.
     */
    Project();

    /**
     * Constructor for project that creates a project by configuration given
     * in file \p projectFile.
     * \param projectFile is the absolute path to the project file
     */
    Project(QString projectFile);
    virtual ~Project();

    void setProjectDirectory(KUrl directory);
    QString projectDirectory() const;

    void setName(QString name);
    QString name() const;

    int addCodeFile(KUrl file);
    void removeCodeFile(int fileID);
    QList<KUrl> codeFiles() const;
    QList<KTextEditor::Document*> codeFilesNew() const;

    /**
     * Add a new code document to the project hat does not have a filename, yet.
     */
    void addCodeFileNew(KTextEditor::Document* document);
    void removeCodeFileNew(KTextEditor::Document* document);
    void saveCodeFileNew(KTextEditor::Document* document, KUrl file);

    int addGraphFile(KUrl file);
    void removeGraphFile(int fileID);
    QList<KUrl> graphFiles() const;

    /**
     * Add a new graph document to the project hat does not have a filename, yet.
     */
    void addGraphFileNew(Document* document);

    /**
     * Save existing graph document file under new filename
     */
    void saveGraphFileAs(Document* document, QString file);

    void setJournalFile(QString file);
    QString journalFile() const;

    /**
     * Specifies if the project file is only temporary or not.
     * The status changes to non-temporary if a temporary file is saved.
     */
    bool isTemporary();

    bool writeProjectFile();

    /**
     * Save project file under \p file that is given as absolute file path.
     * \param file is the target file
     */
    bool writeProjectFile(QString file);

private:
    void removeGraphFileNew(Document* document);
    void saveGraphFileNew(Document* document, QString file);

    bool writeNewProjectFile();
    boost::scoped_ptr<ProjectPrivate> d;
};

#endif // PROJECT_H
