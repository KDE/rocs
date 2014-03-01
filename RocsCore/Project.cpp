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


#include "Project.h"
#include "Document.h"

#include <QString>
#include <QMap>
#include <QList>

#include <KUrl>
#include <KConfig>
#include <KConfigGroup>
#include <KTemporaryFile>
#include <QDebug>
#include <KTextEditor/Document>
#include <KTar>

class ProjectPrivate
{
public:
    ProjectPrivate() {}

    KUrl _projectFile;
    QMap<int, QString> _codeFileGroup;
    QMap<int, QString> _graphFileGroup;
    QList<Document*> _graphFileNew;
    QList<KTextEditor::Document*> _codeFileNew;
    KConfig* _config;
    bool _temporary;
    bool _modified;

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
            _graphFileGroup.insert(offset.toInt(), "GraphFile" + offset);
        }

        // for now, journal files only have name "journal.html"
        KConfigGroup journalGroup(_config, "Journal");
        journalGroup.writeEntry("JournalHtml", "journal.html");

        return projectGroup;
    }
};

Project::Project()
    : d(new ProjectPrivate)
{
    KTemporaryFile tmpProjectFile;
    tmpProjectFile.setPrefix("rocsproject");
    tmpProjectFile.setSuffix(".tmp");
    tmpProjectFile.setAutoRemove(false);
    tmpProjectFile.open();
    d->_projectFile = KUrl::fromLocalFile(tmpProjectFile.fileName());

    d->initKConfigObject();
    d->_temporary = true;
    d->_modified = false;
}

Project::Project(const KUrl &projectFile) :
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

Project::Project(const KUrl &projectArchive, const KUrl &projectDirectory)
    : d(new ProjectPrivate)
{
    // extract archive into project directory
    KTar tar(projectArchive.toLocalFile());
    if (!tar.open(QIODevice::ReadOnly)) {
        qCritical() << "Could not open export archive to read.";
        return;
    }
    tar.directory()->copyTo(projectDirectory.path(), true);
    KUrl projectFile = projectDirectory.resolved(KUrl("project.rocs"));

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
    if (d->_temporary == true) {
        return "";
    }
    return d->_projectFile.directory(KUrl::AppendTrailingSlash);
}

KUrl Project::projectFile() const
{
    return d->_projectFile;
}

void Project::setProjectFile(const KUrl &fileUrl)
{
    d->_projectFile = fileUrl;
    d->_temporary = false;
}

