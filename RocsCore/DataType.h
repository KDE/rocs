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

#include <QString>
#include <QVariant>
#include <QColor>
#include <QIcon>

#include "RocsCoreExport.h"
#include "CoreTypes.h"

class Data;
class DataTypePrivate;
class DataStructure;
class Document;

/**
* \class DataType
*
* A data type groups a set of data elements and gives them a common appearance. Each data type is
* associated to a document and must get an unique identifier on creation. To create a data type,
* use the static \see create(document, identifier) method.
*/
class ROCSLIB_EXPORT DataType : public QObject
{
    Q_OBJECT
public:
    /**
     * Create data type objects.
     *
     * \param document is graph document for that the data type is created
     * \param identifier is unique identifier of the data type within the document
     * \return the created data type
     */
    static DataTypePtr create(Document* document, int identifier);

    /**
     * Default destructor.
     * DO NOT CALL IT, let the shared pointer take care for deletion.
     */
    virtual ~DataType();

    /**
     * \return name of data type
     */
    QString name() const;

    /**
     * \return data type icon identifier for iconPackage
     */
    QString iconName() const;

    /**
     * Generates QIcon for data type at run time. This icon is not cached.
     *
     * \return icon according to current iconPackage and iconName
     */
    QIcon icon() const;

    /**
     * \return unique identifier of data type
     */
    int identifier() const;

    /**
     * \return default color for data elements of this data type
     */
    QColor defaultColor() const;

    /**
     * Add a new dynamic property to all data elements of this data type.
     *
     * \param name is the unique name of the property
     * \param defaultValue is the default value that is set initially
     */
    void addProperty(const QString& name, const QString& defaultValue = QString());

    /**
     * Remove dynamic property from data type.
     */
    void removeProperty(const QString& name);

    /**
     * Rename property from \p oldName to \p newName. The old
     * property name at all data elements is changed with this change.
     * The property is not changed, if \p newName already exists at this
     * data type.
     *
     * \param oldName is the current property name
     * \param newName is the new property name
     */
    void renameProperty(const QString& oldName, const QString& newName);

    /**
     * \return list of all properties, ordered in display order
     */
    QStringList properties() const;

    /**
     * Set default value of property \p name to \p value.
     */
    void setPropertyDefaultValue(const QString& name, const QVariant& value);

    /**
     * \return default value for property \p name
     */
    QVariant propertyDefaultValue(const QString& name) const;

    /**
     * Set default value of property \p name to \p visible.
     */
    void setPropertyVisible(const QString& name, bool visible);

    /**
     * \return true if this property is visible, otherwise false.
     */
    bool isPropertyVisible(const QString& name) const;

    /**
     * \return true if data type is visible, otherwise false.
     */
    bool isVisible() const;


public slots:
    /**
     * Set name of the data type.
     *
     * \param name of the data type
     */
    void setName(const QString& name);

    /**
     * Set icon of data elements of this type.
     * "rocs_" prefix of icon identifier must be removed before setting this.
     *
     * \param icon identifier in current icon package
     */
    void setIcon(const QString& icon);

    /**
     * Set visibility for all data elements of this type.
     *
     * \param visible is the visibility value to be set
     */
    void setVisible(bool visible);

    /**
     * Set default color value for this data type.
     *
     * \param color is the default color to be set
     */
    void setDefaultColor(const QColor& color);

    /**
     * Unregister data type from graph document.
     */
    void remove();

signals:
    /**
     * Emitted when data type is removed.
     */
    void removed();

    /**
     * Emitted when default color was changed.
     */
    void defaultColorChanged(const QColor& color);

    /**
     * Emitted when icon was changed.
     */
    void iconChanged(const QString& icon);

    /**
     * Emitted when name was changed.
     */
    void nameChanged(const QString& name);

    /**
     * Emitted when new property was created.
     */
    void propertyAdded(const QString& name, const QVariant& defaultValue);

    /**
     * Emitted when property was removed.
     */
    void propertyRemoved(const QString& name);

    /**
     * Emitted when default value of property is changed.
     */
    void propertyDefaultValueChanged(const QString& name);

    /**
     * Emitted when the visibility was changed.
     */
    void propertyVisibilityChanged(const QString& name);

    /**
     * Emitted when the visibility was changed.
     */
    void visibilityChanged(bool visible);

    /**
     * Emitted when a property was renamed.
     */
    void propertyRenamed(const QString& oldName, const QString& newName);

protected:
    /**
     * \internal
     * Default constructor. To create DataType elements use \see DataType::create(...).
     *
     * \param document is graph document for that the data type is created
     * \param identifier is unique identifier of the data type within the document
     */
    DataType(Document *document, int identifier);

private:
    const QScopedPointer<DataTypePrivate> d;
    DataType(const DataType &);
    DataType& operator=(const DataType&);
};

#endif // DATATYPE_H
