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
    class Editor;
}

/**
 * \class Project
 * \brief A project represents the compilation of graphs, scripts and further meta information.
 *
 * Topics:
 *  - \ref project_intro
 *  - \ref project_usage
 *
 * \section project_intro Introduction
 *
 * A project consists of a zipped archive file that contains all of its files and, when opened,
 * a working directory that contains (temporary) copies of all of these files. Only on writing back,
 * the archive file gets updated.
 *
 * \section project_usage Using Projects
 *
 * A project can be created by creating an empty project or by using the overloaded constructor
 * to open an existing project file.
 */
class Project : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructs an empty project.
     */
    explicit Project(GraphTheory::Editor *graphEditor);

    /**
     * Opens the project @p url as well as all contained files.chive
     */
    explicit Project(const QUrl &url, GraphTheory::Editor *graphEditor);

    virtual ~Project();

    /**
     * Set the project name to @p name.
     */
    void setName(const QString &name);

    /**
     * @return name of the project
     */
    QString name() const;

    /**
     * @return list of all scripts contained in this project
     */
    QList<KTextEditor::Document*> codeDocuments() const;

    /**
     * Add code document @p document to project. This updates the document's url.
     */
    bool addCodeDocument(KTextEditor::Document *document);

    /**
     * Import the script given in file @p url to the project. This creates a copy
     * of the original document.
     */
    KTextEditor::Document * importCodeDocument(const QUrl &url);

    /**
     * Remove the script file @p document from the project
     */
    void removeCodeDocument(KTextEditor::Document *document);

    /**
     * @return list of all graph documents contained in this project
     */
    QList<GraphTheory::GraphDocumentPtr> graphDocuments() const;

    GraphTheory::GraphDocumentPtr activeGraphDocument() const;

    /**
     * Add the graph document @p document to project. This updates the document's url.
     */
    bool addGraphDocument(GraphTheory::GraphDocumentPtr document);

    /**
     * Import the graph document given in file @p url to the project. This creates a copy
     * of the original graph document.
     */
    GraphTheory::GraphDocumentPtr importGraphDocument(const QUrl &url);

    /**
     * Remove the graph file @p document from the project
     */
    void removeGraphDocument(GraphTheory::GraphDocumentPtr document);

    /**
     * @return the project's journal document
     * Note that the pointer may be 0.
     */
    KTextEditor::Document * journalDocument() const;

Q_SIGNALS:
    void nameChanged(const QString &name);

    void codeDocumentAboutToBeAdded(KTextEditor::Document*,int);
    void codeDocumentAdded();
    void codeDocumentAboutToBeRemoved(int,int);
    void codeDocumentRemoved();

  /*
   * General file related actions.
   * None of these actions cause user interaction.
   */
public:
    /**
     * Save project to path as given by projectUrl().
     * @return @e true on success, i.e. the save has been done, otherwise
     *         @e false
     */
    bool projectSave();

    /**
     * Save project to path @p url. This also changes the projectUrl() path.
     * @return @e true on success, i.e. the save has been done, otherwise
     *         @e false
     */
    bool projectSaveAs(const QUrl &url);

    /**
     * @return project file path
     */
    QUrl projectUrl() const;

    /**
     * Set the project file to @p url. This is the path where the project is saved.
     */
    void setProjectUrl(const QUrl &url);

    /**
     * @internal
     * @return working directory that contains the extraced project archive while open
     */
    QString workingDir() const;

    /**
     * @internal
     * set modified state of project container to @c modified
     * @note this does not change the modified state of any associated document
     */
    void setModified(bool modified = true);

    /**
     * @return @e true if project (including any file of this project) was modified after last saving/loading, otherwise
     *         @e false
     */
    bool isModified() const;

Q_SIGNALS:
    void activeGraphDocumentChanged();

private:
    bool writeNewProjectFile();
    const QScopedPointer<ProjectPrivate> d;
};

#endif
