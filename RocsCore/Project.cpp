/*
    This file is part of Rocs.
    Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>

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

#include "Project.h"
#include "libgraphtheory/typenames.h"
// #include "libgraphtheory/grapheditor.h"
#include "libgraphtheory/graphdocument.h"

#include <QString>
#include <QMap>
#include <QList>

#include <QUrl>
#include <QDir>
#include <KConfig>
#include <KConfigGroup>
#include <QTemporaryFile>
#include <QDebug>
#include <KTextEditor/Document>
#include <KTar>

using namespace GraphTheory;

class ProjectPrivate
{
public:
    ProjectPrivate()
        : m_graphEditor(0)
    {

    }

    QUrl _projectFile;
    GraphTheory::Editor *m_graphEditor;
    QMap<int, QString> _codeFileGroup;
    QList<GraphDocumentPtr> _graphFileNew;
    QList<KTextEditor::Document*> _codeFileNew;
    KConfig* _config;
    bool _temporary;
    bool _modified;
    QList<GraphDocumentPtr> m_graphDocuments;
    GraphDocumentPtr m_activeGraphDocument;

    KConfigGroup initKConfigObject() {
        // helper method for Project::open()
        qDebug() << "Creating KConfig object temporary project file: " << _projectFile.toLocalFile();
        _config = new KConfig(_projectFile.toLocalFile());

        KConfigGroup projectGroup(_config, "Project");
        QStringList codeFileIDs = projectGroup.readEntry("CodeFiles", QStringList());
        foreach(const QString& offset, codeFileIDs) {
            _codeFileGroup.insert(offset.toInt(), "CodeFile" + offset);
        }

        QStringList graphFileIDs = projectGroup.readEntry("GraphFiles", QStringList());
        foreach(const QString& offset, graphFileIDs) {
            KConfigGroup group(_config, "GraphFile" + offset);
            QString file = group.readEntry("file");
            QUrl path;
            if (QUrl::fromLocalFile(file).isRelative()) {
                path = QUrl::fromLocalFile(projectDirectory() + group.readEntry("file"));
            } else {
                path = QUrl::fromLocalFile(group.readEntry("file"));
            }
            GraphDocumentPtr document;
            if (m_graphEditor) {
                document = m_graphEditor->createDocument();
            } else {
                document = GraphDocument::create();
            }
            document->setDocumentUrl(path);
            m_graphDocuments.append(document);
        }
        if (m_graphDocuments.count() > 0) {
            m_activeGraphDocument = m_graphDocuments.first();
        }

        // for now, journal files only have name "journal.html"
        KConfigGroup journalGroup(_config, "Journal");
        journalGroup.writeEntry("JournalHtml", "journal.html");

        return projectGroup;
    }

    QString projectDirectory() const
    {
        if (_temporary == true) {
            return "";
        }
        QString directory = _projectFile.path();
        int i = directory.lastIndexOf(QLatin1Char('/'));
        if (i == -1) {
            return QString();
        }
        if (i == 0) {
            return QString(QLatin1Char('/'));
        }
    #ifdef Q_OS_WIN
        if (i == 2 && result[1] == QLatin1Char(':')) {
            return result.left(3);
        }
    #endif
        // append trailing slash
        directory = directory.left(i + 1);
        return directory;
    }
};

Project::Project()
    : d(new ProjectPrivate)
{
    QTemporaryFile tmpProjectFile;
    tmpProjectFile.setAutoRemove(false);
    tmpProjectFile.open();
    d->_projectFile = QUrl::fromLocalFile(tmpProjectFile.fileName());

    d->initKConfigObject();
    d->_temporary = true;
    d->_modified = false;
}

Project::Project(const QUrl &projectFile) :
    d(new ProjectPrivate)
{
    d->_projectFile = projectFile;

    d->initKConfigObject();
    if (!d->_config->isConfigWritable(true)) {
        d->_temporary = true;
    } else {
        d->_temporary = false;
    }
    d->_modified = false;
}

Project::Project(const QUrl &projectArchive, const QUrl &projectDirectory)
    : d(new ProjectPrivate)
{
    // extract archive into project directory
    KTar tar(projectArchive.toLocalFile());
    if (!tar.open(QIODevice::ReadOnly)) {
        qCritical() << "Could not open export archive to read.";
        return;
    }
    tar.directory()->copyTo(projectDirectory.path(), true);
    QUrl projectFile = projectDirectory.resolved(QUrl("project.rocs"));

    d->_projectFile = projectFile;
    d->initKConfigObject();
    if (!d->_config->isConfigWritable(true)) {
        d->_temporary = true;
    } else {
        d->_temporary = false;
    }
    d->_projectFile = projectArchive;
    d->_modified = false;
}

Project::~Project()
{
}

void Project::setName(const QString &name)
{
    KConfigGroup projectGroup(d->_config, "Project");
    projectGroup.writeEntry("Name", name);
    d->_modified = true;
}

QString Project::name() const
{
    KConfigGroup projectGroup(d->_config, "Project");
    return projectGroup.readEntry("Name", QString());
}

QString Project::projectDirectory() const
{
    return d->projectDirectory();
}

QUrl Project::projectFile() const
{
    return d->_projectFile;
}

void Project::setProjectFile(const QUrl &fileUrl)
{
    d->_projectFile = fileUrl;
    d->_temporary = false;
}

void Project::setGraphEditor(Editor *graphEditor)
{
    d->m_graphEditor = graphEditor;
}

int Project::addCodeFile(const QUrl &file)
{
    QList<int> keys = d->_codeFileGroup.uniqueKeys();
    int newKey = 1;
    if (keys.count() > 0) {
        newKey = keys.last() + 1;
    }

    KConfigGroup newGroup(d->_config, "CodeFile" + QString("%1").arg(newKey));
    newGroup.writeEntry("file", Project::relativePath(projectDirectory(), file.toLocalFile()));
    newGroup.writeEntry("identifier", newKey);
    d->_codeFileGroup.insert(newKey, "CodeFile" + QString("%1").arg(newKey));
    d->_modified = true;

    return newKey;
}

void Project::removeCodeFile(int fileID)
{
    d->_config->deleteGroup("CodeFile" + fileID);
    d->_codeFileGroup.remove(fileID);
}

QList<QUrl> Project::codeFiles() const
{
    QList<QUrl> files;
    foreach(const QString& fileGroup, d->_codeFileGroup) {
        KConfigGroup group(d->_config, fileGroup);
        QString file = group.readEntry("file");
        if (QUrl::fromLocalFile(file).isRelative()) {
            files.append(QUrl::fromLocalFile(projectDirectory() + group.readEntry("file")));
        } else {
            files.append(QUrl::fromLocalFile(group.readEntry("file")));
        }
    }
    return files;
}

QList< KTextEditor::Document* > Project::codeFilesNew() const
{
    return d->_codeFileNew;
}

void Project::addCodeFileNew(KTextEditor::Document *document)
{
    d->_codeFileNew.append(document);
}

void Project::removeCodeFileNew(KTextEditor::Document *document)
{
    d->_codeFileNew.removeAll(document);
}

void Project::saveCodeFileNew(KTextEditor::Document *document, const QUrl &file)
{
    removeCodeFileNew(document);
    document->saveAs(file);
    addCodeFile(file);
}

QList<GraphDocumentPtr> Project::graphDocuments() const
{
    return d->m_graphDocuments;
}

GraphDocumentPtr Project::activeGraph() const
{
    return d->m_activeGraphDocument;
}

GraphDocumentPtr Project::createGraphDocument()
{
    GraphDocumentPtr document;
    if (d->m_graphEditor) {
        document = d->m_graphEditor->createDocument();
    } else {
        document = GraphDocument::create();
    }
    d->m_graphDocuments.append(document);
    if (d->m_activeGraphDocument.isNull()) {
        d->m_activeGraphDocument = document;
        emit activeGraphDocumentChanged();
    }
    return document;
}

void Project::removeGraphDocument(GraphDocumentPtr document)
{
    // remove document from list, next save will then exclude document from be written to container
    d->m_graphDocuments.removeOne(document);
}

void Project::saveGraphFileNew(GraphDocumentPtr document, const QUrl &fileUrl)
{
    document->documentSaveAs(fileUrl);
    document->setDocumentUrl(fileUrl);
}

void Project::saveGraphFileAs(GraphDocumentPtr document, const QUrl &fileUrl)
{
    Q_ASSERT(document);
    document->setDocumentUrl(fileUrl);
    document->documentSave();
}

QUrl Project::journalFile() const
{
    if (projectDirectory().isEmpty()) {
        return QUrl();
    }
    KConfigGroup group(d->_config, "Journal");
    return QUrl::fromLocalFile(projectDirectory().append(group.readEntry("JournalHtml", QString())));
}

bool Project::writeNewProjectFile()
{
    if (!d->_config->isConfigWritable(true)) {
        qCritical() << "Cannot write to project config file.";
        return false;
    }
    d->_config->sync();
    d->_modified = false;

    return true;
}

bool Project::writeProjectFile(const QString &fileUrl)
{
    if (fileUrl.isEmpty() && isTemporary()) {
        qCritical() << "Could not save temporary project file: no file URL specified.";
        return false;
    }

    if (!fileUrl.isEmpty()) {
        // do not save to the old file
        d->_config->markAsClean();
        d->_projectFile = QUrl::fromLocalFile(fileUrl);

        // copy and save
        KConfig* temp = d->_config->copyTo(fileUrl);
        delete d->_config;
        d->_config = temp;
    }

    // update file reference lists
    KConfigGroup projectGroup(d->_config, "Project");

    QStringList codeFileIDs;
    foreach(const QString& fileGroup, d->_codeFileGroup) {
        KConfigGroup group(d->_config, fileGroup);
        // TODO change to order given by editor
        codeFileIDs.append(group.readEntry("identifier"));
    }
    projectGroup.writeEntry("CodeFiles", codeFileIDs);

    // add all graph files
    QStringList graphFileIDs;
    int newKey = 0;
    foreach(GraphDocumentPtr graph, d->m_graphDocuments) {
        KConfigGroup newGroup(d->_config, "GraphFile" + QString("%1").arg(newKey));
        newGroup.writeEntry("file", relativePath(projectDirectory(), graph->documentUrl().toLocalFile()));
        newGroup.writeEntry("identifier", newKey);
        graphFileIDs.append(QString(newKey));

    }
    projectGroup.writeEntry("GraphFiles", graphFileIDs);

    // write back
    d->_config->sync();
    d->_temporary = false;
    d->_modified = false;

    return true;
}

bool Project::exportProject(const QUrl &exportUrl)
{
    KTar tar(exportUrl.toLocalFile());
    if (!tar.open(QIODevice::WriteOnly)) {
        qCritical() << "Could not open export archive to write.";
        return false;
    }

    // create project configuration for export
    QTemporaryFile tmpProjectConfig;
    tmpProjectConfig.open();
    KConfig* exportConfig = d->_config->copyTo(tmpProjectConfig.fileName());
    KConfigGroup projectGroup(exportConfig, "Project");

    // add all scripts to tarball
    QStringList codeFileIDs;
    QMap<int, QString>::const_iterator iter = d->_codeFileGroup.constBegin();
    while (iter != d->_codeFileGroup.constEnd()) {
        KConfigGroup group(exportConfig, (*iter));

        // get file url and add to Tar
        QString configFileString = group.readEntry("file");
        QUrl file;
        if (QUrl::fromLocalFile(configFileString).isRelative()) {
            file = QUrl(projectDirectory() + configFileString);
        } else {
            file = QUrl::fromLocalFile(configFileString);
        }
        tar.addLocalFile(file.toLocalFile(), file.fileName());

        // update export project config in case of out-of-project-dir files
        group.writeEntry("file", relativePath(projectDirectory(), file.fileName()));
        ++iter;
        codeFileIDs.append(group.readEntry("identifier"));
    }
    projectGroup.writeEntry("CodeFiles", codeFileIDs);

    // add all graph files
    QStringList graphFileIDs;
    int newKey = 0;
    foreach(GraphDocumentPtr graph, d->m_graphDocuments) {
        KConfigGroup newGroup(exportConfig, "GraphFile" + QString("%1").arg(newKey));
        newGroup.writeEntry("file", relativePath(projectDirectory(), graph->documentUrl().toLocalFile()));
        newGroup.writeEntry("identifier", newKey);
        graphFileIDs.append(QString(newKey));

    }
    projectGroup.writeEntry("GraphFiles", graphFileIDs);

    QUrl journal(journalFile());
    if (!journal.isEmpty()) {
        tar.addLocalFile(journal.toLocalFile(), journal.fileName());
    }

    // write project to export
    projectGroup.writeEntry("Directory", "");
    exportConfig->sync();
    tar.addLocalFile(tmpProjectConfig.fileName(), "project.rocs");

    tar.close(); // write tar
    tmpProjectConfig.close(); // delete temporary config

    return true;
}

bool Project::isTemporary()
{
    return d->_temporary;
}

bool Project::isModified() const
{
    return d->_modified;
}

void Project::setModified(bool modified)
{
    d->_modified = modified;
}

QString Project::relativePath(const QString &base_dir, const QString &path)
{
    QString _base_dir(QDir::cleanPath(base_dir));
    QString _path(QDir::cleanPath(path.isEmpty() || QDir::isRelativePath(path) ? _base_dir + QLatin1Char('/') + path : path));

    if (_base_dir.isEmpty()) {
        return _path;
    }

    if (_base_dir[_base_dir.length() - 1] != QLatin1Char('/')) {
        _base_dir.append(QLatin1Char('/'));
    }

    const QStringList list1 = _base_dir.split(QLatin1Char('/'), QString::SkipEmptyParts);
    const QStringList list2 = _path.split(QLatin1Char('/'), QString::SkipEmptyParts);

    // Find where they meet
    int level = 0;
    int maxLevel = qMin(list1.count(), list2.count());
    while ((level < maxLevel) && (list1[level] == list2[level])) {
        level++;
    }

    QString result;
    // Need to go down out of the first path to the common branch.
    for (int i = level; i < list1.count(); i++) {
        result.append(QLatin1String("../"));
    }

    // Now up up from the common branch to the second path.
    for (int i = level; i < list2.count(); i++) {
        result.append(list2[i]).append(QLatin1Char('/'));
    }

    if ((level < list2.count()) && (path[path.length() - 1] != QLatin1Char('/'))) {
        result.truncate(result.length() - 1);
    }

    return result;
}
