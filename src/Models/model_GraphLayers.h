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
#ifndef MODEL_GRAPHLAYERS_H
#define MODEL_GRAPHLAYERS_H

#include <QAbstractListModel>

class QModelIndex;

class GraphDocument;
class Graph;


class GraphLayersModel : public QAbstractListModel
{
  Q_OBJECT
  public:
      GraphLayersModel( GraphDocument *document, QObject *parent = 0);
      int rowCount(const QModelIndex &parent = QModelIndex()) const;
      QVariant data(const QModelIndex &index, int role) const;
      QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
      Qt::ItemFlags flags(const QModelIndex& index) const;
      bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

      bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
      bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

       Graph *at(const QModelIndex& index);

  private slots:
    void update( Graph *g);
    void connectGraphSignals( Graph *g);
  private:
     GraphDocument *_document;
  
};

#endif