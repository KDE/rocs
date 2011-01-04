/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "model_GraphLayers.h"
#include "Document.h"
#include "DataStructure.h"
#include <QString>
#include <QModelIndex>
#include <KDebug>

DataStructureLayersModel::DataStructureLayersModel( Document *document, QObject *parent)
        : QAbstractListModel( parent ) {
    _document = document;
}

int DataStructureLayersModel::rowCount(const QModelIndex&) const {
    if ( _document == 0) return 0;
    return _document -> dataStructures().size();
}

QVariant DataStructureLayersModel::data(const QModelIndex &index, int role) const {
    if ( _document == 0) {
        return 0;
    }
    if ( ( !index.isValid() ) || ( index.row() > _document ->dataStructures().size() ) || ( role != Qt::DisplayRole) ) {
        return QVariant();
    }

    return _document->dataStructures().at(index.row())->property("name");
}

QVariant DataStructureLayersModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if ( _document == 0) {
        return QVariant();
    }
    if ( role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Horizontal) {
        return QString("Column %1").arg(section);
    }

    return QString("Row %1").arg(section);

}

Qt::ItemFlags DataStructureLayersModel::flags(const QModelIndex& index) const {
    if ( !index.isValid() ) {
        return Qt::ItemIsEnabled;
    }
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool DataStructureLayersModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if ( index.isValid() && (role == Qt::ItemIsEditable)) {
        DataStructure *g = _document->dataStructures().at(index.row());
        g-> setProperty("name",value.toString());
        return true;
    }
    return false;
}

bool DataStructureLayersModel::insertRows(int position, int , const QModelIndex&) {
    if ( _document == 0) return false;

    beginInsertRows(QModelIndex(), position, position);
    _document->addDataStructure(i18n("Untitled%1", rowCount()));
    kDebug() << "Called!";
    endInsertRows();
    return true;
}

bool DataStructureLayersModel::removeRows(int position, int rows, const QModelIndex&) {
    if (_document == 0) return false;
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    _document->dataStructures().removeAt(position);
    endRemoveRows();
    return true;
}

DataStructure *DataStructureLayersModel::at(const QModelIndex& index)
{
    if (_document->dataStructures().size() == 0) return 0;
    return _document->dataStructures().at(index.row());
}
