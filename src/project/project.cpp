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

#include "project.h"
#include "libgraphtheory/typenames.h"
#include "libgraphtheory/graphdocument.h"
#include "libgraphtheory/fileformats/fileformatmanager.h"

#include <KConfig>
#include <KConfigGroup>
#include <KTextEditor/Document>
#include <KTextEditor/Editor>
#include <KTar>
#include <QUrl>
#include <QDir>
#include <QString>
#include <QMap>
#include <QList>
#include <QSaveFile>
#include <QTemporaryFile>
#include <QTemporaryDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

using namespace GraphTheory;

class ProjectPrivate
{
public:
    ProjectPrivate()
        : m_name(QString())
        , m_journal(0)
        , m_graphEditor(0)
        , m_modified(false)
    {

    }

    QString m_name; //!< the project's name
    QUrl m_projectUrl; //!< the project's archive file
    QTemporaryDir m_workingDirectory; //!< temporary directory where all project files are organized
    QList<KTextEditor::Document*> m_codeDocuments;
    QList<GraphDocumentPtr> m_graphDocuments;
    KTextEditor::Document* m_journal;
    GraphTheory::Editor *m_graphEditor;
    GraphTheory::FileFormatManager m_graphFileFormatManager;
    bool m_modified;

    GraphDocumentPtr m_activeGraphDocument;     // TODO needed?

    /**
     * Set project from project archive file.
     */
    bool loadProject(const QUrl &path);

    /**
     * Write project meta info file
     */
    bool writeProjectMetaInfo();
};

bool ProjectPrivate::loadProject(const QUrl &url)
{
    // extract archive file to temporary working directory
    KTar tar = KTar(url.toLocalFile(), QString("application/x-gzip"));
    if (!tar.open(QIODevice::ReadOnly)) {
        qCritical() << "Could not open project archive file for reading, aborting.";
        return false;
    }
    tar.directory()->copyTo(m_workingDirectory.path(), true);
    QFile metaInfoFile(m_workingDirectory.path() + QChar('/') + "project.json");
    if (!metaInfoFile.open(QIODevice::ReadOnly)) {
        qWarning("Could not open project.json file for reading, aborting.");
        return false;
    }
    QJsonDocument metaInfoDoc = QJsonDocument::fromJson(metaInfoFile.readAll());

    // set project
    QJsonObject metaInfo = metaInfoDoc.object();
    m_name = metaInfo["name"].toString();

    QJsonArray codeDocs = metaInfo["scripts"].toArray();
    for (int index = 0; index < codeDocs.count(); ++index) {
        QString filename = codeDocs.at(index).toString();
        KTextEditor::Document *document = KTextEditor::Editor::instance()->createDocument(0);
        document->openUrl(QUrl::fromLocalFile(m_workingDirectory.path() + QChar('/') + filename));
        m_codeDocuments.append(document);
    }

    QJsonArray graphDocs = metaInfo["graphs"].toArray();
    for (int index = 0; index < graphDocs.count(); ++index) {
        QString filename = graphDocs.at(index).toString();
//         document->documentOpen(); //TODO <- implement this API to easier load documents

        GraphTheory::FileFormatInterface *importer = m_graphFileFormatManager.backendByExtension("graph");
        importer->setFile(QUrl::fromLocalFile(m_workingDirectory.path() + QChar('/') + filename));
        importer->readFile();
        if (importer->hasError()) {
            qCritical() << "Graph file importer reported the following error, aborting.";
            importer->errorString();
            return GraphDocumentPtr();
        }
        GraphTheory::GraphDocumentPtr document = importer->graphDocument();
        m_graphDocuments.append(document);
    }
    m_journal = KTextEditor::Editor::instance()->createDocument(0);
    m_journal->openUrl(QUrl::fromLocalFile(m_workingDirectory.path() + QChar('/') + metaInfo["journal.txt"].toString()));

    return true;
}

bool ProjectPrivate::writeProjectMetaInfo()
{
    QJsonObject metaInfo;
    metaInfo.insert("name", m_name);

    QJsonArray codeDocs, graphDocs;
    foreach (KTextEditor::Document *document,  m_codeDocuments) {
        codeDocs.append(document->url().fileName());
    }
    foreach (GraphTheory::GraphDocumentPtr document,  m_graphDocuments) {
        graphDocs.append(document->documentUrl().fileName());
    }
    metaInfo.insert("scripts", codeDocs);
    metaInfo.insert("graphs", graphDocs);
    metaInfo.insert("journal", m_journal->url().fileName());

    // write to file
    QFile metaInfoFile(m_workingDirectory.path() + QChar('/') + "project.json");
    if (!metaInfoFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open project.json file for writing, abort.");
        return false;
    }
    QJsonDocument metaInfoDoc(metaInfo);
    metaInfoFile.write(metaInfoDoc.toJson());

    return true;
}


//TODO make graphEditor singleton
Project::Project(GraphTheory::Editor *graphEditor)
    : d(new ProjectPrivate)
{
    d->m_graphEditor = graphEditor;
    d->m_journal = KTextEditor::Editor::instance()->createDocument(0);
    d->m_journal->saveAs(QUrl::fromLocalFile(workingDir() + QChar('/') + QString("journal.txt")));
}

//TODO make graphEditor singleton
Project::Project(const QUrl &projectFile, GraphTheory::Editor *graphEditor)
    : d(new ProjectPrivate)
{
    d->m_graphEditor = graphEditor;
    d->m_projectUrl = projectFile;
    d->loadProject(projectFile);
}

Project::~Project()
{
}

void Project::setName(const QString &name)
{
    if (d->m_name == name) {
        return;
    }
    d->m_name = name;
    emit nameChanged(name);
    d->m_modified = true;
}

