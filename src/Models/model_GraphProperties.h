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

#ifndef MODEL_GRAPHPROPERTIES_H
#define MODEL_GRAPHPROPERTIES_H

#include <QObject>
#include <QAbstractTableModel>
#include <QMetaObject>
#include <QModelIndex>
#include <QVariant>

class GraphPropertiesModel : public QAbstractTableModel
{
  Q_OBJECT
public:
    /*! Default Constructor.
      \param parent the QObject that owns this Model.   */
    explicit GraphPropertiesModel(QObject *parent = 0);

    /*! Gives the number of rows on the model.
      \param parent a default constructed, unused, QModelIndex.
      \return the number of rows on the model. */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /*! Gives the number or columns on the model.
     \param parent a default constructed, unused, QModelIndex.
     \return the const value 2. */
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;

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

    /*! recreates the information on the model based on another datasource.
    \param dataSource the new dataSource of the model.*/
    void setDataSource(QObject *dataSource);

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    
    Qt::ItemFlags flags(const QModelIndex &index) const;
    
    /**Add properti to data source and insert a new row,*/
    void addDynamicProperty(QString name, QVariant value, QObject *obj, bool isGlobal);

private:
    QObject *_dataSource;
    const QMetaObject *_metaObject;
};

#endif
