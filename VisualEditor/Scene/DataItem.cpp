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
#include "DocumentManager.h"

#include <KDebug>
#include <QFont>
#include <QGraphicsColorizeEffect>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QSvgRenderer>
#include <boost/concept_check.hpp>


class DataItemPrivate
{
public:
    explicit DataItemPrivate(DataPtr n)
    : _data(n)
    , _colorizer(0)
    , _font(QFont("Helvetica [Cronyx]", 12))
    , _oldStyle(GraphicsLayout::self()->viewStyleDataNode())
    , _item(new QGraphicsItemGroup())
    , _originalWidth(n->width())
    , _width(-1)
    , _oldDataType(n->dataStructure()->document()->dataType(n->dataType()))
    {
    }

    ~DataItemPrivate()
    {
        qDeleteAll(_propertyValues);
        _propertyValues.clear();
        delete _item;
    }

    DataPtr _data;
    QMap<QString, QGraphicsSimpleTextItem*> _propertyValues;
    QGraphicsColorizeEffect *_colorizer;
    QGraphicsRectItem *_boundingRect;
    QFont _font;
    int _oldStyle;
    QGraphicsItemGroup* _item;

    qreal _originalWidth;
    qreal _width;
    DataTypePtr _oldDataType;
};

DataItem::DataItem(DataPtr n)
    : QGraphicsSvgItem(0)
    , d(new DataItemPrivate(n))
{

    connect(n.get(), SIGNAL(removed()), this, SLOT(deleteLater()));
    connect(d->_oldDataType.get(), SIGNAL(iconChanged(QString)), this, SLOT(updateIcon()));

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
    delete d;
}

DataPtr DataItem::data() const
{
    return d->_data;
}

void DataItem::setupNode()
{
    if (d->_data.get()->dataStructure()->document()->dataType(d->_data.get()->dataType()) != d->_oldDataType) {
        disconnect(d->_oldDataType.get(), SIGNAL(iconChanged(QString)), this, SLOT(updateIcon()));
        d->_oldDataType = d->_data.get()->dataStructure()->document()->dataType(d->_data.get()->dataType());
        connect(d->_oldDataType.get(), SIGNAL(iconChanged(QString)), this, SLOT(updateIcon()));
    }

    // register properties
    foreach (const QString &property, d->_data->properties()) {
        registerProperty(property);
    }

    updateRenderer();
    updateIcon();
    updateColor();
    updateSize();
    updatePos();
    update();
}

void DataItem::updatePos()
{
    int fixPos = boundingRect().width() / 2;
    setPos(d->_data->x() - fixPos, d->_data->y() - fixPos);
    updatePropertyList();
}

void DataItem::updateSize()
{
    if (d->_data->width() == d->_width) {
        return;
    }
    resetTransform();
    d->_width = d->_data->width();
    setScale(d->_data->width());
}

void DataItem::updateRenderer()
{
    QString iconPackage = d->_data->dataStructure()->document()->iconPackage();
    setSharedRenderer(DocumentManager::self()->sharedRenderer(iconPackage));
}

void DataItem::updateIcon()
{
    QString icon = d->_data->dataStructure()->document()->dataType(d->_data->dataType())->iconName();
    if (elementId().isEmpty() || elementId() != icon) {
        setElementId(icon);
        setTransformOriginPoint(boundingRect().width() / 2, boundingRect().width() / 2);
    }
}

void DataItem::updateVisibility(bool visible)
{
    if (visible == true) {
        this->show();
        QMap<QString, QGraphicsSimpleTextItem*>::const_iterator iter = d->_propertyValues.constBegin();
        while (iter != d->_propertyValues.constEnd()) {
            (*iter)->setVisible(true);
            ++iter;
        }
    } else {
        this->hide();
        QMap<QString, QGraphicsSimpleTextItem*>::const_iterator iter = d->_propertyValues.constBegin();
        while (iter != d->_propertyValues.constEnd()) {
            (*iter)->setVisible(false);
            ++iter;
        }
    }
}

void DataItem::updateColor()
{
    QColor c(d->_data->color().value<QColor>());
    delete d->_colorizer;
    d->_colorizer = new QGraphicsColorizeEffect();
    d->_colorizer->setColor(c);
    setGraphicsEffect(d->_colorizer);
}

void DataItem::updateProperty(const QString& name)
{
    if (!d->_propertyValues.contains(name)) {
        registerProperty(name);
        return;
    }
    DataTypePtr dataType = data()->dataStructure()->document()->dataType(data()->dataType());
    d->_propertyValues[name]->setText(data()->property(name.toStdString().c_str()).toString());
    d->_propertyValues[name]->setVisible(dataType->isPropertyVisible(name));
    d->_propertyValues[name]->update();
    updatePropertyList();
}

QGraphicsItem* DataItem::propertyListItem() const
{
    return d->_item;
}

void DataItem::updatePropertyList()
{
    qreal offset = 0;
    foreach (const QString& property, data()->properties()) {
        if (!d->_propertyValues.contains(property)) {
            kError() << "Cannot update unknown property : " << property;
            continue;
        }
        if (d->_propertyValues[property]->isVisible() == false) {
            continue;
        }
        d->_propertyValues[property]->setPos(data()->x()+20, data()->y() + offset);
        d->_propertyValues[property]->update();
        offset += 20;
    }
}

void DataItem::registerProperty(const QString& name)
{
    if (d->_propertyValues.contains(name)) {
        return;
    }
    DataTypePtr dataType = data()->dataStructure()->document()->dataType(data()->dataType());
    d->_propertyValues.insert(name, new QGraphicsSimpleTextItem(data()->property(name.toStdString().c_str()).toString()));
    d->_propertyValues[name]->setFlags(ItemIgnoresTransformations);
    d->_propertyValues[name]->setFont(d->_font);
    d->_propertyValues[name]->setVisible(dataType->isPropertyVisible(name));
    d->_propertyValues[name]->setZValue(zValue() + 1);
    d->_item->addToGroup(d->_propertyValues[name]);

    updatePropertyList();
}

void DataItem::removeProperty(const QString& name)
{
    if (d->_propertyValues.contains(name)) {
        kWarning() << "Property not removed: not registered at DataItem.";
        return;
    }
    d->_propertyValues[name]->setVisible(false);
    d->_item->removeFromGroup(d->_propertyValues[name]);
    delete d->_propertyValues[name];
    d->_propertyValues.remove(name);

    updatePropertyList();
}
