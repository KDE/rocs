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

#include <QString>
#include <QColor>

#include "rocslib_export.h"
#include "Rocs_Typedefs.h"

class Data;
class PointerTypePrivate;
class DataStructure;
class Document;

/**
* \class PointerType
*
* A pointer type groups a set of pointers and gives them a common appearance. Each pointer type is
* associated to a document and must get an unique identifier on creation. To create a pointer type,
* use the static \see create(document, identifier) method.
*/
class ROCSLIB_EXPORT PointerType : public QObject
{
    Q_OBJECT
public:
    /**
     * Create pointer type objects.
     *
     * \param document is graph document for that the pointer type is created
     * \param identifier is unique identifier of the pointer type within the document
     * \return the created pointer type
     */
    static PointerTypePtr create(Document* parent, int identifier);

    /**
     * Destructor.
     */
    ~PointerType();

    /**
     * \return name of pointer type
     */
    const QString& name() const;

    /**
     * \return unique identifier of pointer type
     */
    int identifier() const;

    /**
     * \return default color for data elements of this pointer type
     */
    const QColor& defaultColor() const;

public slots:
    /**
     * Set name of the pointer type.
     *
     * \param name of the pointer type
     */
    void setName(QString name);

    /**
     * Set default color value for this pointer type.
     *
     * \param color is the default color to be set
     */
    void setDefaultColor(QColor color);

signals:
    /**
     * Emitted when name was changed.
     */
    void nameChanged(const QString& name);

    /**
     * Emitted when default color was changed.
     */
    void colorChanged(const QColor& color);

protected:
    /**
     * \internal
     * Default constructor. To create PointerType elements use \see PointerType::create(...).
     *
     * \param document is graph document for that the pointer type is created
     * \param identifier is unique identifier of the pointer type within the document
     */
    PointerType(Document* document, int identifier);

private:
    boost::shared_ptr<PointerTypePrivate> const d;
};

#endif // POINTERTYPE_H