QString Project::name() const
{
    return d->m_name;
}

QUrl Project::projectUrl() const
{
    return d->m_projectUrl;
}

void Project::setProjectUrl(const QUrl &url)
{
    d->m_projectUrl = url;
}

QString Project::workingDir() const
{
    return d->m_workingDirectory.path();
}

bool Project::addCodeDocument(KTextEditor::Document *document)
{
    // compute first unused document path
    QStringList usedPaths;
    foreach (KTextEditor::Document *document, d->m_codeDocuments) {
        usedPaths.append(document->url().toLocalFile());
    }
    QString path;
    for (int i = 0; i <= d->m_codeDocuments.count(); ++i) {
        path = d->m_workingDirectory.path()
            + QChar('/')
            + "codefile" + QString::number(i) + QString(".js");
        if (!usedPaths.contains(path)) {
            break;
        }
    }

    // put document into working directory
    if (!document->saveAs(QUrl::fromLocalFile(path))) {
        qCritical() << "Error when saving code file to working directory, aborting.";
        return false;
    }

    d->m_codeDocuments.append(document);
    d->m_modified = true;
    return true;
}

KTextEditor::Document * Project::importCodeDocument(const QUrl &url)
{
    KTextEditor::Document *document = KTextEditor::Editor::instance()->createDocument(0);
    document->openUrl(url);
    addCodeDocument(document);
    return document;
}

void Project::removeCodeDocument(KTextEditor::Document *document)
{
    QString path = document->url().toString();
    document->closeUrl();
    d->m_codeDocuments.removeAll(document);
    if (!path.startsWith(d->m_workingDirectory.path())) {
        qCritical() << "Aborting removal of document: not in temporary working directory";
        return;
    }
    if (!QFile::remove(path)) {
        qCritical() << "Could not remove code file" << path;
    }
    d->m_modified = true;
}

QList<KTextEditor::Document*> Project::codeDocuments() const
{
    return d->m_codeDocuments;
}

QList<GraphDocumentPtr> Project::graphDocuments() const
{
    return d->m_graphDocuments;
}

GraphDocumentPtr Project::activeGraphDocument() const
{
    return d->m_activeGraphDocument;
}

bool Project::addGraphDocument(GraphDocumentPtr document)
{
    // compute first unused document path
    QStringList usedPaths;
    foreach (GraphTheory::GraphDocumentPtr document, d->m_graphDocuments) {
        usedPaths.append(document->documentUrl().fileName());
    }
    QString path;
    for (int i = 0; i <= d->m_graphDocuments.count(); ++i) {
        path = d->m_workingDirectory.path()
            + QChar('/')
            + "graphfile" + QString::number(i) + QString(".graph");
        if (!usedPaths.contains(path)) {
            break;
        }
    }

    // put document into working directory
    document->documentSaveAs(QUrl::fromLocalFile(path));
    d->m_graphDocuments.append(document);
    d->m_modified = true;

    if (d->m_activeGraphDocument.isNull()) {
        d->m_activeGraphDocument = document;
        emit activeGraphDocumentChanged();
    }

    return true;
}

GraphTheory::GraphDocumentPtr Project::importGraphDocument(const QUrl &url)
{
    Q_ASSERT(d->m_graphEditor);
    GraphTheory::FileFormatInterface *importer = d->m_graphFileFormatManager.backendByExtension(url.fileName());
    if (!importer) {
        qCritical() << "No file format backend found to import file" << url.fileName();
        return GraphDocumentPtr();
    }
    importer->setFile(url);
    importer->readFile();
    if (importer->hasError()) {
        qCritical() << "Graph file importer reported the following error, aborting.";
        importer->errorString();
        return GraphDocumentPtr();
    }
    GraphTheory::GraphDocumentPtr document = importer->graphDocument();
    addGraphDocument(document);
    return document;
}

void Project::removeGraphDocument(GraphDocumentPtr document)
{
    QString path = document->documentUrl().toLocalFile();
    d->m_graphDocuments.removeAll(document);
    if (!path.startsWith(d->m_workingDirectory.path())) {
        qCritical() << "Aborting removal of document: not in temporary working directory";
        return;
    }
    if (!QFile::remove(path)) {
        qCritical() << "Could not remove graph file" << path;
    }

    d->m_graphDocuments.removeAll(document);
    d->m_modified = true;
}

KTextEditor::Document * Project::journalDocument() const
{
    return d->m_journal;
}

bool Project::projectSave()
{
    if (d->m_projectUrl.isEmpty()) {
        qCritical() << "No project file specified, abort saving.";
        return false;
    }
    KTar tar = KTar(d->m_projectUrl.toLocalFile(), QString("application/x-gzip"));
    tar.open(QIODevice::WriteOnly);

    foreach (KTextEditor::Document *document, d->m_codeDocuments) {
        tar.addLocalFile(document->url().toLocalFile(), document->url().fileName());
    }
    foreach (GraphTheory::GraphDocumentPtr document, d->m_graphDocuments) {
        tar.addLocalFile(document->documentUrl().toLocalFile(), document->documentUrl().fileName());
    }
    tar.addLocalFile(d->m_journal->url().toLocalFile(), "journal.txt");
    d->writeProjectMetaInfo();
    tar.addLocalFile(d->m_workingDirectory.path() + QChar('/') + "project.json", "project.json");
    tar.close();
    d->m_modified = false;

    return true;
}

bool Project::projectSaveAs(const QUrl &url)
{
    d->m_projectUrl = url;
    return projectSave();
}

bool Project::isModified() const
{
    return d->m_modified;
}
