/*
    This file is part of Rocs.
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef DATATYPE_H
#define DATATYPE_H

#include <QObject>
#include <QtScript>
#include <QString>
#include <QColor>

#include "rocslib_export.h"
#include "Rocs_Typedefs.h"

class Data;
class DataTypePrivate;
class DataStructure;

/**
* \class DataType
*
* This class descibes a pointer types.
*/


class ROCSLIB_EXPORT DataType : public QObject {
    Q_OBJECT

public:
    static DataTypePtr create(DataStructurePtr dataStructure, int identifier);
    ~DataType();

    QString name() const;
    int identifier() const;
    bool isNameVisible() const;
    bool isValueVisible() const;
    QColor defaultColor() const;

public slots:
    void setName(QString name);
    void setNameVisible(bool visible);
    void setValueVisible(bool visible);
    void setDefaultColor(QColor color);

protected:
    /** Default constructor. To create DataType elements use \see DataType::create(...).
     *\param parent is the parent DataStructure
     *\param identifier is the unique identifier for this type
     */
    DataType(DataStructurePtr dataStructure, int identifier);

private:
    boost::shared_ptr<DataTypePrivate> const d;
};

#endif // DATATYPE_H
