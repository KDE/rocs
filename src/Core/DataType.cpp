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
#include <boost/weak_ptr.hpp>

#include <QPainter>
#include <QSvgRenderer>
#include <DataItem.h>
#include <boost/graph/graph_concepts.hpp>

class DataTypePrivate
{
public:
    struct Property {
        QString name;
        QVariant defaultValue;
        bool visible;
    };

    DataTypePrivate() {}
    boost::weak_ptr<DataType> q; // self pointer

    QList<Property> _properties;
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
    d->_defaultColor = QColor("blue");
    d->_icon = "rocs_default";
    d->_name = i18n("Element");
    d->_document = document;
}


DataType::~DataType()
{
}


int DataType::identifier() const
{
    return d->_identifier;
}


void DataType::setName(QString name)
{
    d->_name = name;
    emit nameChanged(d->_name);
}


const QString& DataType::name() const
{
    return d->_name;
}


void DataType::setIcon(QString icon)
{
    d->_icon = "rocs_" + icon;
    emit iconChanged(d->_icon);
}


KIcon DataType::icon() const
{
    // create icon for data type
    if (!d->_document->iconPackage().isEmpty()) {
        QFile svgFile(d->_document->iconPackage());
        svgFile.open(QIODevice::ReadOnly | QIODevice::Text);

        QXmlStreamReader reader(&svgFile);
        QSvgRenderer *renderer = DataItem::sharedRenderer(svgFile.fileName());
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
                return KIcon(QPixmap::fromImage(iconImage));
            }
        }
    }
    return KIcon();
}


const QString& DataType::iconName() const
{
    return d->_icon;
}


void DataType::setDefaultColor(QColor color)
{
    d->_defaultColor = color;
    emit defaultColorChanged(color);
}


const QColor& DataType::defaultColor() const
{
    return d->_defaultColor;
}

void DataType::addProperty(QString name, QString defaultValue)
{
    DataTypePrivate::Property newProperty;
    newProperty.name = name;
    newProperty.defaultValue = defaultValue;
    newProperty.visible = true;
    d->_properties.append(newProperty);
    emit(propertyAdded(newProperty.name, newProperty.defaultValue));
}

QList<QString> DataType::properties() const
{
    QList<QString> properties;
    QList<DataTypePrivate::Property>::const_iterator iter = d->_properties.constBegin();
    while (iter != d->_properties.constEnd()) {
        properties.append(iter->name);
        ++iter;
    }
    return properties;
}

QVariant DataType::propertyDefaultValue(QString name) const
{
    //FIXME very inefficient with this implementation, but working
    QList<QString> properties;
    QList<DataTypePrivate::Property>::const_iterator iter = d->_properties.constBegin();
    while (iter != d->_properties.constEnd()) {
        if (name == iter->name) {
            return iter->defaultValue;
        }
    }
    return false;
}

bool DataType::isPropertyVisible(QString name) const
{
    //FIXME very inefficient with this implementation, but working
    QList<QString> properties;
    QList<DataTypePrivate::Property>::const_iterator iter = d->_properties.constBegin();
    while (iter != d->_properties.constEnd()) {
        if (name == iter->name) {
            return iter->visible;
        }
    }
    return false;
}

void DataType::setPropertyVisible(QString name, bool visible)
{
    //FIXME very inefficient with this implementation, but working
    QList<QString> properties;
    QList<DataTypePrivate::Property>::iterator iter = d->_properties.begin();
    while (iter != d->_properties.end()) {
        if (name == iter->name) {
            iter->visible = visible;
            return;
        }
    }
}

void DataType::remove()
{
    d->_document->removeDataType(identifier());
}
