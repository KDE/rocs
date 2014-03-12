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
#include <QDebug>
#include <QUrl>
#include <KLocale>

ApiDocManager::ApiDocManager(QObject *parent)
    : QObject(parent)
{
}

void ApiDocManager::loadLocalData()
{
    QStringList apiDocFiles = KGlobal::dirs()->findAllResources("appdata", QString("engineapi/*.xml"));
    foreach (const QString &file, apiDocFiles) {
        loadObjectApi(QUrl::fromLocalFile(file));
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

QString ApiDocManager::objectApiDocument(const QString &identifier)
{
    if (_objectApiDocuments.contains(identifier)) {
        return _objectApiDocuments.value(identifier);
    }

    // get object API object
    ObjectDocumentation *objectApi = 0;
    foreach (ObjectDocumentation *obj, _objectApiList) {
        if (obj->id() == identifier) {
            objectApi = obj;
            break;
        }
    }
    if (!objectApi) {
        qCritical() << "Could not find Object API with ID " << identifier;
        return QString();
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

    // if parent object exists, find it
    ObjectDocumentation *parentObjectApi = 0;
    if (!objectApi->objectParent().isEmpty()) {
        foreach (ObjectDocumentation *obj, _objectApiList) {
            if (obj->id() == objectApi->objectParent()) {
                parentObjectApi = obj;
                break;
            }
        }
    }

    // object
    QVariant objectVar = QVariant::fromValue<QObject*>(objectApi);
    mapping.insert("object", objectVar);

    // properties
    // we use QHash to override parent properties
    QHash<QString, QVariant> propertyList;
    if (parentObjectApi) { // add properties from parent
        foreach (PropertyDocumentation *property, parentObjectApi->properties()) {
            propertyList.insert(property->name(), QVariant::fromValue<QObject*>(property));
        }
    }
    foreach (PropertyDocumentation *property, objectApi->properties()) {
        // override parent properties, if necessary
        propertyList.insert(property->name(), QVariant::fromValue<QObject*>(property));
    }
    mapping.insert("properties", propertyList.values());

    // properties
    QVariantList methodList;
    if (parentObjectApi) {
        foreach (MethodDocumentation *method, parentObjectApi->methods()) {
            methodList.append(QVariant::fromValue<QObject*>(method));
        }
    }
    foreach (MethodDocumentation *method, objectApi->methods()) {
        // TODO override parent methods
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

    // create and cache HTML file
    _objectApiDocuments.insert(objectApi->id(), t->render(&c));

    return _objectApiDocuments.value(identifier);
}

bool ApiDocManager::loadObjectApi(const QUrl &path)
{
    if (!path.isLocalFile()) {
        qWarning() << "Cannot open API file at " << path.toLocalFile() << ", aborting.";
        return false;
    }

    QXmlSchema schema = loadXmlSchema("engineApi");
    if (!schema.isValid()) {
        return false;
    }

    QDomDocument document = loadDomDocument(path, schema);
    if (document.isNull()) {
        qWarning() << "Could not parse document " << path.toLocalFile() << ", aborting.";
        return false;
    }

    QDomElement root(document.documentElement());

    // this addition must be performed for every object before any HTML documentation page
    // is created
    _objectApiCache.append(root.firstChildElement("id").text());

    // create object documentation
    ObjectDocumentation *objectApi = new ObjectDocumentation(this);
    _objectApiList.append(objectApi);
    emit objectApiAboutToBeAdded(objectApi, _objectApiList.count() - 1);

    objectApi->setTitle(root.firstChildElement("name").text());
    objectApi->setId(root.firstChildElement("id").text());
    objectApi->setComponentType(root.firstChildElement("componentType").text());
    objectApi->setSyntaxExample(root.firstChildElement("syntax").text());
    objectApi->setObjectParent(root.attribute("inherit"));
    QStringList paragraphs;
    for (QDomElement descriptionNode = root.firstChildElement("description").firstChildElement("para");
        !descriptionNode.isNull();
        descriptionNode = descriptionNode.nextSiblingElement())
    {
        paragraphs.append(i18nc("Scripting API Description", descriptionNode.text().toUtf8()));
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
            paragraphs.append(i18nc("Scripting API Description", descriptionNode.text().toUtf8()));
        }
        property->setDescription(paragraphs);
        if (_objectApiCache.contains(property->type())) {
            property->setTypeLink(property->type());
        }

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
        if (_objectApiCache.contains(method->returnType())) {
            method->setReturnTypeLink(method->returnType());
        }

        QStringList paragraphs;
        for (QDomElement descriptionNode = methodNode.firstChildElement("description").firstChildElement("para");
            !descriptionNode.isNull();
            descriptionNode = descriptionNode.nextSiblingElement())
        {
            paragraphs.append(i18nc("Scripting API Description", descriptionNode.text().toUtf8()));
        }
        method->setDescription(paragraphs);

        for (QDomElement parameterNode = methodNode.firstChildElement("parameters").firstChildElement();
            !parameterNode.isNull();
            parameterNode = parameterNode.nextSiblingElement())
        {
            QString typeLink;
            if (_objectApiCache.contains(parameterNode.firstChildElement("type").text())) {
                typeLink = parameterNode.firstChildElement("type").text();
            }
            method->addParameter(
                parameterNode.firstChildElement("name").text(),
                parameterNode.firstChildElement("type").text(),
                parameterNode.firstChildElement("info").text(),
                typeLink);
        }

        objectApi->addMethod(method);
    }

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
    QVariantList engineComponentList;
    QVariantList dataStructureComopnentList;
    foreach (ObjectDocumentation *object, _objectApiList) {
        switch (object->componentType()) {
        case ObjectDocumentation::EngineComponent:
            engineComponentList.append(QVariant::fromValue<QObject*>(object));
            break;
        case ObjectDocumentation::DataStructure:
            dataStructureComopnentList.append(QVariant::fromValue<QObject*>(object));
            break;
        case ObjectDocumentation::Pointer:
            dataStructureComopnentList.append(QVariant::fromValue<QObject*>(object));
            break;
        case ObjectDocumentation::Data:
            dataStructureComopnentList.append(QVariant::fromValue<QObject*>(object));
            break;
        }
    }
    mapping.insert("engineComponents", engineComponentList);
    mapping.insert("dataStructureComponents", dataStructureComopnentList);

    // localized strings
    mapping.insert("i18nScriptEngineApi", i18nc("@title", "Script Engine API"));
    mapping.insert("i18nObjects", i18nc("@title", "Objects"));
    mapping.insert("i18nDataStructures", i18nc("@title", "Data Structures"));
    mapping.insert("i18nEngineComponents", i18nc("@title", "Script Engine Components"));

    Grantlee::Context c(mapping);

    // create HTML file
    return t->render(&c);
}

QXmlSchema ApiDocManager::loadXmlSchema(const QString &schemeName) const
{
    QString relPath = QString("schemes/%1.xsd").arg(schemeName);
    QUrl file = QUrl::fromLocalFile(KGlobal::dirs()->findResource("appdata", relPath));

    QXmlSchema schema;
    if (schema.load(file) == false) {
        qWarning() << "Schema at file " << file.toLocalFile() << " is invalid.";
    }
    return schema;
}

QDomDocument ApiDocManager::loadDomDocument(const QUrl &path, const QXmlSchema &schema) const
{
    QDomDocument document;
    QXmlSchemaValidator validator(schema);
    if (!validator.validate(path)) {
        qWarning() << "Schema is not valid, aborting loading of XML document:" << path.toLocalFile();
        return document;
    }

    QString errorMsg;
    QFile file(path.toLocalFile());
    if (file.open(QIODevice::ReadOnly)) {
        if (!document.setContent(&file, &errorMsg)) {
            qWarning() << errorMsg;
        }
    } else {
        qWarning() << "Could not open XML document " << path.toLocalFile() << " for reading, aborting.";
    }
    return document;
}
