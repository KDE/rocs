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
    QList<QString> _codeFiles;
    QList<QString> _graphDocumentFiles;
    QList<Document*> _graphDocumentNew;
    QString _journalFile;
    KSharedConfig::Ptr _config;
    bool _temporary;

    KConfigGroup initKConfigObject() {
        // helper method for Project::open()
        kDebug() << "Creating KConfig object temporary project file: " << _projectFile;
        _config = KSharedConfig::openConfig(_projectFile);
        KConfigGroup projectGroup(_config, "Project");

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
    d->_name = name;
}

QString Project::name() const
{
    return d->_name;
}

void Project::setProjectPath(QString directory)
{
    d->_projectPath = directory;
}

QString Project::projectPath() const
{
    return d->_projectPath;
}

void Project::addCodeFile(QString file)
{
    d->_codeFiles.append(file);
}

void Project::removeCodeFile(QString file)
{
    d->_codeFiles.removeAll(file);
}

QList< QString > Project::codeFiles() const
{
    return d->_codeFiles;
}

void Project::addGraphDocumentFile(QString file)
{
    d->_graphDocumentFiles.append(file);
}

void Project::removeGraphDocumentFile(QString file)
{
    d->_graphDocumentFiles.removeAll(file);
}

QList< QString > Project::graphDocumentFiles() const
{
    return d->_graphDocumentFiles;
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
    addGraphDocumentFile(document->documentPath());
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

bool Project::isTemporary()
{
    return d->_temporary;
}
