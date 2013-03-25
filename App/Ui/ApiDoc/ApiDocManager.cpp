/*
    This file is part of Rocs.
    Copyright 2013  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "ApiDocManager.h"
#include "ObjectDocumentation.h"

#include <QIODevice>
#include <QFile>
#include <QPointer>
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QDomDocument>

#include <KGlobal>
#include <KStandardDirs>
#include <KDebug>
#include <KUrl>

ApiDocManager::ApiDocManager(QObject *parent)
    : QObject(parent)
{
}

void ApiDocManager::loadLocalData()
{
    QStringList apiDocFiles = KGlobal::dirs()->findAllResources("appdata", QString("scriptapi/*.xml"));
    foreach (const QString &file, apiDocFiles) {
        loadObjectApi(KUrl::fromLocalFile(file));
    }
}

ObjectDocumentation * ApiDocManager::objectApi(int index) const
{
    Q_ASSERT (index >= 0 && index < _objectApiList.count());
    return _objectApiList.at(index);
}

bool ApiDocManager::loadObjectApi(const KUrl &path)
{
    if (!path.isLocalFile()) {
        kWarning() << "Cannot open API file at " << path.toLocalFile() << ", aborting.";
        return false;
    }

    QXmlSchema schema = loadXmlSchema("objectScheme");
    if (!schema.isValid()) {
        return false;
    }

    QDomDocument document = loadDomDocument(path, schema);
    if (document.isNull()) {
        kWarning() << "Could not parse document " << path.toLocalFile() << ", aborting.";
        return false;
    }

    QDomElement root(document.documentElement());
    ObjectDocumentation *objectApi = new ObjectDocumentation(this);
    _objectApiList.append(objectApi);
    emit objectApiAboutToBeAdded(objectApi, _objectApiList.count() - 1);

    //TODO complete this: currently parsing only the object title
    objectApi->setTitle(root.firstChildElement("title").text());

    emit objectApiAdded();
    return true;
}

QXmlSchema ApiDocManager::loadXmlSchema(const QString &schemeName) const
{
    QString relPath = QString("schemes/%1.xsd").arg(schemeName);
    KUrl file = KUrl::fromLocalFile(KGlobal::dirs()->findResource("appdata", relPath));

    QXmlSchema schema;
    if (schema.load(file) == false) {
        kWarning() << "Schema at file " << file.toLocalFile() << " is invalid.";
    }
    return schema;
}

QDomDocument ApiDocManager::loadDomDocument(const KUrl &path, const QXmlSchema &schema) const
{
    QDomDocument document;
    QXmlSchemaValidator validator(schema);
    if (!validator.validate(path)) {
        kWarning() << "Schema is not valid, aborting loading of XML document:" << path.toLocalFile();
        return document;
    }

    QString errorMsg;
    QFile file(path.toLocalFile());
    if (file.open(QIODevice::ReadOnly)) {
        if (!document.setContent(&file, &errorMsg)) {
            kWarning() << errorMsg;
        }
    } else {
        kWarning() << "Could not open XML document " << path.toLocalFile() << " for reading, aborting.";
    }
    return document;
}
