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


#include "DataType.h"
#include "DataStructure.h"
#include "DocumentManager.h"
#include <boost/weak_ptr.hpp>

#include <QDebug>
#include <KLocalizedString>
#include <QPainter>
#include <QSvgRenderer>

class DataTypePrivate
{
public:
    struct Property {
        QString name;
        QVariant defaultValue;
        bool visible;
    };

    DataTypePrivate() :
        _propertyList(),
        _propertyDisplayList(),
        _name(i18n("Element")),
        _icon("rocs_default"),
        _defaultColor(QColor("black")),
        _visibility(true),
        _document(0)
    {
    }

    boost::weak_ptr<DataType> q; // self pointer

    QMap<QString, Property> _propertyList;
    QList<QString> _propertyDisplayList;
    QString _name;
    QString _icon;
    QColor _defaultColor;
    int _identifier;
    bool _visibility;
    Document* _document;
};


DataTypePtr DataType::create(Document* document, int identifier)
{
    DataTypePtr pi(new DataType(document, identifier));
    pi->d->q = pi;
    pi->addProperty("name", "");
    return pi;
}


DataType::DataType(Document* document, int identifier):
    d(new DataTypePrivate())
{
    d->_identifier = identifier;
    d->_document = document;
}


DataType::~DataType()
{
}


int DataType::identifier() const
{
    return d->_identifier;
}


void DataType::setName(const QString& name)
{
    d->_name = name;
    emit nameChanged(d->_name);
}


QString DataType::name() const
{
    return d->_name;
}


void DataType::setIcon(const QString& icon)
{
    d->_icon = "rocs_" + icon;
    emit iconChanged(d->_icon);
}


QIcon DataType::icon() const
{
    // create icon for data type
    if (!d->_document->iconPackage().isEmpty()) {
        QFile svgFile(d->_document->iconPackage());
        svgFile.open(QIODevice::ReadOnly | QIODevice::Text);

        QXmlStreamReader reader(&svgFile);
        QSvgRenderer *renderer = DocumentManager::self().sharedRenderer(svgFile.fileName());
        while (!reader.atEnd()) {
            reader.readNext();
            if (!reader.attributes().hasAttribute("id")) {
                continue;
            }
            QString attribute = reader.attributes().value("id").toString();
            if (attribute.startsWith(d->_icon)) {
                QImage iconImage = QImage(80, 80, QImage::Format_ARGB32);

                QPainter painter;
                painter.begin(&iconImage);
                renderer->render(&painter, attribute);
                painter.end();

                attribute.remove("rocs_");
                return QIcon(QPixmap::fromImage(iconImage));
            }
        }
    }
    return QIcon();
}


QString DataType::iconName() const
{
    return d->_icon;
}


void DataType::setVisible(bool visible)
{
    d->_visibility = visible;
    emit(visibilityChanged(visible));
}


bool DataType::isVisible() const
{
    return d->_visibility;
}


void DataType::setDefaultColor(const QColor& color)
{
    d->_defaultColor = color;
    emit defaultColorChanged(color);
}

QColor DataType::defaultColor() const
{
    return d->_defaultColor;
}

void DataType::addProperty(const QString& name, const QString& defaultValue)
{
    DataTypePrivate::Property newProperty;
    newProperty.name = name;
    newProperty.defaultValue = defaultValue;
    newProperty.visible = true;
    d->_propertyList.insert(name, newProperty);
    d->_propertyDisplayList.append(name);
    emit(propertyAdded(newProperty.name, newProperty.defaultValue));
}

void DataType::removeProperty(const QString& name)
{
    d->_propertyDisplayList.removeOne(name);
    d->_propertyList.remove(name);
    emit propertyRemoved(name);
}

void DataType::renameProperty(const QString& oldName, const QString& newName)
{
    if (d->_propertyList.contains(newName)) {
        qWarning() << "Property name was not changed: new name already exists";
        return;
    }
    int index = d->_propertyDisplayList.indexOf(oldName);
    if (index >= 0) {
        d->_propertyDisplayList.removeAt(index);
        d->_propertyDisplayList.insert(index, newName);
    }
    d->_propertyList.insert(newName, d->_propertyList[oldName]);
    d->_propertyList.remove(oldName);
    emit propertyRenamed(oldName, newName);
}

QStringList DataType::properties() const
{
    return QStringList(d->_propertyList.keys());
}

QVariant DataType::propertyDefaultValue(const QString& name) const
{
    if (!d->_propertyList.contains(name)) {
        return false;
    }
    return d->_propertyList[name].defaultValue;
}


void DataType::setPropertyDefaultValue(const QString& name, const QVariant& value)
{
    if (!d->_propertyList.contains(name)) {
        return;
    }
    d->_propertyList[name].defaultValue = value;
    emit propertyDefaultValueChanged(name);
}


bool DataType::isPropertyVisible(const QString& name) const
{
    if (!d->_propertyList.contains(name)) {
        return false;
    }
    return d->_propertyList[name].visible;
}

void DataType::setPropertyVisible(const QString& name, bool visible)
{
    if (!d->_propertyList.contains(name)) {
        return;
    }
    d->_propertyList[name].visible = visible;
    emit propertyVisibilityChanged(name);
}

void DataType::remove()
{
    emit removed();
    disconnect();
}
