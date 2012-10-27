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

#include "RocsCoreExport.h"
#include "CoreTypes.h"

#include <boost/scoped_ptr.hpp>
#include <QList>
#include <QString>

class KUrl;
class ProjectPrivate;
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
    Project(const KUrl& projectFile);

    /**
     * Constructor for project that creates a project by configuration given
     * in file \p projectFile inside directory \p projectDirectory
     * \param projectFile is the absolute path to the project file
     */
    Project(const KUrl& projectArchive, const KUrl& projectDirectory);

    virtual ~Project();

    /**
     * Gives the working directory of the project. If no directory was set or the loaded
     * project files does not specify a directory, QString() is returned.
     * \return project directory
     */
    QString projectDirectory() const;

    KUrl projectFile() const;

    /**
     * Set path of project to \p fileUrl and set project to being non-temporary.
     * \param fileUrl is the designated path to the project
     */
    void setProjectFile(const KUrl& fileUrl);

    /**
     * Set project name.
     * \param name is the new name of the project
     */
    void setName(const QString& name);

    /**
     * Returns name of the project. Name can be set with \see setName(...).
     * \return name of the project
     */
    QString name() const;

    /**
     * Specifies if the project file is only temporary or not.
     * The status changes to non-temporary if a temporary file is saved.
     * \return true if this is a project not associated to a file, otherwise false
     */
    bool isTemporary();

    /**
     * Returns true if the poject is modified.
     * \return modification status
     */
    bool isModified() const;

    /**
     * Get list of all code files of the project that have a path.
     * \return list of URLs
     */
    QList<KUrl> codeFiles() const;

    /**
     * Get list of all temporary code files of the project, i.e., code files
     * that do not have a path.
     * \return list of URLs
     */
    QList<KTextEditor::Document*> codeFilesNew() const;

    /**
     * Add a new temporary code file to the project. This file has not a file name and must
     * be saved before it can be added to the project. Use \see saveCodeFileNew(...) to
     * save temporary file to actually file and update project configuration.
     * \param document is the temporary code file
     */
    void addCodeFileNew(KTextEditor::Document* document);

    /**
     * Add code file to project. The file is specified by \p file.
     * \param file is the local file URL
     */
    int addCodeFile(const KUrl& file);

    /**
     * Save a temporary code document to a code file and update association to the project.
     * This method calls save method of \p document, saves it to \p file, removes file from
     * set of temporary code files and puts it into the list of code files of the project.
     * \param document is the temporary code file
     * \param file is the local file URL to where the code file shall be saved
     */
    void saveCodeFileNew(KTextEditor::Document* document, const KUrl& file);

    /**
     * Remove code file \p document from list of temporary code files of the project.
     * \param document is the temporary document to be removed
     */
    void removeCodeFileNew(KTextEditor::Document* document);

    /**
     * Remove code file from the project. File is specified by its identifier in internal list.
     * \param fileID is key for QMap that organizes the files
     */
    void removeCodeFile(int fileID);

    /**
     * Get list of all graph files of the project that have a path.
     * \return list of local file URLs
     */
    QList<KUrl> graphFiles() const;

    /**
     * Add a new temporary graph document to the project. This file has not a file name and must
     * be saved before it can be added to the project. Use \see saveGraphFileNew(...) to
     * save temporary file to actually file and update project configuration.
     * \param document is the temporary code file
     */
    void addGraphFileNew(Document* document);

    /**
     * Add graph file to project. The file is specified by \p file.
     * \param file is the local file URL
     */
    int addGraphFile(const KUrl& file);

    /**
     * Save a temporary graph document to a graph file and update association to the project.
     * This method calls save method of \p document, saves it to \p file, removes file from
     * set of temporary graph files and puts it into the list of graph files of the project.
     * \param document is the temporary graph file
     * \param file is the file url to where the graph file shall be saved
     */
    void saveGraphFileNew(Document* document, const QString& file);

    /**
     * Save existing graph document file under new filename
     */
    void saveGraphFileAs(Document* document, const QString& file);

    /**
     * Remove graph file \p document from list of temporary graph files of the project.
     * \param document is the temporary document to be removed
     */
    void removeGraphFileNew(Document* document);

    /**
     * Remove graph file from the project. File is specified by its identifier in internal list.
     * \param fileID is key for QMap that organizes the files
     */
    void removeGraphFile(int fileID);

    /**
     * Set journal file of the project.
     * \param fileUrl is URL of local file.
     */
    void setJournalFile(const KUrl& file);

    /**
     * Returns URL to local file containing the project journal.
     * \return URL to local file
     */
    KUrl journalFile() const;

    /**
     * Save the project to its current file if empty or no filename is given. Otherwise if filename is
     * given, save the project in file \p fileUrl. If the project is temporary, i.e., isTemporary() returns
     * true, a file URL is mandatory.
     * \param fileUrl is the target file, expected as absolute path
     * \return true if save was successful, otherwise false
     */
    bool writeProjectFile(const QString& fileUrl = QString());

    /**
     * Exports the project with all its components (scripts, graph, journal) to an archive file.
     * \param exportUrl is the path to the newly created archive
     * \return true if archive was created, else false
     */
    bool exportProject(const KUrl& exportUrl);

private:
    bool writeNewProjectFile();
    boost::scoped_ptr<ProjectPrivate> d;
};

#endif // PROJECT_H
