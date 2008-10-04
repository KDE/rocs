#include "model_GraphLayers.h"
#include "libgraph_GraphDocument.h"
#include "libgraph_Graph.h"
#include <QString>
#include <QDebug>
#include <QModelIndex>

GraphLayersModel::GraphLayersModel(libgraph::GraphDocument *document, QObject *parent)
  : QAbstractListModel( parent ){
  _document = document;
}

int GraphLayersModel::rowCount(const QModelIndex &parent) const{
  if ( _document == 0) return 0;
  return _document -> size();
}

QVariant GraphLayersModel::data(const QModelIndex &index, int role) const{
  if ( _document == 0) return 0;
  if ( !index.isValid() ) return QVariant();
  if ( index.row() > _document -> size()) return QVariant();
  if ( role != Qt::DisplayRole) return QVariant();

  return _document->at(index.row())->toString();
}

QVariant GraphLayersModel::headerData(int section, Qt::Orientation orientation, int role) const{
  if ( _document == 0) return QVariant();
  if ( role != Qt::DisplayRole) return QVariant();
  if (orientation == Qt::Horizontal) return QString("Column %1").arg(section);
  else return QString("Row %1").arg(section);
}

Qt::ItemFlags GraphLayersModel::flags(const QModelIndex& index) const{
  if ( !index.isValid() ) return Qt::ItemIsEnabled;
  return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool GraphLayersModel::setData(const QModelIndex& index, const QVariant& value, int role){
  if ( index.isValid() && (role == Qt::ItemIsEditable)) {
    _document->at(index.row())-> setName(value.toString());
    emit dataChanged(index, index);
    return true;
  }
  return false;
}

bool GraphLayersModel::insertRows(int position, int rows, const QModelIndex &index){
  if ( _document == 0) return false;

  beginInsertRows(QModelIndex(), position, position+rows-1);  
  _document->addGraph("untitled");  
  endInsertRows();
  return true;
}

bool GraphLayersModel::removeRows(int position, int rows, const QModelIndex &index){
    if (_document == 0) return false;
     beginRemoveRows(QModelIndex(), position, position+rows-1);
    _document->removeAt(position);
     endRemoveRows();
     return true;
}