int Project::addCodeFile(const KUrl &file)
{
    QList<int> keys = d->_codeFileGroup.uniqueKeys();
    int newKey = 1;
    if (keys.count() > 0) {
        newKey = keys.last() + 1;
    }

    KConfigGroup newGroup(d->_config, "CodeFile" + QString("%1").arg(newKey));
    newGroup.writeEntry("file", KUrl::relativePath(projectDirectory(), file.toLocalFile()));
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

QList<KUrl> Project::codeFiles() const
{
    QList<KUrl> files;
    foreach(const QString& fileGroup, d->_codeFileGroup) {
        KConfigGroup group(d->_config, fileGroup);
        QString file = group.readEntry("file");
        if (KUrl::isRelativeUrl(file)) {
            files.append(KUrl(projectDirectory(), group.readEntry("file")));
        } else {
            files.append(KUrl::fromLocalFile(group.readEntry("file")));
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

void Project::saveCodeFileNew(KTextEditor::Document *document, const KUrl &file)
{
    removeCodeFileNew(document);
    document->saveAs(file);
    addCodeFile(file);
}

int Project::addGraphFile(const KUrl &file)
{
    QList<int> keys = d->_graphFileGroup.uniqueKeys();
    int newKey = 1;
    if (keys.count() > 0) {
        newKey = keys.last() + 1;
    }

    KConfigGroup newGroup(d->_config, "GraphFile" + QString("%1").arg(newKey));
    newGroup.writeEntry("file", KUrl::relativePath(projectDirectory(), file.toLocalFile()));
    newGroup.writeEntry("identifier", newKey);
    d->_graphFileGroup.insert(newKey, "GraphFile" + QString("%1").arg(newKey));
    d->_modified = true;

    return newKey;
}

void Project::removeGraphFile(int fileID)
{
    d->_config->deleteGroup("GraphFile" + fileID);
    d->_graphFileGroup.remove(fileID);
}

QList<KUrl> Project::graphFiles() const
{
    QList< KUrl > files;
    foreach(const QString& fileGroup, d->_graphFileGroup) {
        KConfigGroup group(d->_config, fileGroup);
        QString file = group.readEntry("file");
        if (KUrl::isRelativeUrl(file)) {
            files.append(KUrl(projectDirectory(), group.readEntry("file")));
        } else {
            files.append(KUrl::fromLocalFile(group.readEntry("file")));
        }
    }
    return files;
}

void Project::addGraphFileNew(Document *document)
{
    d->_graphFileNew.append(document);
}


void Project::removeGraphFileNew(Document *document)
{
    d->_graphFileNew.removeAll(document);
}

void Project::saveGraphFileNew(Document *document, const QString &file)
{
    removeGraphFileNew(document);
    document->saveAs(file);
    addGraphFile(KUrl::fromLocalFile(document->fileUrl()));
}

void Project::saveGraphFileAs(Document *document, const QString &file)
{
    Q_ASSERT(document);
    if (d == 0) {
        return;
    }

    if (d->_graphFileNew.contains(document)) {
        saveGraphFileNew(document, file);
        return;
    }
    // TODO the following is probably error prone
    int filekey = d->_graphFileGroup.key(document->fileUrl());
    d->_graphFileGroup[filekey] = file;
    document->saveAs(file);
}

KUrl Project::journalFile() const
{
    if (projectDirectory().isEmpty()) {
        return KUrl();
    }
    KConfigGroup group(d->_config, "Journal");
    return KUrl::fromLocalFile(projectDirectory().append(group.readEntry("JournalHtml", QString())));
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
        d->_projectFile = KUrl::fromLocalFile(fileUrl);

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

    QStringList graphFileIDs;
    foreach(const QString& fileGroup, d->_graphFileGroup) {
        KConfigGroup group(d->_config, fileGroup);
        // TODO change to order given by editor
        graphFileIDs.append(group.readEntry("identifier"));
    }
    projectGroup.writeEntry("GraphFiles", graphFileIDs);

    // write back
    d->_config->sync();
    d->_temporary = false;
    d->_modified = false;

    return true;
}

bool Project::exportProject(const KUrl &exportUrl)
{
    KTar tar(exportUrl.toLocalFile());
    if (!tar.open(QIODevice::WriteOnly)) {
        qCritical() << "Could not open export archive to write.";
        return false;
    }

    // create project configuration for export
    KTemporaryFile tmpProjectConfig;
    tmpProjectConfig.setPrefix("export");
    tmpProjectConfig.setSuffix(".rocs");
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
        KUrl file;
        if (KUrl::isRelativeUrl(configFileString)) {
            file = KUrl(projectDirectory(), configFileString);
        } else {
            file = KUrl::fromLocalFile(configFileString);
        }
        tar.addLocalFile(file.toLocalFile(), file.fileName());

        // update export project config in case of out-of-project-dir files
        group.writeEntry("file", KUrl::relativePath(projectDirectory(), file.fileName()));
        ++iter;
        codeFileIDs.append(group.readEntry("identifier"));
    }
    projectGroup.writeEntry("CodeFiles", codeFileIDs);

    QStringList graphFileIDs;
    iter = d->_graphFileGroup.constBegin();
    while (iter != d->_graphFileGroup.constEnd()) {
        KConfigGroup group(exportConfig, (*iter));

        // get file url and add to Tar
        QString configFileString = group.readEntry("file");
        KUrl file;
        if (KUrl::isRelativeUrl(configFileString)) {
            file = KUrl(projectDirectory(), configFileString);
        } else {
            file = KUrl::fromLocalFile(configFileString);
        }
        tar.addLocalFile(file.toLocalFile(), file.fileName());

        // update export project config in case of out-of-project-dir files
        group.writeEntry("file", KUrl::relativePath(projectDirectory(), file.fileName()));
        ++iter;
        graphFileIDs.append(group.readEntry("identifier"));
    }
    projectGroup.writeEntry("GraphFiles", graphFileIDs);

    KUrl journal(journalFile());
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
