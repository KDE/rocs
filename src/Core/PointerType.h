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


#ifndef POINTERTYPE_H
#define POINTERTYPE_H

#include <QObject>
#include <QtScript>
#include <QString>
#include <QColor>

#include "rocslib_export.h"
#include "Rocs_Typedefs.h"

class Data;
class PointerTypePrivate;
class DataStructure;

/**
* \class PointerType
*
* This class descibes a pointer types.
*/


class ROCSLIB_EXPORT PointerType : public QObject
{
    Q_OBJECT

public:
    static PointerTypePtr create(DataStructurePtr parent, int identifier);
    ~PointerType();

    QString name() const;
    int identifier() const;
    bool isNameVisible() const;
    bool isValueVisible() const;
    bool isVisible() const;
    QColor defaultColor() const;

public slots:
    void setName(QString name);
    void setNameVisible(bool visible);
    void setValueVisible(bool visible);
    void setVisible(bool visible);
    void setDefaultColor(QColor color);

protected:
    /** Default constructor. To create PointerType elements use \see PointerType::create(...).
     *\param parent is the parent DataStructure
     *\param identifier is the unique identifier for this type
     */
    PointerType(DataStructurePtr dataStructure, int identifier);

private:
    boost::shared_ptr<PointerTypePrivate> const d;
};

#endif // POINTERTYPE_H
