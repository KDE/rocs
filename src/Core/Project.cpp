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
#include <KDebug>
#include <KTextEditor/Document>

class ProjectPrivate
{
public:
    ProjectPrivate() {}

    QString _projectDirectory;
    QString _projectFile;
    QMap<int, KUrl> _codeFileGroup;
    QMap<int, QString> _graphFileGroup;
    QList<Document*> _graphFileNew;
    QList<KTextEditor::Document*> _codeFileNew;
    QString _journalFile;
    KConfig* _config;
    bool _temporary;

    KConfigGroup initKConfigObject() {
        // helper method for Project::open()
        kDebug() << "Creating KConfig object temporary project file: " << _projectFile;
        _config = new KConfig(_projectFile);

        KConfigGroup projectGroup(_config, "Project");
        _projectDirectory = projectGroup.readEntry("Directory", QString());

        QStringList codeFileIDs = projectGroup.readEntry("CodeFiles", QStringList());
        foreach(const QString& offset, codeFileIDs) {
            _codeFileGroup.insert(offset.toInt(), "CodeFile" + offset);
        }

        QStringList graphFileIDs = projectGroup.readEntry("GraphFiles", QStringList());
        foreach(const QString& offset, graphFileIDs) {
            _graphFileGroup.insert(offset.toInt(), "GraphFile" + offset);
        }

        KConfigGroup journalGroup(_config, "Journal");
        _journalFile = journalGroup.readEntry("JournalFile", QString());

        return projectGroup;
    }
};


Project::Project() :
    d(new ProjectPrivate)
{
    KTemporaryFile temp;
    temp.setPrefix("rocsproject");
    temp.setSuffix(".tmp");
    temp.setAutoRemove(false);
    temp.open();
    d->_projectFile = temp.fileName();
    d->initKConfigObject();
    d->_temporary = true;
}


Project::Project(QString projectFile) :
    d(new ProjectPrivate)
{
    d->_projectFile = projectFile;
    d->initKConfigObject();
    if (!d->_config->isConfigWritable(true)) {
        d->_temporary = true;
    } else {
        d->_temporary = false;
    }
}


Project::~Project()
{

}

void Project::setName(QString name)
{
    KConfigGroup projectGroup(d->_config, "Project");
    projectGroup.writeEntry("Name", name);
}


QString Project::name() const
{
    KConfigGroup projectGroup(d->_config, "Project");
    return projectGroup.readEntry("Name", QString());
}


void Project::setProjectDirectory(KUrl directory)
{
    d->_projectDirectory = directory.directory(KUrl::AppendTrailingSlash);
}


QString Project::projectDirectory() const
{
    return d->_projectDirectory;
}


int Project::addCodeFile(KUrl file)
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

    return newKey;
}


void Project::removeCodeFile(int fileID)
{
    d->_config->deleteGroup("CodeFile" + fileID);
    d->_codeFileGroup.remove(fileID);
}


QList< KUrl > Project::codeFiles() const
{
    QList<KUrl> files;
    foreach(const KUrl& fileGroup, d->_codeFileGroup.values()) {
        KConfigGroup group(d->_config, fileGroup.toLocalFile());
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


void Project::addCodeFileNew(KTextEditor::Document* document)
{
    d->_codeFileNew.append(document);
}


void Project::removeCodeFileNew(KTextEditor::Document* document)
{
    d->_codeFileNew.removeAll(document);
}


void Project::saveCodeFileNew(KTextEditor::Document* document, KUrl file)
{
    removeCodeFileNew(document);
    document->saveAs(file);
    addCodeFile(file);
}


int Project::addGraphFile(KUrl file)
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

    return newKey;
}


void Project::removeGraphFile(int fileID)
{
    d->_config->deleteGroup("GraphFile" + fileID);
    d->_graphFileGroup.remove(fileID);
}


QList< KUrl > Project::graphFiles() const
{
    QList< KUrl > files;
    foreach(const KUrl& fileGroup, d->_graphFileGroup.values()) {
        KConfigGroup group(d->_config, fileGroup.toLocalFile());
        QString file = group.readEntry("file");
        if (KUrl::isRelativeUrl(file)) {
            files.append(KUrl(projectDirectory(), group.readEntry("file")));
        } else {
            files.append(KUrl::fromLocalFile(group.readEntry("file")));
        }
    }
    return files;
}


void Project::addGraphFileNew(Document* document)
{
    d->_graphFileNew.append(document);
}


void Project::removeGraphFileNew(Document* document)
{
    d->_graphFileNew.removeAll(document);
}


void Project::saveGraphFileNew(Document* document, QString file)
{
    removeGraphFileNew(document);
    document->saveAs(file);
    addGraphFile(KUrl::fromLocalFile(document->fileUrl()));
}


void Project::saveGraphFileAs(Document* document, QString file)
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


void Project::setJournalFile(KUrl file)
{
    d->_journalFile = file.toLocalFile();
}


KUrl Project::journalFile() const
{
    return KUrl::fromPath(d->_journalFile);
}


bool Project::writeNewProjectFile()
{
    if (!d->_config->isConfigWritable(true)) {
        kDebug() << "Cannot write to project config file.";
        return false;
    }
    KConfigGroup group = d->_config->group("Project");
    group.writeEntry("Directory", d->_projectDirectory);
    d->_config->sync();

    return true;
}


bool Project::writeProjectFile(QString fileUrl)
{
    if (fileUrl.isEmpty() && isTemporary()) {
        kDebug() << "Could not save temporary project file: no file URL specified.";
        return false;
    }

    if (!fileUrl.isEmpty()) {
        // do not save to the old file
        d->_config->markAsClean();

        // copy and save
        KConfig* temp = d->_config->copyTo(fileUrl);
        delete d->_config;
        d->_config = temp;
    }

    // update file reference lists
    KConfigGroup projectGroup(d->_config, "Project");
    projectGroup.writeEntry("Directory", d->_projectDirectory);

    QStringList codeFileIDs;
    foreach(const KUrl& fileGroup, d->_codeFileGroup.values()) {
        KConfigGroup group(d->_config, fileGroup.toLocalFile());
        // TODO change to order given by editor
        codeFileIDs.append(group.readEntry("identifier"));
    }
    projectGroup.writeEntry("CodeFiles", codeFileIDs);

    QStringList graphFileIDs;
    foreach(const QString& fileGroup, d->_graphFileGroup.values()) {
        KConfigGroup group(d->_config, fileGroup);
        // TODO change to order given by editor
        graphFileIDs.append(group.readEntry("identifier"));
    }
    projectGroup.writeEntry("GraphFiles", graphFileIDs);

    // write back
    d->_config->sync();
    d->_temporary = false;

    return true;
}


bool Project::isTemporary()
{
    return d->_temporary;
}
