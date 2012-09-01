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
#include <KIcon>

#include "rocslib_export.h"
#include "Rocs_Typedefs.h"

class Data;
class DataTypePrivate;
class DataStructure;
class Document;

/**
* \class DataType
*
* This class describes data type elements.
*/


class ROCSLIB_EXPORT DataType : public QObject
{
    Q_OBJECT

public:
    static DataTypePtr create(Document* document, int identifier);
    ~DataType();

    const QString& name() const;
    QString iconName() const;

    /**
     * Generates KIcon for data type at run time. This icon is not cached.
     * \return icon according to current iconPackage and iconName
     */
    KIcon icon() const;
    int identifier() const;
    QColor defaultColor() const;

public slots:
    void setName(QString name);

    /**
     * Set icon of data elements of this type.
     * "rocs_" prefix of icons must be removed before setting this.
     */
    void setIcon(QString icon);
    void setDefaultColor(QColor color);
    void remove();

signals:
    void defaultColorChanged(QColor color);
    void iconChanged(const QString& icon);
    void nameChanged(const QString& name);

protected:
    /** Default constructor. To create DataType elements use \see DataType::create(...).
     *\param parent is the parent Document
     *\param identifier is the unique identifier for this type
     */
    DataType(Document* document, int identifier);

private:
    boost::shared_ptr<DataTypePrivate> const d;
};

#endif // DATATYPE_H
