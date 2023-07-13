/*
 *  SPDX-FileCopyrightText: 2013-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include "scriptapimanager.h"
#include "method.h"
#include "object.h"
#include "parameter.h"
#include "property.h"

#include <grantlee/engine.h>
#include <grantlee/metatype.h>
#include <grantlee/templateloader.h>

#include <KLocalizedString>
#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QIODevice>
#include <QPointer>
#include <QStandardPaths>
#include <QString>
#include <QUrl>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

ScriptApiManager::ScriptApiManager(QObject *parent)
    : QObject(parent)
{
}

void ScriptApiManager::loadLocalData()
{
    // locate directory
    const QString dir = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("rocs/kernelapi"), QStandardPaths::LocateDirectory);
    if (dir.isEmpty()) {
        qCritical() << "Could not locate \"rocs/kernelapi\" directory, abort loading script API documentation.";
        return;
    }

    const QStringList files = QDir(dir).entryList(QStringList{QStringLiteral("*.xml")});
    for (const QString &file : files) {
        loadObjectApi(QUrl::fromLocalFile(dir + '/' + file));
    }
}

QList<Object *> ScriptApiManager::objectApiList() const
{
    return m_objectApiList;
}

Object *ScriptApiManager::objectApi(int index) const
{
    Q_ASSERT(index >= 0 && index < m_objectApiList.count());
    return m_objectApiList.at(index);
}

QString ScriptApiManager::objectApiDocument(const QString &identifier)
{
    if (m_objectApiDocuments.contains(identifier)) {
        return m_objectApiDocuments.value(identifier);
    }

    // get object API object
    Object *objectApi{nullptr};
    for (Object *obj : m_objectApiList) {
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
    Grantlee::Engine *engine = new Grantlee::Engine(this);
    QSharedPointer<Grantlee::FileSystemTemplateLoader> loader = QSharedPointer<Grantlee::FileSystemTemplateLoader>(new Grantlee::FileSystemTemplateLoader);
    loader->setTemplateDirs(QStandardPaths::standardLocations(QStandardPaths::DataLocation));
    engine->addTemplateLoader(loader);
    Grantlee::Template t = engine->loadByName("plugin/apidoc/objectApi.html");

    // create mapping
    QVariantHash mapping;

    // object
    QVariant objectVar = QVariant::fromValue<QObject *>(objectApi);
    mapping.insert("object", objectVar);

    // properties
    // we use QHash to override parent properties
    QHash<QString, QVariant> propertyList;
    const auto properties = objectApi->properties();
    for (Property *property : properties) {
        propertyList.insert(property->name(), QVariant::fromValue<QObject *>(property));
    }
    mapping.insert("properties", propertyList.values());

    // properties
    QVariantList methodList;
    const auto methods = objectApi->methods();
    for (Method *method : methods) {
        methodList.append(QVariant::fromValue<QObject *>(method));
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
    m_objectApiDocuments.insert(objectApi->id(), t->render(&c));

    return m_objectApiDocuments.value(identifier);
}

bool ScriptApiManager::loadObjectApi(const QUrl &path)
{
    if (!path.isLocalFile()) {
        qWarning() << "Cannot open API file at " << path.toLocalFile() << ", aborting.";
        return false;
    }

    QXmlSchema schema = loadXmlSchema("kernelapi");
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
    m_objectApiCache.append(root.firstChildElement("id").text());

    // create object documentation
    Object *objectApi = new Object(this);
    m_objectApiList.append(objectApi);
    Q_EMIT objectApiAboutToBeAdded(objectApi, m_objectApiList.count() - 1);

    objectApi->setTitle(root.firstChildElement("name").text());
    objectApi->setId(root.firstChildElement("id").text());
    objectApi->setComponentType(root.firstChildElement("componentType").text());
    objectApi->setSyntaxExample(root.firstChildElement("syntax").text());
    QStringList paragraphs;
    for (QDomElement descriptionNode = root.firstChildElement("description").firstChildElement("para"); !descriptionNode.isNull();
         descriptionNode = descriptionNode.nextSiblingElement()) {
        paragraphs.append(i18nc("Scripting API Description", descriptionNode.text().toUtf8()));
    }
    objectApi->setDescription(paragraphs);

    // set property documentation
    for (QDomElement propertyNode = root.firstChildElement("properties").firstChildElement(); !propertyNode.isNull();
         propertyNode = propertyNode.nextSiblingElement()) {
        Property *property = new Property(objectApi);
        property->setName(propertyNode.firstChildElement("name").text());
        property->setType(propertyNode.firstChildElement("type").text());

        QStringList paragraphs;
        for (QDomElement descriptionNode = propertyNode.firstChildElement("description").firstChildElement("para"); !descriptionNode.isNull();
             descriptionNode = descriptionNode.nextSiblingElement()) {
            paragraphs.append(i18nc("Scripting API Description", descriptionNode.text().toUtf8()));
        }
        property->setDescription(paragraphs);
        if (m_objectApiCache.contains(property->type())) {
            property->setTypeLink(property->type());
        }

        objectApi->addProperty(property);
    }

    // set method documentation
    for (QDomElement methodNode = root.firstChildElement("methods").firstChildElement(); !methodNode.isNull(); methodNode = methodNode.nextSiblingElement()) {
        Method *method = new Method(objectApi);
        method->setName(methodNode.firstChildElement("name").text());
        method->setReturnType(methodNode.firstChildElement("returnType").text());
        if (m_objectApiCache.contains(method->returnType())) {
            method->setReturnTypeLink(method->returnType());
        }

        QStringList paragraphs;
        for (QDomElement descriptionNode = methodNode.firstChildElement("description").firstChildElement("para"); !descriptionNode.isNull();
             descriptionNode = descriptionNode.nextSiblingElement()) {
            paragraphs.append(i18nc("Scripting API Description", descriptionNode.text().toUtf8()));
        }
        method->setDescription(paragraphs);

        for (QDomElement parameterNode = methodNode.firstChildElement("parameters").firstChildElement(); !parameterNode.isNull();
             parameterNode = parameterNode.nextSiblingElement()) {
            QString typeLink;
            if (m_objectApiCache.contains(parameterNode.firstChildElement("type").text())) {
                typeLink = parameterNode.firstChildElement("type").text();
            }
            method->addParameter(parameterNode.firstChildElement("name").text(),
                                 parameterNode.firstChildElement("type").text(),
                                 parameterNode.firstChildElement("info").text(),
                                 typeLink);
        }

        objectApi->addMethod(method);
    }

    Q_EMIT objectApiAdded();
    return true;
}

QString ScriptApiManager::apiOverviewDocument() const
{
    // initialize Grantlee engine
    Grantlee::Engine engine;
    QSharedPointer<Grantlee::FileSystemTemplateLoader> loader = QSharedPointer<Grantlee::FileSystemTemplateLoader>(new Grantlee::FileSystemTemplateLoader);
    loader->setTemplateDirs(QStandardPaths::standardLocations(QStandardPaths::DataLocation));
    engine.addTemplateLoader(loader);
    Grantlee::Template t = engine.loadByName("plugin/apidoc/overview.html");

    // create mapping
    QVariantHash mapping;

    // objects
    QVariantList kernelModuleList;
    QVariantList elementList;
    for (Object *object : m_objectApiList) {
        switch (object->componentType()) {
        case Object::KernelModule:
            kernelModuleList.append(QVariant::fromValue<QObject *>(object));
            break;
        case Object::Document:
            elementList.append(QVariant::fromValue<QObject *>(object));
            break;
        case Object::Edge:
            elementList.append(QVariant::fromValue<QObject *>(object));
            break;
        case Object::Node:
            elementList.append(QVariant::fromValue<QObject *>(object));
            break;
        }
    }
    mapping.insert("kernelModules", kernelModuleList);
    mapping.insert("kernelElements", elementList);

    // localized strings
    mapping.insert("i18nScriptEngineApi", i18nc("@title", "Script Engine API"));
    mapping.insert("i18nDocument", i18nc("@title", "The Graph Document"));
    mapping.insert("i18nObjects", i18nc("@title", "Objects"));
    mapping.insert("i18nEngineComponents", i18nc("@title", "Script Engine Modules"));

    Grantlee::Context c(mapping);

    // create HTML file
    return t->render(&c);
}

QXmlSchema ScriptApiManager::loadXmlSchema(const QString &schemeName) const
{
    QString relPath = QString("rocs/schemes/%1.xsd").arg(schemeName);
    QUrl file = QUrl::fromLocalFile(QStandardPaths::locate(QStandardPaths::GenericDataLocation, relPath));
    QXmlSchema schema;
    if (file.isEmpty() || schema.load(file) == false) {
        qWarning() << "Schema at file " << file.toLocalFile() << " is invalid.";
    }
    return schema;
}

QDomDocument ScriptApiManager::loadDomDocument(const QUrl &path, const QXmlSchema &schema) const
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

#include "moc_scriptapimanager.cpp"
