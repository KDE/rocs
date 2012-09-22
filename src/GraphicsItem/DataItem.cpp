/*
    This file is part of Rocs.
    Copyright 2011       Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DataItem.h"

#include "Data.h"
#include "DataStructure.h"
#include "GraphicsLayout.h"
#include "Interface/ConfigureDefaultProperties.h"

#include <KDebug>
#include <QFont>
#include <QGraphicsColorizeEffect>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QSvgRenderer>

QMap<QString, QSvgRenderer*> DataItem::_sharedRenderers;

QSvgRenderer* DataItem::sharedRenderer(QString iconPackage)
{
    return _sharedRenderers.value(iconPackage);
}


QSvgRenderer* DataItem::registerSharedRenderer(QString iconPackage)
{
    if (!_sharedRenderers.contains(iconPackage)) {
        QSvgRenderer *z = new QSvgRenderer(iconPackage);
        _sharedRenderers.insert(iconPackage, z);
    }
    return _sharedRenderers.value(iconPackage);
}


void DataItem::removeSharedRenderer(QString iconPackage)
{
    _sharedRenderers.value(iconPackage)->deleteLater();
    _sharedRenderers.remove(iconPackage);
}


DataItem::DataItem(DataPtr n)
    : QGraphicsSvgItem(0)
    , _data(n)
    , _colorizer(0)
    , _font(QFont("Helvetica [Cronyx]", 12))
    , _oldStyle(GraphicsLayout::self()->viewStyleDataNode())
    , _item(new QGraphicsItemGroup())
    , _originalWidth(n->width())
    , _width(-1)
    , _oldDataType(n->dataStructure()->document()->dataType(n->dataType()))
{

    connect(n.get(), SIGNAL(removed()), this, SLOT(deleteLater()));
    connect(_oldDataType.get(), SIGNAL(iconChanged(QString)), this, SLOT(updateIcon()));

    connect(n.get(), SIGNAL(propertyAdded(QString)), this, SLOT(registerProperty(QString)));
    connect(n.get(), SIGNAL(propertyRemoved(QString)), this, SLOT(removeProperty(QString)));
    connect(n.get(), SIGNAL(propertyChanged(QString)), this, SLOT(updateProperty(QString)));
    connect(GraphicsLayout::self(), SIGNAL(changed()), this, SLOT(updatePropertyList()));

    connect(n.get(), SIGNAL(colorChanged(QColor)), this, SLOT(updateColor()));
    connect(n.get(), SIGNAL(posChanged(QPointF)), this, SLOT(updatePos()));
    connect(n.get(), SIGNAL(visibilityChanged(bool)), this, SLOT(updateVisibility(bool)));
    connect(n.get(), SIGNAL(useColorChanged(bool)), this, SLOT(updateColor()));
    connect(n.get(), SIGNAL(widthChanged(double)), this, SLOT(updateSize()));
    connect(n.get(), SIGNAL(dataTypeChanged(int)), this, SLOT(setupNode()));

    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setZValue(1);
    setFlag(ItemIsSelectable, true);
    setupNode();
}

DataItem::~DataItem()
{
    foreach (QString name, _propertyValues.keys()) {
        delete _propertyValues[name];
    }
    _propertyValues.clear();
    delete _item;
}

void DataItem::setupNode()
{
    if (_data.get()->dataStructure()->document()->dataType(_data.get()->dataType()) != _oldDataType) {
        disconnect(_oldDataType.get(), SIGNAL(iconChanged(QString)), this, SLOT(updateIcon()));
        _oldDataType = _data.get()->dataStructure()->document()->dataType(_data.get()->dataType());
        connect(_oldDataType.get(), SIGNAL(iconChanged(QString)), this, SLOT(updateIcon()));
    }

    foreach (QString name, _data->properties()) {
        updateProperty(name);
    }
    updateRenderer();
    updateIcon();
    updateColor();
    updateSize();
    updatePos();
    updatePropertyList();
    update();
}

void DataItem::updatePos()
{
    int fixPos = boundingRect().width() / 2;
    setPos(_data->x() - fixPos, _data->y() - fixPos);
}

void DataItem::updateSize()
{
    if (_data->width() == _width) {
        return;
    }
    resetTransform();
    _width = _data->width();
    setScale(_data->width());
}

void DataItem::updateRenderer()
{
    QString iconPackage = _data->dataStructure()->document()->iconPackage();
    if (_sharedRenderers.count(iconPackage) == 0 || !_sharedRenderers.contains(iconPackage)) {
        QSvgRenderer *z = new QSvgRenderer(iconPackage);
        _sharedRenderers.insert(iconPackage, z);
        setSharedRenderer(z);
    } else {
        setSharedRenderer(_sharedRenderers.value(iconPackage));
    }
}

void DataItem::updateIcon()
{
    QString icon = _data->dataStructure()->document()->dataType(_data->dataType())->iconName();
    if (elementId().isEmpty() || elementId() != icon) {
        setElementId(icon);
        setTransformOriginPoint(boundingRect().width() / 2, boundingRect().width() / 2);
    }
}

void DataItem::updateVisibility(bool visible)
{
    if (visible == true) {
        this->show();
        QMap<QString, QGraphicsSimpleTextItem*>::const_iterator iter = _propertyValues.constBegin();
        while (iter != _propertyValues.constEnd()) {
            (*iter)->setVisible(true);
            ++iter;
        }
    } else {
        this->hide();
        QMap<QString, QGraphicsSimpleTextItem*>::const_iterator iter = _propertyValues.constBegin();
        while (iter != _propertyValues.constEnd()) {
            (*iter)->setVisible(false);
            ++iter;
        }
    }
}


void DataItem::updateColor()
{
    QColor c(_data->color().value<QColor>());
    if (!_data->isColored()) {
        delete _colorizer;
        setGraphicsEffect(0);
        _colorizer = 0;
        return;
    }

    delete _colorizer;
    _colorizer = new QGraphicsColorizeEffect();
    _colorizer->setColor(c);
    setGraphicsEffect(_colorizer);
}

void DataItem::updateProperty(QString name)
{
    if (!_propertyValues.contains(name)) {
        registerProperty(name);
        return;
    }
    _propertyValues[name]->setText(data()->property(name.toStdString().c_str()).toString());
    _propertyValues[name]->update();
}

QGraphicsItem* DataItem::propertyListItem() const
{
    return _item;
}

void DataItem::updatePropertyList()
{
    qreal offset = 0;
    foreach (QString property, data()->properties()) {
        if (!_propertyValues.contains(property)) {
            kError() << "Cannot update unknown property : " << property;
            continue;
        }
        _propertyValues[property]->setPos(data()->x()+20, data()->y() + offset);
        _propertyValues[property]->setVisible(true);
        _propertyValues[property]->update();

        offset += 20;
    }
}

void DataItem::registerProperty(QString name)
{
    if (_propertyValues.contains(name)) {
        return;
    }
    // do not show property if it is not a type property
    if (!_data->properties().contains(name)) {
        return;
    }
    _propertyValues.insert(name, new QGraphicsSimpleTextItem(data()->property(name.toStdString().c_str()).toString()));
    _propertyValues[name]->setFlags(ItemIgnoresTransformations);
    _propertyValues[name]->setFont(_font);
    _propertyValues[name]->setZValue(zValue() + 1);
    _item->addToGroup(_propertyValues[name]);

    updatePropertyList();
}

void DataItem::removeProperty(QString name)
{
    if (_propertyValues.contains(name)) {
        kWarning() << "Property not removed: not registered at DataItem.";
        return;
    }
    delete _propertyValues[name];
    _propertyValues.remove(name);
    _item->removeFromGroup(_propertyValues[name]);

    updatePropertyList();
}

// void DataItem::updateName()
// {
//     if (!_name) {
//         _name = new QGraphicsSimpleTextItem(i18n("%1", _data->name()));
//         _name->setFlags(ItemIgnoresTransformations);
//         _name->setFont(_font);
//         _name->setZValue(zValue() + 1);
//     } else if (_name->text() != _data->name()) {
//         _name->setText(i18n("%1", _data->name()));
//     }
//
//     int style = GraphicsLayout::self()->viewStyleDataNode();
//
//     qreal dataWidth = boundingRect().width() * scale();
//
//     qreal x =  pos().x();
//     if (style == ConfigureDefaultProperties::CENTER) {
//         x  += ((dataWidth > _name->boundingRect().width() + 10)
//                ? ((dataWidth - _name->boundingRect().width()) / 4)
//                : dataWidth + 30);
//     }
//
//     qreal y =  pos().y() + ((style == ConfigureDefaultProperties::ABOVE) ? + 15 - (dataWidth / 2)
//                             : (style == ConfigureDefaultProperties::BELOW) ? 25 + (dataWidth / 2)
//                             : 0);
//
//     _name->setVisible(_data->isNameVisible());
//
//     if (_value && _value->isVisible()) {
//         y += ((style == ConfigureDefaultProperties::ABOVE) ? -20 :  20);
//     }
//     _name->setPos(x, y);
// }
/*
void DataItem::updateValue()
{
    if (!_value) {
        _value = new QGraphicsSimpleTextItem(i18n("v=%1", _data->value().toString()));
        _value->setFlags(ItemIgnoresTransformations);
        _value->setFont(_font);
        _value->setZValue(zValue() + 2);
    } else if (QVariant(_value->text()) != _data->value().toString()) {
        _value ->setText(i18n("v=%1", _data->value().toString()));
    }

    int style = GraphicsLayout::self()->viewStyleDataNode();

    qreal dataWidth = boundingRect().width() * scale();

    qreal x =  pos().x();
    if (style == ConfigureDefaultProperties::CENTER) {
        x  += ((dataWidth > _value->boundingRect().width() + 10)
               ? ((dataWidth - _value->boundingRect().width()) / 4)
               : dataWidth + 30);
    }

    qreal y =  pos().y() + ((style == ConfigureDefaultProperties::ABOVE) ? + 15 - (dataWidth / 2)
                            : (style == ConfigureDefaultProperties::BELOW) ? 65 + (dataWidth / 2)
                            : 40);

    if (!_name || !_name->isVisible()) {
        y += ((style == ConfigureDefaultProperties::BELOW) ? -20 : -10);
    }

    _value->setPos(x, y);
    _value->setVisible(_data->isValueVisible());
}*/
