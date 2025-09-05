/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef PROJECT_H
#define PROJECT_H

#include "libgraphtheory/editor.h"
#include "libgraphtheory/typenames.h"
#include <QList>
#include <QScopedPointer>
#include <QString>

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

    ~Project() override;

    /**
     * @return the project's journal document
     * Note that the pointer may be 0.
     */
    KTextEditor::Document *journalDocument() const;

    /*
     * Handling of script documents.
     */
public:
    /**
     * @return list of all scripts contained in this project
     */
    QList<KTextEditor::Document *> codeDocuments() const;

    /**
     * Creates a code document based on a given file path
     */
    KTextEditor::Document *createCodeDocument(const QString &filePath);

    /**
     * Returns a document of a given url (path)
     */
    KTextEditor::Document *openCodeDocument(const QUrl &url);

    /**
     * Add code document @p document to project. This updates the document's url.
     */
    bool addCodeDocument(KTextEditor::Document *document);

    /**
     * Import the script given in file @p url to the project. This creates a copy
     * of the original document.
     */
    KTextEditor::Document *importCodeDocument(const QUrl &url);

    /**
     * Remove the script file @p document from the project
     */
    void tryToRemoveCodeDocument(KTextEditor::Document *document);

    /**
     * @return project dependent document name
     */
    QString documentName(KTextEditor::Document *document) const;

    /**
     * Set a project dependent document name @p name for @p document , i.e., the name is only
     * provided by the project but not by the text document file.
     */
    void setDocumentName(KTextEditor::Document *document, const QString &name);

    /**
     * @return false if the project could not be initialized e.g. due to a file parsing error
     */
    bool isValid() const;

public Q_SLOTS:
    /**
     * Set the currently active graph document index to @p index.
     * If the index does not exist, it will not be changed.
     */
    void setActiveCodeDocument(int index);

Q_SIGNALS:
    void codeDocumentAboutToBeAdded(KTextEditor::Document *, int);
    void codeDocumentAdded();
    void codeDocumentAboutToBeRemoved(int, int);
    void codeDocumentRemoved();
    void activeCodeDocumentChanged(int index);
    void modifiedChanged();

    /*
     * Handling of graph documents.
     */
public:
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
     * Import the graph document given in file @p documentUrl to the project. This creates a copy
     * of the original graph document.
     */
    GraphTheory::GraphDocumentPtr importGraphDocument(const QUrl &documentUrl);

    /**
     * Remove the graph file @p document from the project
     */
    void removeGraphDocument(GraphTheory::GraphDocumentPtr document);

public Q_SLOTS:
    /**
     * Set the currently active graph document index to @p index.
     * If the index does not exist, it will not be changed.
     */
    void setActiveGraphDocument(int index);

Q_SIGNALS:
    void graphDocumentAboutToBeAdded(GraphTheory::GraphDocumentPtr, int);
    void graphDocumentAdded();
    void graphDocumentAboutToBeRemoved(int, int);
    void graphDocumentRemoved();
    void activeGraphDocumentChanged(int index);
    void activeGraphDocumentChanged(GraphTheory::GraphDocumentPtr document);

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

private:
    const QScopedPointer<ProjectPrivate> d;
};

#endif
