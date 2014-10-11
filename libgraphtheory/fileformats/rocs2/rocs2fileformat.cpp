/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "rocs2fileformat.h"
#include "fileformats/fileformatinterface.h"
#include "modifiers/topology.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include <KLocalizedString>
#include <KPluginFactory>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QUrl>
#include <QDebug>

using namespace GraphTheory;

K_PLUGIN_FACTORY_WITH_JSON( FilePluginFactory,
                            "rocs2fileformat.json",
                            registerPlugin<Rocs2FileFormat>();)

Rocs2FileFormat::Rocs2FileFormat(QObject* parent, const QList< QVariant >&)
    : FileFormatInterface("rocs_tgffileformat", parent)
{
}

Rocs2FileFormat::~Rocs2FileFormat()
{
}

const QStringList Rocs2FileFormat::extensions() const
{
    return QStringList()
           << i18n("Rocs Graph Format (%1)", QString("*.graph2")); // do not confuse with Rocs-1 format
}

void Rocs2FileFormat::readFile()
{
    GraphDocumentPtr document = GraphDocument::create();

    // cleanup default
    document->remove(document->nodeTypes().first());
    document->remove(document->edgeTypes().first());

    QFile fileHandle(file().toLocalFile());
    if (!fileHandle.open(QFile::ReadOnly)) {
        setError(CouldNotOpenFile, i18n("Could not open file \"%1\" in read mode: %2", file().toLocalFile(), fileHandle.errorString()));
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileHandle.readAll());
    QJsonObject jsonObj = jsonDoc.object();

    // check format
    int formatVersion = jsonObj["FormatVersion"].toInt();
    if (formatVersion != 1) {
        qCritical() << "File format has version" << formatVersion << "which is higher than the latest supported version.";
    }

    // import node types
    QJsonArray nodeTypesJson = jsonObj["NodeTypes"].toArray();
    for (int index = 0; index < nodeTypesJson.count(); ++index) {
        QJsonObject typeJson = nodeTypesJson.at(index).toObject();
        NodeTypePtr type = NodeType::create(document);
        type->setId(typeJson["Id"].toInt());
        type->setName(typeJson["Name"].toString());
        type->setColor(QColor(typeJson["Color"].toString()));

        QJsonArray propertiesJson = typeJson["Properties"].toArray();
        for (int pIndex = 0; pIndex < propertiesJson.count(); ++pIndex) {
            type->addDynamicProperty(propertiesJson.at(pIndex).toString());
        }
    }

    // import edge types
    QJsonArray edgeTypesJson = jsonObj["EdgeTypes"].toArray();
    for (int index = 0; index < edgeTypesJson.count(); ++index) {
        QJsonObject typeJson = edgeTypesJson.at(index).toObject();
        EdgeTypePtr type = EdgeType::create(document);
        type->setId(typeJson["Id"].toInt());
        type->setName(typeJson["Name"].toString());
        type->setColor(QColor(typeJson["Color"].toString()));
        type->setDirection(direction(typeJson["Direction"].toString()));

        QJsonArray propertiesJson = typeJson["Properties"].toArray();
        for (int pIndex = 0; pIndex < propertiesJson.count(); ++pIndex) {
            type->addDynamicProperty(propertiesJson.at(pIndex).toString());
        }
    }

    setGraphDocument(document);
    setError(None);
}

void Rocs2FileFormat::writeFile(GraphDocumentPtr document)
{
    QFile fileHandle(file().toLocalFile());
    if (!fileHandle.open(QFile::WriteOnly | QFile::Text)) {
        setError(FileIsReadOnly, i18n("Could not open file \"%1\" in write mode: %2", file().fileName(), fileHandle.errorString()));
        return;
    }

    QJsonObject output; // this JSON object fill eventually be serialized
    output.insert("FormatVersion", 1);

    // serialize node types
    QJsonArray nodeTypes;
    foreach (NodeTypePtr type, document->nodeTypes()) {
        QJsonObject typeJson;
        typeJson.insert("Id", type->id());
        if (type->id() == -1) {
            qCritical() << "Serializing unset ID, this will break import";
        }
        typeJson.insert("Name", type->name());
        typeJson.insert("Color", type->color().name());
        QJsonArray propertiesJson;
        foreach (QString property, type->dynamicProperties()) {
            propertiesJson.append(property);
        }
        typeJson.insert("Properties", propertiesJson);
        nodeTypes.append(typeJson);
    }
    output.insert("NodeTypes", nodeTypes);

    // serialize edge types
    QJsonArray edgeTypes;
    foreach (EdgeTypePtr type, document->edgeTypes()) {
        QJsonObject typeJson;
        typeJson.insert("Id", type->id());
        if (type->id() == -1) {
            qCritical() << "Serializing unset ID, this will break import";
        }
        typeJson.insert("Name", type->name());
        typeJson.insert("Color", type->color().name());
        typeJson.insert("Direction", direction(type->direction()));
        QJsonArray propertiesJson;
        foreach (QString property, type->dynamicProperties()) {
            propertiesJson.append(property);
        }
        typeJson.insert("Properties", propertiesJson);
        edgeTypes.append(typeJson);
    }
    output.insert("EdgeTypes", edgeTypes);

    // serialize nodes
    QJsonArray nodes;
    foreach (NodePtr node, document->nodes()) {
        QJsonObject nodeJson;
    }

    // serialize to file
    QJsonDocument outputDocument(output);
    if (fileHandle.write(outputDocument.toJson()) == -1) {
        setError(Unknown, i18n("Error on serializing file format to file."));
        return;
    }

    //ONLY TESTING
    qDebug() << outputDocument.toJson();

    setError(None);
}

QString Rocs2FileFormat::direction(EdgeType::Direction direction) const
{
    switch (direction) {
    case EdgeType::Bidirectional:
        return "Bidirectional";
    case EdgeType::Unidirectional:
        return "Unidirectional";
    default:
        return "Undefined";
    }
}

EdgeType::Direction Rocs2FileFormat::direction(QString direction) const
{
    if (direction.startsWith("Unidirectional")) {
        return EdgeType::Unidirectional;
    }
    if (direction.startsWith("Bidirectional")) {
        return EdgeType::Bidirectional;
    }
    qCritical() << "Unknown direction, cannot convert and defaulting to Bidirectional";
    return EdgeType::Unidirectional;
}

#include "rocs2fileformat.moc"
