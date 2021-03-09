/*
 *  SPDX-FileCopyrightText: 2013-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include "scriptapimodel.h"
#include "object.h"
#include "method.h"
#include "property.h"

#include <QDebug>
#include <KLocalizedString>

class Item
{
public:
    Item(const QList<QVariant> &data, Item *parent = 0);
    ~Item();

    void appendChild(Item *child);

    Item *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void setDocumentAnchor(const QString &document, const QString &anchor);
    QString anchor();
    QString document();
    int row() const;
    Item *parent();

private:
    QList<Item*> _childItems;
    QList<QVariant> _itemData;
    QString _anchor;
    QString _document;
    Item *_parentItem;
};

Item::Item(const QList<QVariant> &data, Item *parent)
{
    _parentItem = parent;
    _itemData = data;
}

Item::~Item()
{
    qDeleteAll(_childItems);
}

void Item::appendChild(Item *item)
{
    _childItems.append(item);
}

Item * Item::child(int row)
{
    return _childItems.value(row);
}

int Item::childCount() const
{
    return _childItems.count();
}

int Item::row() const
{
    if (_parentItem) {
        return _parentItem->_childItems.indexOf(const_cast<Item*>(this));
    }
    return 0;
}

int Item::columnCount() const
{
    return _itemData.count();
}

QVariant Item::data(int column) const
{
    return _itemData.value(column);
}

void Item::setDocumentAnchor(const QString& document, const QString& anchor)
{
    _anchor = anchor;
    _document = document;
}

QString Item::anchor()
{
    return _anchor;
}

QString Item::document()
{
    return _document;
}

Item * Item::parent()
{
    return _parentItem;
}


// ScriptApiModel methods
ScriptApiModel::ScriptApiModel(QList<Object*> dataList, QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << i18n("Script Objects");
    rootItem = new Item(rootData);
    setupModelData(dataList, rootItem);
}

ScriptApiModel::~ScriptApiModel()
{
    delete rootItem;
}

QModelIndex ScriptApiModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    Item *parentItem;
    if (!parent.isValid()) {
        parentItem = rootItem;
    }
    else {
        parentItem = static_cast<Item*>(parent.internalPointer());
    }

    Item *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }
    else {
        return QModelIndex();
    }
}

QModelIndex ScriptApiModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    Item *childItem = static_cast<Item*>(index.internalPointer());
    Item *parentItem = childItem->parent();

    if (parentItem == rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int ScriptApiModel::rowCount(const QModelIndex &parent) const
{
    Item *parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = rootItem;
    }
    else {
        parentItem = static_cast<Item*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int ScriptApiModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return static_cast<Item*>(parent.internalPointer())->columnCount();
    }
    else {
        return rootItem->columnCount();
    }
}

QVariant ScriptApiModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    Item *item = static_cast<Item*>(index.internalPointer());
    if (role == DocumentRole) {
        return QVariant::fromValue<QString>(item->document());
    }
    if (role == AnchorRole) {
        return QVariant::fromValue<QString>(item->anchor());
    }
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    return item->data(index.column());
}

Qt::ItemFlags ScriptApiModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ScriptApiModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return rootItem->data(section);
    }

    return QVariant();
}

void ScriptApiModel::setupModelData(QList<Object*> dataList, Item *parent)
{
    foreach (Object *object, dataList) {
        QList<QVariant> columnData;
        columnData << object->id();
        Item *objectItem = new Item(columnData, parent);
        objectItem->setDocumentAnchor(object->apiDocumentIdentifier(), QString());
        parent->appendChild(objectItem);

        QList<QVariant> propertyColumnData;
        propertyColumnData << i18n("Properties");
        Item *propertyContainer = new Item(propertyColumnData, objectItem);
        propertyContainer->setDocumentAnchor(object->apiDocumentIdentifier(), "properties");
        objectItem->appendChild(propertyContainer);
        foreach (Property *property, object->properties()) {
            QList<QVariant> columnData;
            columnData << property->name();
            Item *propertyItem = new Item(columnData, propertyContainer);
            propertyItem->setDocumentAnchor(object->apiDocumentIdentifier(),
                property->apiDocumentAnchor());
            propertyContainer->appendChild(propertyItem);
        }

        QList<QVariant> methodColumnData;
        methodColumnData << i18n("Methods");
        Item *methodContainer = new Item(methodColumnData, objectItem);
        methodContainer->setDocumentAnchor(object->apiDocumentIdentifier(), "methods");
        objectItem->appendChild(methodContainer);
        foreach (Method *method, object->methods()) {
            QList<QVariant> columnData;
            columnData << method->name();
            Item *methodProperty = new Item(columnData, methodContainer);
            methodProperty->setDocumentAnchor(object->apiDocumentIdentifier(),
                method->apiDocumentAnchor());
            methodContainer->appendChild(methodProperty);
        }
    }
}
