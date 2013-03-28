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
#include "PropertyDocumentation.h"
#include "MethodDocumentation.h"

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
#include <KLocale>

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

QList< ObjectDocumentation* > ApiDocManager::objectApiList() const
{
    return _objectApiList;
}

ObjectDocumentation * ApiDocManager::objectApi(int index) const
{
    Q_ASSERT (index >= 0 && index < _objectApiList.count());
    return _objectApiList.at(index);
}

QString ApiDocManager::objectApiDocument(const QString &identifier) const
{
    return _objectApiDocuments.value(identifier);
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

    objectApi->setTitle(root.firstChildElement("name").text());
    objectApi->setSyntaxExample(root.firstChildElement("syntax").text());
    QStringList paragraphs;
    for (QDomElement descriptionNode = root.firstChildElement("description").firstChildElement("para");
        !descriptionNode.isNull();
        descriptionNode = descriptionNode.nextSiblingElement())
    {
        paragraphs.append(descriptionNode.text());
    }
    objectApi->setDescription(paragraphs);

    // set property documentation
    for (QDomElement propertyNode = root.firstChildElement("properties").firstChildElement();
        !propertyNode.isNull();
        propertyNode = propertyNode.nextSiblingElement())
    {
        PropertyDocumentation *property = new PropertyDocumentation(objectApi);
        property->setName(propertyNode.firstChildElement("name").text());
        property->setType(propertyNode.firstChildElement("type").text());

        QStringList paragraphs;
        for (QDomElement descriptionNode = propertyNode.firstChildElement("description").firstChildElement("para");
            !descriptionNode.isNull();
            descriptionNode = descriptionNode.nextSiblingElement())
        {
            paragraphs.append(descriptionNode.text());
        }
        property->setDescription(paragraphs);

        objectApi->addProperty(property);
    }

    // set method documentation
    for (QDomElement methodNode = root.firstChildElement("methods").firstChildElement();
        !methodNode.isNull();
        methodNode = methodNode.nextSiblingElement())
    {
        MethodDocumentation *method = new MethodDocumentation(objectApi);
        method->setName(methodNode.firstChildElement("name").text());
        method->setReturnType(methodNode.firstChildElement("returnType").text());

        QStringList paragraphs;
        for (QDomElement descriptionNode = methodNode.firstChildElement("description").firstChildElement("para");
            !descriptionNode.isNull();
            descriptionNode = descriptionNode.nextSiblingElement())
        {
            paragraphs.append(descriptionNode.text());
        }
        method->setDescription(paragraphs);

        for (QDomElement parameterNode = methodNode.firstChildElement("parameters").firstChildElement();
            !parameterNode.isNull();
            parameterNode = parameterNode.nextSiblingElement())
        {
            method->addParameter(
                parameterNode.firstChildElement("name").text(),
                parameterNode.firstChildElement("type").text(),
                parameterNode.firstChildElement("info").text());
        }

        objectApi->addMethod(method);
    }

    // create API document
    // TODO port this documentation to a template base solution
    QString apiDocument;
    apiDocument.append(QString("<h1>%1</h1>").arg(objectApi->title()));
    foreach (QString description, objectApi->description()) {
        apiDocument.append(QString("<p>%1</p>").arg(description));
    }
    if (!objectApi->syntaxExample().isEmpty()) {
        apiDocument.append(i18n("<h2>Syntax</h2>"));
        apiDocument.append(QString("<verbatim>%1</verbatim>").arg(objectApi->syntaxExample()));
    }
    apiDocument.append(i18n("<h2>Properties</h2>"));
    foreach (PropertyDocumentation *property, objectApi->properties()) {
        apiDocument.append(QString("<h3>%1</h3>").arg(property->name()));
        foreach (QString description, property->description()) {
            apiDocument.append(QString("<p>%1</p>").arg(description));
        }
        apiDocument.append("<dl><dt>").append(i18n("Type")).
            append(QString("</dt><dd>%1</dd></dl>").arg(property->type()));
    }
    apiDocument.append(i18n("<h2>Methods</h2>"));
    foreach (MethodDocumentation *method, objectApi->methods()) {
        apiDocument.append(QString("<h3>%1</h3>").arg(method->name()));
        foreach (QString description, method->description()) {
            apiDocument.append(QString("<p>%1</p>").arg(description));
        }
        apiDocument.append("<dl>");
        foreach (MethodDocumentation::Parameter parameter, method->parameters()) {
        apiDocument.append(i18n("<b>Parameters</b>"));
        apiDocument.append(QString("<dt>(%1) %2</dt><dd>%3</dd>").
            arg(parameter.type).arg(parameter.name).arg(parameter.info));
        }
        apiDocument.append("<dt>").append(i18n("Return Value")).
            append(QString("</dt><dd>%1</dd>").arg(method->returnType()));
        apiDocument.append("</dl>");
    }
    _objectApiDocuments.insert(objectApi->title(), apiDocument);

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
