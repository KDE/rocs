/*  
    This file is part of Rocs.
    Copyright 2008  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008  Ugo Sangiori <ugorox@gmail.com>

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
#include "model_GraphDocument.h"
#include "Document.h"
#include "DataStructure.h"
#include <QString>
#include <KDebug>
#include <QModelIndex>

DocumentModel::DocumentModel(QList< Document*>* documents, QObject *parent)
        : QAbstractListModel( parent ), _documents( (*documents) ) {

}

int DocumentModel::rowCount(const QModelIndex&) const {
    return _documents.size();
}

QVariant DocumentModel::data(const QModelIndex &index, int role) const {
    if (( !index.isValid() ) || ( index.row() > _documents.size() ) || ( role != Qt::DisplayRole)) {
        return QVariant();
    }
    return _documents.at(index.row())->name();
}

QVariant DocumentModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if ( role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Horizontal) {
        return QString("Column %1").arg(section);
    }

    return QString("Row %1").arg(section);

}

Qt::ItemFlags DocumentModel::flags(const QModelIndex& index) const {
    if ( !index.isValid() ) {
        return Qt::ItemIsEnabled;
    }
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool DocumentModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if ( index.isValid() && (role == Qt::ItemIsEditable)) {
        _documents.at(index.row())-> setName(value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool DocumentModel::insertRows(int position, int rows, const QModelIndex&) {
    beginInsertRows(QModelIndex(), position, position+rows-1);

    Document *doc = new  Document("untitled");
    _documents.append(doc);
    endInsertRows();
    return true;
}

bool DocumentModel::removeRows(int position, int rows, const QModelIndex&) {
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    _documents.removeAt(position);
    endRemoveRows();
    return true;
}

Document *DocumentModel::at(const QModelIndex& index) {
    return _documents.at(index.row());
}

