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
#include "graphDocument.h"
#include "graph.h"
#include <QString>
#include <QDebug>
#include <QModelIndex>
#include <KDebug>

GraphLayersModel::GraphLayersModel( GraphDocument *document, QObject *parent)
  : QAbstractListModel( parent ){
  _document = document;
  connect( _document, SIGNAL( graphCreated( Graph*)), 
	  this, SLOT( connectGraphSignals( Graph*)));

  kDebug () << "GraphCreated signal connected with connect graphSignals";
}

int GraphLayersModel::rowCount(const QModelIndex&) const{
  if ( _document == 0) return 0;
  return _document -> size();
}

QVariant GraphLayersModel::data(const QModelIndex &index, int role) const{
  if ( _document == 0){
    return 0;
  }
  if ( ( !index.isValid() ) || ( index.row() > _document -> size() ) || ( role != Qt::DisplayRole) ){
    return QVariant();
  }

  return _document->at(index.row())->property("name");
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
     Graph *g = _document->at(index.row());
    g-> setProperty("name",value.toString());
  }
  return false;
}

bool GraphLayersModel::insertRows(int position, int type, const QModelIndex&){
  if ( _document == 0) return false;

  beginInsertRows(QModelIndex(), position, position);

  Graph* g =  _document->addGraph("Untitled");
  g->setDirected( (type == 0)? false : true );
  endInsertRows();
  return true;
}

bool GraphLayersModel::removeRows(int position, int rows, const QModelIndex&){
    if (_document == 0) return false;
     beginRemoveRows(QModelIndex(), position, position+rows-1);
    _document->removeAt(position);
     endRemoveRows();
     return true;
}

 Graph *GraphLayersModel::at(const QModelIndex& index)
{
  return _document->at(index.row());
}

void GraphLayersModel::update( Graph *g){
  kDebug() << "Tryed to change the name of the graph on the model";
  int i = _document->indexOf(g);
  QModelIndex index = QAbstractItemModel::createIndex(i, 0);
  emit dataChanged(index, index);
}

void GraphLayersModel::connectGraphSignals( Graph *g){
  kDebug () << "connecting Graph Signals with GraphLayer Model";
  connect(g, SIGNAL(nameChanged( Graph*)), this, SLOT(update( Graph*)));
}

