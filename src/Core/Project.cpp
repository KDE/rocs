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

#include <kurl.h>
#include <kconfig.h>
#include <kconfiggroup.h>
#include <ktemporaryfile.h>
#include <kdebug.h>
#include <ksharedconfig.h>

class ProjectPrivate
{
public:
    ProjectPrivate() {}

    QString _name;
    QString _projectPath;
    QString _projectFile;
    QMap<int, QString> _codeFileGroup;
    QMap<int, QString> _graphFileGroup;
    QList<Document*> _graphDocumentNew;
    QString _journalFile;
    KSharedConfig::Ptr _config;
    bool _temporary;

    KConfigGroup initKConfigObject() {
        // helper method for Project::open()
        kDebug() << "Creating KConfig object temporary project file: " << _projectFile;
        _config = KSharedConfig::openConfig(_projectFile);

        KConfigGroup projectGroup(_config, "Project");

        QStringList codeFileIDs = projectGroup.readEntry("CodeFiles", QStringList());
        foreach(QString offset, codeFileIDs) {
            _codeFileGroup.insert(offset.toInt(), "CodeFile" + offset);
        }

        QStringList graphFileIDs = projectGroup.readEntry("GraphFiles", QStringList());
        foreach(QString offset, graphFileIDs) {
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
    temp.setPrefix("project");
    temp.setSuffix(".rocs");
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
    projectGroup.writeEntry("Name",name);
}

QString Project::name() const
{
    KConfigGroup projectGroup(d->_config, "Project");
    return projectGroup.readEntry("Name", QString());
}

void Project::setProjectPath(QString directory)
{
    d->_projectPath = directory;
}

QString Project::projectPath() const
{
    return d->_projectPath;
}

int Project::addCodeFile(QString file)
{
    QList<int> keys = d->_codeFileGroup.uniqueKeys();
    int newKey = 1;
    if (keys.count() > 0) {
        newKey = keys.last() + 1;
    }

    KConfigGroup newGroup(d->_config, "CodeFile" + QString("%1").arg(newKey));
    newGroup.writeEntry("file", file);
    newGroup.writeEntry("identifier", newKey);
    d->_codeFileGroup.insert(newKey, "CodeFile" + QString("%1").arg(newKey));

    return newKey;
}

void Project::removeCodeFile(int fileID)
{
    d->_config->deleteGroup(d->_codeFileGroup[fileID]);
    d->_codeFileGroup.remove(fileID);
}

QList< QString > Project::codeFiles() const
{
    QList<QString> files;
    foreach(QString fileGroup, d->_codeFileGroup.values()) {
        KConfigGroup group(d->_config, fileGroup);
        files.append(group.readEntry("file"));
    }
    return files;

}

int Project::addGraphFile(QString file)
{
    QList<int> keys = d->_graphFileGroup.uniqueKeys();
    int newKey = 1;
    if (keys.count() > 0) {
        newKey = keys.last() + 1;
    }

    KConfigGroup newGroup(d->_config, "GraphFile" + QString("%1").arg(newKey));
    newGroup.writeEntry("file", file);
    newGroup.writeEntry("identifier", newKey);
    d->_graphFileGroup.insert(newKey, "GraphFile" + QString("%1").arg(newKey));

    return newKey;
}

void Project::removeGraphFile(int fileID)
{
    d->_config->deleteGroup(d->_graphFileGroup[fileID]);
    d->_graphFileGroup.remove(fileID);
}

QList< QString > Project::graphFiles() const
{
    QList< QString > files;
    foreach(QString fileGroup, d->_graphFileGroup.values()) {
        KConfigGroup group(d->_config, fileGroup);
        files.append(group.readEntry("file"));
    }
    return files;
}

void Project::addGraphDocumentNew(Document* document)
{
    d->_graphDocumentNew.append(document);
}

void Project::removeGraphDocumentNew(Document* document)
{
    d->_graphDocumentNew.removeAll(document);
}

void Project::saveGraphDocumentNew(Document* document)
{
    removeGraphDocumentNew(document);
    addGraphFile(document->documentPath());
}

void Project::setJournalFile(QString file)
{
    d->_journalFile = file;
}

QString Project::journalFile() const
{
    return d->_journalFile;
}

bool Project::writeNewProjectFile()
{
    if (!d->_config->isConfigWritable(true)) {
        kDebug() << "Cannot write to project config file.";
        return false;
    }
    KConfigGroup group = d->_config->group("Project");
    group.writeEntry("Name", d->_name);
    d->_config->sync();

    return true;
}

bool Project::writeProjectFile()
{
    // update file reference lists
    KConfigGroup projectGroup(d->_config, "Project");

    QStringList codeFileIDs;
    foreach(QString fileGroup, d->_codeFileGroup.values()) {
        KConfigGroup group(d->_config, fileGroup);
        // TODO change to order given by editor
        codeFileIDs.append(group.readEntry("identifier"));
    }
    projectGroup.writeEntry("GraphFiles", codeFileIDs);

    QStringList graphFileIDs;
    foreach(QString fileGroup, d->_graphFileGroup.values()) {
        KConfigGroup group(d->_config, fileGroup);
        // TODO change to order given by editor
        graphFileIDs.append(group.readEntry("identifier"));
    }
    projectGroup.writeEntry("GraphFiles", graphFileIDs);

    // write back
    d->_config->sync();

    return true;
}

bool Project::isTemporary()
{
    return d->_temporary;
}
