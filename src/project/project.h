/*
 *  Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PROJECT_H
#define PROJECT_H

#include "libgraphtheory/editor.h"
#include "libgraphtheory/typenames.h"

#include <QList>
#include <QString>
#include <QScopedPointer>

class QUrl;
class ProjectPrivate;

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
class Project : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor for project that creates a new project with empty configuration.
     * A temporary project file is used and \see temporary() will return true until
     * project is saved.
     */
    explicit Project();

    /**
     * Constructor for project that creates a project by configuration given
     * in file \p projectFile.
     *
     * \param projectFile is the absolute path to the project file
     */
    explicit Project(const QUrl &projectFile);

    /**
     * Constructor for project that creates a project by configuration given
     * in file \p projectFile inside directory \p projectDirectory
     *
     * \param projectArchive the local file path to the project file
     * \param projectDirectory the local file path to the project directory
     */
    explicit Project(const QUrl &projectArchive, const QUrl &projectDirectory);

    virtual ~Project();

    /**
     * Gives the working directory of the project. If no directory was set or the loaded
     * project files does not specify a directory, QString() is returned.
     *
     * \return project directory
     */
    QString projectDirectory() const;

    QUrl projectFile() const;

    /**
     * Set path of project to \p fileUrl and set project to being non-temporary.
     *
     * \param fileUrl is the designated path to the project
     */
    void setProjectFile(const QUrl &fileUrl);

    /**
     * Set graph editor responsible for all graph documents. Set this to ensure
     * layout settings from graph editor to be applied to documents.
     */
    void setGraphEditor(GraphTheory::Editor *graphEditor);

    /**
     * Set project name.
     *
     * \param name is the new name of the project
     */
    void setName(const QString &name);

    /**
     * Returns name of the project. Name can be set with \see setName(...).
     *
     * \return name of the project
     */
    QString name() const;

    /**
     * Specifies if the project file is only temporary or not.
     * The status changes to non-temporary if a temporary file is saved.
     *
     * \return true if this is a project not associated to a file, otherwise false
     */
    bool isTemporary();

    /**
     * Set modified value of project to \p modified.
     */
    void setModified(bool modified=true);

    /**
     * Returns true if the poject is modified.
     *
     * \return modification status
     */
    bool isModified() const;

    /**
     * Get list of all code files of the project that have a path.
     *
     * \return list of URLs
     */
    QList<QUrl> codeFiles() const;

    /**
     * Get list of all temporary code files of the project, i.e., code files
     * that do not have a path.
     *
     * \return list of URLs
     */
    QList<KTextEditor::Document*> codeFilesNew() const;

    /**
     * Add a new temporary code file to the project. This file has not a file name and must
     * be saved before it can be added to the project. Use \see saveCodeFileNew(...) to
     * save temporary file to actually file and update project configuration.
     *
     * \param document is the temporary code file
     */
    void addCodeFileNew(KTextEditor::Document *document);

    /**
     * Add code file to project. The file is specified by \p file.
     *
     * \param file is the local file URL
     */
    int addCodeFile(const QUrl &file);

    /**
     * Save a temporary code document to a code file and update association to the project.
     * This method calls save method of \p document, saves it to \p file, removes file from
     * set of temporary code files and puts it into the list of code files of the project.
     *
     * \param document is the temporary code file
     * \param file is the local file URL to where the code file shall be saved
     */
    void saveCodeFileNew(KTextEditor::Document *document, const QUrl &file);

    /**
     * Remove code file \p document from list of temporary code files of the project.
     *
     * \param document is the temporary document to be removed
     */
    void removeCodeFileNew(KTextEditor::Document *document);

    /**
     * Remove code file from the project. File is specified by its identifier in internal list.
     *
     * \param fileID is key for QMap that organizes the files
     */
    void removeCodeFile(int fileID);

    /**
     * List of all graph documents of the project.
     * \return list of local file URLs
     */
    QList<GraphTheory::GraphDocumentPtr> graphDocuments() const;

    GraphTheory::GraphDocumentPtr activeGraphDocument() const;

    /**
     * Add new graph document to the project.
     * \return the created graph document
     */
    GraphTheory::GraphDocumentPtr createGraphDocument();

    /**
     * Save a temporary graph document to a graph file and update association to the project.
     * This method calls save method of \p document, saves it to \p file, removes file from
     * set of temporary graph files and puts it into the list of graph files of the project.
     *
     * \param document is the temporary graph file
     * \param file is the file url to where the graph file shall be saved
     */
    void saveGraphFileNew(GraphTheory::GraphDocumentPtr document, const QUrl &fileUrl);

    /**
     * Save existing graph document file under new filename
     */
    void saveGraphFileAs(GraphTheory::GraphDocumentPtr document, const QUrl &fileUrl);

    /**
     * Remove graph file \p document from the project.
     *
     * \param document is the temporary document to be removed
     */
    void removeGraphDocument(GraphTheory::GraphDocumentPtr document);

    /**
     * Set journal file of the project.
     *
     * \param file is a local file url pointing to the journal file
     */
    void setJournalFile(const QUrl &file);

    /**
     * Returns URL to local file containing the project journal.
     *
     * \return URL to local file
     */
    QUrl journalFile() const;

    /**
     * Save the project to its current file if empty or no filename is given. Otherwise if filename is
     * given, save the project in file \p fileUrl. If the project is temporary, i.e., isTemporary() returns
     * true, a file URL is mandatory.
     *
     * \param fileUrl is the target file, expected as absolute path
     * \return true if save was successful, otherwise false
     */
    bool writeProjectFile(const QString& fileUrl = QString());

    /**
     * Exports the project with all its components (scripts, graph, journal) to an archive file.
     *
     * \param exportUrl the path to the newly created archive
     * \return true if archive was created, else false
     */
    bool exportProject(const QUrl &exportUrl);

    static QString relativePath(const QString &base_dir, const QString &path);

Q_SIGNALS:
    void activeGraphDocumentChanged();

private:
    bool writeNewProjectFile();
    const QScopedPointer<ProjectPrivate> d;
};

#endif
