#include "model_GraphProperties.h"

GraphPropertiesModel::GraphPropertiesModel( QObject *parent ) : QAbstractTableModel(parent){
  _dataSource = 0;
  _metaObject = 0;
}

int GraphPropertiesModel::rowCount(const QModelIndex &parent) const{
  Q_UNUSED(parent);
  if (_dataSource == 0){
    return 0;
  }
  return _metaObject->propertyCount();
}

int GraphPropertiesModel::columnCount ( const QModelIndex & parent ) const{
  Q_UNUSED(parent);
  return 2;
}
    
QVariant GraphPropertiesModel::data(const QModelIndex &index, int role) const{
  if ( !index.isValid() ){
    return QVariant();
  }
  if ( index.row() >= _metaObject->propertyCount() ){
    return QVariant();
  }
  if ( role != Qt::DisplayRole ){
    return QVariant();
  }
  
  
  if (index.column() == 0){
    return _metaObject->property(index.row()).name();
  }
  else if (index.column() == 1){
     return _dataSource->property(_metaObject->property(index.row()).name());
  }
  return QVariant();
}

QVariant GraphPropertiesModel::headerData(int section, Qt::Orientation orientation, int role) const{
  if (role != Qt::DisplayRole){
    return QVariant();
  }

  if (orientation == Qt::Horizontal) {
    switch (section) {
      case 0:  return tr("Property");
      case 1:  return tr("Value");
      default: return QVariant();
    }
  }
  return QVariant();
}

void GraphPropertiesModel::setDataSource(QObject *dataSource){
  if (dataSource == 0){
    int count = rowCount();
    if (count == 0) return;
    beginRemoveRows(QModelIndex(), 0, count-1);
    endRemoveRows();
    return;
  }

  if( _dataSource){
    beginRemoveRows(QModelIndex(), 0, _metaObject->propertyCount()-1);
    endRemoveRows();
  }
  _dataSource = dataSource;
  _metaObject = _dataSource -> metaObject();
 
 beginInsertRows(QModelIndex(), 0, _metaObject->propertyCount()-1);
 endInsertRows();
 
}