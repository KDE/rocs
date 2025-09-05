/*
 *  SPDX-FileCopyrightText: 2012-2015 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "project.h"
#include "libgraphtheory/graphdocument.h"
#include <KConfig>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KMessageBox>
#include <KTar>
#include <KTextEditor/Document>
#include <KTextEditor/Editor>
#include <QDebug>
#include <QDir>
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QSaveFile>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QUrl>

using namespace GraphTheory;

class ProjectPrivate
{
public:
    QUrl m_projectUrl; //!< the project's archive file
    QTemporaryDir m_workingDirectory; //!< temporary directory where all project files are organized
    QList<KTextEditor::Document *> m_codeDocuments;
    QList<GraphDocumentPtr> m_graphDocuments;
    QHash<KTextEditor::Document *, QString> m_documentNames;
    KTextEditor::Document *m_journal{nullptr};
    GraphTheory::Editor *m_graphEditor{nullptr};
    bool m_modified{false};
    bool m_valid{false};

    int m_activeGraphDocumentIndex{-1};
    int m_activeCodeDocumentIndex{-1};

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
    KTar tar = KTar(url.toLocalFile(), QStringLiteral("application/gzip"));
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

    // TODO save & load open document index

    return true;
}

bool ProjectPrivate::writeProjectMetaInfo()
{
    QJsonObject metaInfo;

    QJsonArray codeDocs, codeDocNames, graphDocs;
    for (KTextEditor::Document *document : std::as_const(m_codeDocuments)) {
        QJsonObject docInfo;
        docInfo.insert("file", document->url().fileName());
        codeDocs.append(docInfo);
        codeDocNames.append(m_documentNames.value(document));
    }
    for (const GraphTheory::GraphDocumentPtr &document : std::as_const(m_graphDocuments)) {
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

Project::Project(GraphTheory::Editor *graphEditor)
    : d(new ProjectPrivate)
{
    d->m_graphEditor = graphEditor;
    d->m_journal = KTextEditor::Editor::instance()->createDocument(nullptr);
    d->m_journal->saveAs(QUrl::fromLocalFile(workingDir() + QChar('/') + QString("journal.txt")));
}

Project::Project(const QUrl &projectFile, GraphTheory::Editor *graphEditor)
    : d(new ProjectPrivate)
{
    d->m_graphEditor = graphEditor;
    d->m_projectUrl = projectFile;
    if (d->loadProject(projectFile)) {
        for (const auto &document : std::as_const(d->m_codeDocuments)) {
            connect(document, &KTextEditor::Document::modifiedChanged, this, &Project::modifiedChanged);
        }
        for (const auto &document : std::as_const(d->m_graphDocuments)) {
            connect(document.data(), &GraphDocument::modifiedChanged, this, &Project::modifiedChanged);
        }
        d->m_valid = true;
    } else {
        qCritical() << "Could not parse project, omitting further loading";
        d->m_valid = false;
    }

    if (isValid() && d->m_graphDocuments.size() == 0) {
        addGraphDocument(graphEditor->createDocument());
    }
    d->m_journal = KTextEditor::Editor::instance()->createDocument(nullptr);
    setModified(false);
}

Project::~Project() = default;

bool Project::isValid() const
{
    return d->m_valid;
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

KTextEditor::Document *Project::createCodeDocument(const QString &filePath)
{
    const QString path = d->m_workingDirectory.path() + QLatin1Char('/') + filePath + QStringLiteral(".js");

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

KTextEditor::Document *Project::openCodeDocument(const QUrl &url)
{
    auto doc = KTextEditor::Editor::instance()->createDocument(this);
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

    Q_EMIT codeDocumentAboutToBeAdded(document, d->m_codeDocuments.count());
    connect(document, &KTextEditor::Document::modifiedChanged, this, &Project::modifiedChanged);
    d->m_codeDocuments.append(document);
    Q_EMIT codeDocumentAdded();
    setModified(true);
    return true;
}

KTextEditor::Document *Project::importCodeDocument(const QUrl &url)
{
    const QString importedUrl = workingDir() + '/' + url.fileName();
    QFile::copy(url.toLocalFile(), importedUrl);
    return openCodeDocument(QUrl::fromLocalFile(importedUrl));
}

void Project::tryToRemoveCodeDocument(KTextEditor::Document *document)
{
    QString path = document->url().toString();
    if (!document->closeUrl())
        return;
    int index = d->m_codeDocuments.indexOf(document);
    Q_EMIT codeDocumentAboutToBeRemoved(index, index);
    disconnect(document, &KTextEditor::Document::modifiedChanged, this, &Project::modifiedChanged);
    d->m_codeDocuments.removeAt(index);
    Q_EMIT codeDocumentRemoved();
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
    return !docName.isEmpty() ? docName : document->documentName();
}

void Project::setDocumentName(KTextEditor::Document *document, const QString &name)
{
    d->m_documentNames.insert(document, name);
    setModified(true);
}

QList<KTextEditor::Document *> Project::codeDocuments() const
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
    Q_EMIT activeCodeDocumentChanged(index);
}

QList<GraphDocumentPtr> Project::graphDocuments() const
{
    return d->m_graphDocuments;
}

bool Project::addGraphDocument(GraphDocumentPtr document)
{
    // compute first unused document path
    QStringList usedFileNames;
    for (const GraphTheory::GraphDocumentPtr &document : std::as_const(d->m_graphDocuments)) {
        usedFileNames.append(document->documentUrl().fileName());
    }
    QString fileName;
    for (int i = 0; i <= d->m_graphDocuments.count(); ++i) {
        fileName = "graphfile" + QString::number(i) + QString(".graph2");
        if (!usedFileNames.contains(fileName)) {
            break;
        }
    }
    QString path = d->m_workingDirectory.path() + QChar('/') + fileName;

    // put document into working directory
    document->documentSaveAs(QUrl::fromLocalFile(path));
    int index = d->m_graphDocuments.length();
    Q_EMIT graphDocumentAboutToBeAdded(document, index);
    connect(document.data(), &GraphDocument::modifiedChanged, this, &Project::modifiedChanged);
    d->m_graphDocuments.append(document);
    Q_EMIT graphDocumentAdded();
    setModified(true);

    if (d->m_activeGraphDocumentIndex < 0) {
        setActiveGraphDocument(index);
    }
    return true;
}

GraphTheory::GraphDocumentPtr Project::importGraphDocument(const QUrl &url)
{
    Q_ASSERT(d->m_graphEditor);
    const QString importedUrl = workingDir() + '/' + url.fileName();
    QFile::copy(url.toLocalFile(), importedUrl);

    GraphTheory::GraphDocumentPtr document = d->m_graphEditor->openDocument(QUrl::fromLocalFile(importedUrl));
    Q_ASSERT(document);
    addGraphDocument(document);
    return document;
}

void Project::removeGraphDocument(GraphDocumentPtr document)
{
    QString path = document->documentUrl().toLocalFile();
    if (!path.startsWith(d->m_workingDirectory.path())) {
        qCritical() << "Aborting removal of graph document with path " << path << ", not in temporary working directory" << d->m_workingDirectory.path();
        return;
    }
    if (!QFile::remove(path)) {
        qWarning() << "Could not remove graph file, but cleanup up project anyways" << path;
    }
    int index = d->m_graphDocuments.indexOf(document);
    if (index >= 0) {
        Q_EMIT graphDocumentAboutToBeRemoved(index, index);
        d->m_graphDocuments.removeAt(index);
        Q_EMIT graphDocumentRemoved();
        setModified(true);
    } else {
        qCritical() << "Could not find file index" << path;
    }
}

void Project::setActiveGraphDocument(int index)
{
    if (index < 0 || index >= d->m_graphDocuments.count()) {
        qCritical() << "Graph document index invalid, aborting change of current document.";
        return;
    }
    d->m_activeGraphDocumentIndex = index;
    Q_EMIT activeGraphDocumentChanged(index);
    Q_EMIT activeGraphDocumentChanged(d->m_graphDocuments.at(index));
}

GraphDocumentPtr Project::activeGraphDocument() const
{
    if (d->m_activeGraphDocumentIndex < 0 || d->m_graphDocuments.count() <= d->m_activeGraphDocumentIndex) {
        return GraphDocumentPtr();
    }
    return d->m_graphDocuments.at(d->m_activeGraphDocumentIndex);
}

KTextEditor::Document *Project::journalDocument() const
{
    return d->m_journal;
}

bool Project::projectSave()
{
    if (d->m_projectUrl.isEmpty()) {
        qCritical() << "No project file specified, abort saving.";
        return false;
    }
    KTar tar = KTar(d->m_projectUrl.toLocalFile(), QStringLiteral("application/gzip"));
    tar.open(QIODevice::WriteOnly);

    for (KTextEditor::Document *document : std::as_const(d->m_codeDocuments)) {
        document->save();
        tar.addLocalFile(document->url().toLocalFile(), document->url().fileName());
    }
    for (const GraphTheory::GraphDocumentPtr &document : std::as_const(d->m_graphDocuments)) {
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
    Q_EMIT modifiedChanged();
}

bool Project::isModified() const
{
    for (const GraphDocumentPtr &document : std::as_const(d->m_graphDocuments)) {
        if (document->isModified()) {
            return true;
        }
    }
    for (KTextEditor::Document *document : std::as_const(d->m_codeDocuments)) {
        if (document->isModified()) {
            return true;
        }
    }
    if (d->m_journal && d->m_journal->isModified()) {
        return true;
    }

    return d->m_modified;
}

#include "moc_project.cpp"
