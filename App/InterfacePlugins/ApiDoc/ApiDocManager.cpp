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
#include "ParameterDocumentation.h"

#include <grantlee_core.h>

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
    QStringList apiDocFiles = KGlobal::dirs()->findAllResources("appdata", QString("engineapi/*.xml"));
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

    QXmlSchema schema = loadXmlSchema("engineApi");
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
    objectApi->setId(root.firstChildElement("id").text());
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

    // initialize Grantlee engine
    Grantlee::Engine *engine = new Grantlee::Engine( this );
    Grantlee::FileSystemTemplateLoader::Ptr loader = Grantlee::FileSystemTemplateLoader::Ptr(
        new Grantlee::FileSystemTemplateLoader() );
    loader->setTemplateDirs(KGlobal::dirs()->resourceDirs("appdata"));
    engine->addTemplateLoader(loader);
    Grantlee::Template t = engine->loadByName("plugin/apidoc/objectApi.html");
    Grantlee::registerMetaType<ParameterDocumentation*>();

    // create mapping
    QVariantHash mapping;

    // object
    QVariant objectVar = QVariant::fromValue<QObject*>(objectApi);
    mapping.insert("object", objectVar);

    // properties
    QVariantList propertyList;
    foreach (PropertyDocumentation *property, objectApi->properties()) {
        propertyList.append(QVariant::fromValue<QObject*>(property));
    }
    mapping.insert("properties", propertyList);

    // properties
    QVariantList methodList;
    foreach (MethodDocumentation *method, objectApi->methods()) {
        methodList.append(QVariant::fromValue<QObject*>(method));
    }
    mapping.insert("methods", methodList);

    mapping.insert("i18nSyntax", i18nc("@title", "Syntax"));
    mapping.insert("i18nProperties", i18nc("@title", "Properties"));
    mapping.insert("i18nParameters", i18nc("@title", "Parameters"));
    mapping.insert("i18nParameter", i18nc("@title", "Parameter"));
    mapping.insert("i18nMethods", i18nc("@title", "Methods"));
    mapping.insert("i18nType", i18nc("@title", "Type"));
    mapping.insert("i18nReturnType", i18nc("@title", "Return Type"));
    mapping.insert("i18nDetailedDescription", i18nc("@title", "Detailed Description"));
    mapping.insert("i18nDescription", i18nc("@title", "Description"));

    Grantlee::Context c(mapping);

    // create HTML file
    _objectApiDocuments.insert(objectApi->id(), t->render(&c));
    emit objectApiAdded();
    return true;
}

QString ApiDocManager::apiOverviewDocument() const
{
    // initialize Grantlee engine
    Grantlee::Engine engine;
    Grantlee::FileSystemTemplateLoader::Ptr loader = Grantlee::FileSystemTemplateLoader::Ptr(
        new Grantlee::FileSystemTemplateLoader() );
    loader->setTemplateDirs(KGlobal::dirs()->resourceDirs("appdata"));
    engine.addTemplateLoader(loader);
    Grantlee::Template t = engine.loadByName("plugin/apidoc/overview.html");
    Grantlee::registerMetaType<ParameterDocumentation*>();

    // create mapping
    QVariantHash mapping;

    // objects
    QVariantList objectList;
    foreach (ObjectDocumentation *object, _objectApiList) {
        objectList.append(QVariant::fromValue<QObject*>(object));
    }
    mapping.insert("objects", objectList);

    // localized strings
    mapping.insert("i18nScriptEngineApi", i18nc("@title", "Script Engine API"));
    mapping.insert("i18nObjects", i18nc("@title", "Objects"));
    Grantlee::Context c(mapping);

    // create HTML file
    return t->render(&c);
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
