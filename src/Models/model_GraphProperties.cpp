/*
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>

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

#include "model_GraphProperties.h"
#include <KLocale>
#include <KDebug>
#include <DynamicPropertiesList.h>
#include "Pointer.h"
#include "Data.h"
#include "DataStructure.h"

GraphPropertiesModel::GraphPropertiesModel(QObject *parent) : QAbstractTableModel(parent)
{
    // start all pointers to zero, so we don't crash things.
    _dataSource = 0;
    _metaObject = 0;
}

int GraphPropertiesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    // if there's no dataSource, there's no data. return zero.
    // else return the size of properties of the object.
    if (_dataSource == 0) {
        return 0;
    }
    return _dataSource->dynamicPropertyNames().size();
}

int GraphPropertiesModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return 3; //Name - Value - Type
}

QVariant GraphPropertiesModel::data(const QModelIndex &index, int role) const
{
    // error conditions, return a default value constructed QVariant().
    if (!index.isValid()) {
        return QVariant();
    }
    if (index.row() >= _dataSource->dynamicPropertyNames().size()) {
        return QVariant();
    }
    if (role != Qt::DisplayRole && role != Qt::EditRole) {
        return QVariant();
    }

    const char* propertyName = _dataSource->dynamicPropertyNames()[index.row()];

    return   (index.column() == 0) ? propertyName
           : (index.column() == 1) ?  _dataSource->property(propertyName)
           : (index.column() == 2) ? DynamicPropertiesList::New()->typeInText(_dataSource, propertyName)
           : QVariant();

}

QVariant GraphPropertiesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // if the role is not for displaying anything, return a default empty value.
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return i18n("Property");
        case 1: return i18n("Value");
        case 2: return i18n("Type");
        }
    }
    return QVariant();
}

void GraphPropertiesModel::setDataSource(QObject *dataSource)
{
    // if there isn't any datasource being send, just remove everything from the model and exit.
    if (dataSource == 0) {
        int count = rowCount();
        if (count == 0) return;
        beginRemoveRows(QModelIndex(), 0, count - 1);
        endRemoveRows();
        return;
    }

    // clear the model for the new data.
    if (_dataSource) {
        beginRemoveRows(QModelIndex(), 0, _dataSource->dynamicPropertyNames().size() - 1);
        endRemoveRows();
    }

    // set some needed variables.
    _dataSource = dataSource;
    _metaObject = _dataSource -> metaObject();

    // insert the information.

    if (dataSource->dynamicPropertyNames().size() > 0) {
        beginInsertRows(QModelIndex(), 0, dataSource->dynamicPropertyNames().size() - 1);
        endInsertRows();
    }


}

Qt::ItemFlags GraphPropertiesModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        if (index.column() != 2) {//Can't change type for now
            return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
        } else {
            return QAbstractItemModel::flags(index);
        }
    }
    return Qt::ItemIsEnabled;
}

bool GraphPropertiesModel::setData(const QModelIndex &index, const QVariant &value,  int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        const char* propertyName = _dataSource->dynamicPropertyNames()[index.row()];
        if (index.column() == 0 && value.toString() == QString(propertyName) ){
            return false;
        }

        switch (index.column()) {
        case 0: DynamicPropertiesList::New()->changePropertyName(QString(_dataSource->dynamicPropertyNames()[index.row()]), value.toString(), _dataSource);   break;
        case 1:  _dataSource->setProperty(_dataSource->dynamicPropertyNames()[index.row()], value); break; /* just change the values */
        default: kDebug() << "shoudn't enter here";   return false;
        }

        emit dataChanged(index, index);
        return true;

    }
    return false;

}

void GraphPropertiesModel::addDynamicProperty(QString name, QVariant value, QObject *obj)
{
    // Need check if the property already exists
    bool insertingRow = false;
    if (name.isEmpty()) {
        kWarning() << "Cannot add an empty property";
        return;
    }

    if (! obj->dynamicPropertyNames().contains(name.toUtf8())) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        insertingRow = true;
    }

    if (Pointer * pointer = qobject_cast<Pointer*> (obj)) {
        pointer->addDynamicProperty(name, value);
    }
    if (Data * datum = qobject_cast<Data*> (obj)) {
        datum->addDynamicProperty(name, value);
    }
    if (DataStructure * dataStructure = qobject_cast<DataStructure*> (obj)) {
        dataStructure->addDynamicProperty(name, value);
    }

    if (insertingRow) { /* if inserting, need finish*/
        endInsertRows();
    }
}
