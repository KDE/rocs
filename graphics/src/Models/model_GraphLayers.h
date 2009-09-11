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
    /*! Default Constructor.
    \param document the GraphDocument that will populate the model.
    \param parent the QObject that owns this Model.   */
    explicit GraphLayersModel( GraphDocument *document, QObject *parent = 0);

    /*! Gives the number of rows on the model.
       \param parent a default constructed, unused, QModelIndex.
       \return the number of rows on the model. */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /*! Return the data of a item on the model.
    \param index the position of the item.
    \param role the kind of data that you're trying to get.
    \return the QVariant of the data.*/
    QVariant data(const QModelIndex &index, int role) const;

    /*! some information that goes on the header.
    \param section the place where the data will be.
    \param orientation Horizontal or Vertical.
    \param role defaulted to DisplayRole for displaying things.
    \return a QVariant containing the string of the header. */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    /*! return the flags for some item in the model.
    \param index the index of the item
    \return a Qt::ItemFlags of the item. */
    Qt::ItemFlags flags(const QModelIndex& index) const;

    /*! changes the data on a item.
    \param index the index of the item that will have it's data modified.
    \param value the new value of the data.
    \param role defaulted to Edit Role, if it's another, it does nothing.
    \return true if the data was changed, false if it wasn't. */
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

    /*! This does not work as it should, it will insert ONE new row (not rows) to the model.
    \param position the index that the data will be inserted.
    \param type the type of the graph.
    \param QModelIndex The position that the item will be inserted after.
    \return true if success, false otherwise. */
    bool insertRows(int position, int type, const QModelIndex &index = QModelIndex());

    /*! remmove n rows, from position to potion+rows from the model.
    \param position the first position to remove anything.
    \param rows the number of rows to remove.
     \param QModelIndex not used.
    \return true if success, false otherwise. */
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

    /*! select the graph at position index.
    \param index the position of the graph that you want to select.
    \return the graph at position Index.*/
    Graph *at(const QModelIndex& index);

private:
    GraphDocument *_document;

};

#endif
