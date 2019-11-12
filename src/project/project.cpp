/*
 *  Copyright 2012-2015  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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
 *  License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "project.h"
#include "libgraphtheory/typenames.h"
#include "libgraphtheory/graphdocument.h"
#include "libgraphtheory/editor.h"
#include <KConfig>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KMessageBox>
#include <KTextEditor/Document>
#include <KTextEditor/Editor>
#include <KTar>
#include <QUrl>
#include <QDir>
#include <QHash>
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
        : m_journal(Q_NULLPTR)
        , m_graphEditor(Q_NULLPTR)
        , m_modified(false)
        , m_activeGraphDocumentIndex(-1)
        , m_activeCodeDocumentIndex(-1)
    {

    }

    QUrl m_projectUrl; //!< the project's archive file
    QTemporaryDir m_workingDirectory; //!< temporary directory where all project files are organized
    QList<KTextEditor::Document*> m_codeDocuments;
    QList<GraphDocumentPtr> m_graphDocuments;
    QHash<KTextEditor::Document*,QString> m_documentNames;
    KTextEditor::Document *m_journal;
    GraphTheory::Editor *m_graphEditor;
    bool m_modified;

    int m_activeGraphDocumentIndex;
    int m_activeCodeDocumentIndex;

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

    QJsonArray codeDocs = metaInfo["scripts"].toArray();
    QJsonArray codeDocNames = metaInfo["scriptNames"].toArray();
    for (int index = 0; index < codeDocs.count(); ++index) {
        QJsonObject docInfo = codeDocs.at(index).toObject();
        QString filename = docInfo["file"].toString();
        KTextEditor::Document *document = KTextEditor::Editor::instance()->createDocument(nullptr);
        document->openUrl(QUrl::fromLocalFile(m_workingDirectory.path() + QChar('/') + filename));
        m_codeDocuments.append(document);
        m_documentNames.insert(document, codeDocNames.at(index).toString());
    }

    QJsonArray graphDocs = metaInfo["graphs"].toArray();
    for (int index = 0; index < graphDocs.count(); ++index) {
        QJsonObject docInfo = graphDocs.at(index).toObject();
        QString fileName = docInfo["file"].toString();
        QUrl fileUrl = QUrl::fromLocalFile(m_workingDirectory.path() + QChar('/') + fileName);
        GraphDocumentPtr document = m_graphEditor->openDocument(fileUrl);
        // add only to project, if document was loaded correctly
        if (document) {
            document->setDocumentName(docInfo["name"].toString());
            m_graphDocuments.append(document);
        }
    }
    m_journal = KTextEditor::Editor::instance()->createDocument(nullptr);
    m_journal->openUrl(QUrl::fromLocalFile(m_workingDirectory.path() + QChar('/') + metaInfo["journal.txt"].toString()));
    Q_ASSERT(m_journal != nullptr);

    //TODO save & load open document index

    return true;
}

bool ProjectPrivate::writeProjectMetaInfo()
{
    QJsonObject metaInfo;

    QJsonArray codeDocs, codeDocNames, graphDocs;
    foreach (KTextEditor::Document *document,  m_codeDocuments) {
        QJsonObject docInfo;
        docInfo.insert("file", document->url().fileName());
        codeDocs.append(docInfo);
        codeDocNames.append(m_documentNames.value(document));
    }
    foreach (GraphTheory::GraphDocumentPtr document,  m_graphDocuments) {
        QJsonObject docInfo;
        docInfo.insert("file", document->documentUrl().fileName());
        docInfo.insert("name", document->documentName());
        graphDocs.append(docInfo);
    }
    metaInfo.insert("scripts", codeDocs);
    metaInfo.insert("scriptNames", codeDocNames);
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
    d->m_journal = KTextEditor::Editor::instance()->createDocument(nullptr);
    d->m_journal->saveAs(QUrl::fromLocalFile(workingDir() + QChar('/') + QString("journal.txt")));
}

//TODO make graphEditor singleton
Project::Project(const QUrl &projectFile, GraphTheory::Editor *graphEditor)
    : d(new ProjectPrivate)
{
    d->m_graphEditor = graphEditor;
    d->m_projectUrl = projectFile;
    if (!d->loadProject(projectFile)) {
        addGraphDocument(graphEditor->createDocument());
        setModified(false);
        d->m_journal = KTextEditor::Editor::instance()->createDocument(nullptr);

        KMessageBox::error(nullptr,
                           i18nc("@info",
                                 "The Rocs project could not be imported because the project file could not be parsed."));
    }

    for (const auto &document : d->m_codeDocuments) {
        connect(document, &KTextEditor::Document::modifiedChanged,
            this, &Project::modifiedChanged);
    }
    for (const auto &document : d->m_graphDocuments) {
        connect(document.data(), &GraphDocument::modifiedChanged,
            this, &Project::modifiedChanged);
    }
}

Project::~Project()
{
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

KTextEditor::Document* Project::createCodeDocument(const QString& filePath)
{
    auto path = d->m_workingDirectory.path() + QLatin1Char('/') + filePath + QStringLiteral(".js");

    auto doc = KTextEditor::Editor::instance()->createDocument(nullptr);
    if (!doc->saveAs(QUrl::fromLocalFile(path))) {
        qCritical() << "Error when saving code file to working directory, aborting.";
        return nullptr;
    }

    if (!addCodeDocument(doc)) {
        return nullptr;
    }

    return doc;
}

KTextEditor::Document* Project::openCodeDocument(const QUrl &url)
{
    auto doc = KTextEditor::Editor::instance()->createDocument(nullptr);
    if (!doc->openUrl(url)) {
        qCritical() << "Error when opening code file, aborting.";
        return nullptr;
    }

    if (!addCodeDocument(doc)) {
        return nullptr;
    }

    return doc;
}

bool Project::addCodeDocument(KTextEditor::Document *document)
{
    if (document == nullptr) {
        return false;
    }

    emit codeDocumentAboutToBeAdded(document, d->m_codeDocuments.count());
    connect(document, &KTextEditor::Document::modifiedChanged,
        this, &Project::modifiedChanged);
    d->m_codeDocuments.append(document);
    emit codeDocumentAdded();
    setModified(true);
    return true;
}

KTextEditor::Document * Project::importCodeDocument(const QUrl &url)
{
    return openCodeDocument(url);
}

void Project::tryToRemoveCodeDocument(KTextEditor::Document *document)
{
    QString path = document->url().toString();
    if(!document->closeUrl())
        return;
    int index = d->m_codeDocuments.indexOf(document);
    emit codeDocumentAboutToBeRemoved(index, index);
    disconnect(document, &KTextEditor::Document::modifiedChanged,
        this, &Project::modifiedChanged);
    d->m_codeDocuments.removeAt(index);
    emit codeDocumentRemoved();
    if (!path.startsWith(d->m_workingDirectory.path())) {
        qCritical() << "Aborting removal of document: not in temporary working directory";
        return;
    }
    if (!QFile::remove(path)) {
        qCritical() << "Could not remove code file" << path;
    }
    d->m_documentNames.remove(document);
    setModified(true);
}

QString Project::documentName(KTextEditor::Document *document) const
{
    const QString docName = d->m_documentNames.value(document);
    return !docName.isEmpty()
        ? docName
        : document->documentName();
}

void Project::setDocumentName(KTextEditor::Document *document, const QString &name)
{
    d->m_documentNames.insert(document, name);
    setModified(true);
}

QList<KTextEditor::Document*> Project::codeDocuments() const
{
    return d->m_codeDocuments;
}

void Project::setActiveCodeDocument(int index)
{
    if (index < 0 || index >= d->m_codeDocuments.count()) {
        qCritical() << "Code document index invalid, aborting change of current document.";
        return;
    }
    d->m_activeCodeDocumentIndex = index;
    emit activeCodeDocumentChanged(index);
}

QList<GraphDocumentPtr> Project::graphDocuments() const
{
    return d->m_graphDocuments;
}

bool Project::addGraphDocument(GraphDocumentPtr document)
{
    // compute first unused document path
    QStringList usedFileNames;
    foreach (GraphTheory::GraphDocumentPtr document, d->m_graphDocuments) {
        usedFileNames.append(document->documentUrl().fileName());
    }
    QString fileName;
    for (int i = 0; i <= d->m_graphDocuments.count(); ++i) {
        fileName = "graphfile" + QString::number(i) + QString(".graph2");
        if (!usedFileNames.contains(fileName)) {
            break;
        }
    }
    QString path = d->m_workingDirectory.path()
            + QChar('/')
            + fileName;

    // put document into working directory
    document->documentSaveAs(QUrl::fromLocalFile(path));
    int index = d->m_graphDocuments.length();
    emit graphDocumentAboutToBeAdded(document, index);
    connect(document.data(), &GraphDocument::modifiedChanged,
        this, &Project::modifiedChanged);
    d->m_graphDocuments.append(document);
    emit graphDocumentAdded();
    setModified(true);

    if (d->m_activeGraphDocumentIndex < 0) {
        setActiveGraphDocument(index);
    }
    return true;
}

GraphTheory::GraphDocumentPtr Project::importGraphDocument(const QUrl &documentUrl)
{
    Q_ASSERT(d->m_graphEditor);
    GraphTheory::GraphDocumentPtr document = d->m_graphEditor->openDocument(documentUrl);
    Q_ASSERT(document);
    addGraphDocument(document);
    return document;
}

void Project::removeGraphDocument(GraphDocumentPtr document)
{
    QString path = document->documentUrl().toLocalFile();
    d->m_graphDocuments.removeAll(document);
    if (!path.startsWith(d->m_workingDirectory.path())) {
        qCritical() << "Aborting removal of graph document with path "
            << path
            << ", not in temporary working directory" << d->m_workingDirectory.path();
        return;
    }
    if (!QFile::remove(path)) {
        qCritical() << "Could not remove graph file" << path;
    }
    int index = d->m_graphDocuments.indexOf(document);
    emit graphDocumentAboutToBeRemoved(index, index);
    d->m_graphDocuments.removeAt(index);
    emit graphDocumentRemoved();
    setModified(true);
}

void Project::setActiveGraphDocument(int index)
{
    if (index < 0 || index >= d->m_graphDocuments.count()) {
        qCritical() << "Graph document index invalid, aborting change of current document.";
        return;
    }
    d->m_activeGraphDocumentIndex = index;
    emit activeGraphDocumentChanged(index);
    emit activeGraphDocumentChanged(d->m_graphDocuments.at(index));
}

GraphDocumentPtr Project::activeGraphDocument() const
{
    if (d->m_activeGraphDocumentIndex < 0 || d->m_graphDocuments.count() <= d->m_activeGraphDocumentIndex) {
        return GraphDocumentPtr();
    }
    return d->m_graphDocuments.at(d->m_activeGraphDocumentIndex);
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
        document->save();
        tar.addLocalFile(document->url().toLocalFile(), document->url().fileName());
    }
    foreach (GraphTheory::GraphDocumentPtr document, d->m_graphDocuments) {
        document->documentSave();
        tar.addLocalFile(document->documentUrl().toLocalFile(), document->documentUrl().fileName());
    }
    tar.addLocalFile(d->m_journal->url().toLocalFile(), "journal.txt");
    d->writeProjectMetaInfo();
    tar.addLocalFile(d->m_workingDirectory.path() + QChar('/') + "project.json", "project.json");
    tar.close();

    // update modified state
    setModified(false);

    return true;
}

bool Project::projectSaveAs(const QUrl &url)
{
    d->m_projectUrl = url;
    return projectSave();
}

void Project::setModified(bool modified)
{
    if (modified == d->m_modified) {
        return;
    }
    d->m_modified = modified;
    emit modifiedChanged();
}

bool Project::isModified() const
{
    foreach (GraphDocumentPtr document, d->m_graphDocuments) {
        if (document->isModified()) {
            return true;
        }
    }
    foreach (KTextEditor::Document *document, d->m_codeDocuments) {
        if (document->isModified()) {
            return true;
        }
    }
    if (d->m_journal && d->m_journal->isModified()) {
        return true;
    }

    return d->m_modified;
}
