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

#ifndef DATAITEM_H
#define DATAITEM_H

#include <QGraphicsSvgItem>
#include <QMap>
#include <QString>
#include <QFont>

#include "VisualEditorExport.h"
#include <CoreTypes.h>

class Data;
class QGraphicsColorizeEffect;
class QSvgRenderer;
class QGraphicsItem;
class QGraphicsSimpleTextItem;

class VISUALEDITOR_EXPORT DataItem : public QGraphicsSvgItem
{
    Q_OBJECT

public:
    DataItem(DataPtr n);
    virtual ~DataItem();
    DataPtr data() const {
        return _data;
    }

    /**
     * \return the graphics item to be displayed at the scene
     */
    QGraphicsItem *propertyListItem() const;
    void remove();

private slots:
    void setupNode();
    void updateRenderer();
    void updateIcon();
    void updatePropertyList();
    void updateColor();
    void updateVisibility(bool visible);
    void updatePos();
    void updateSize();
    void updateProperty(const QString& name);
    void registerProperty(const QString& name);
    void removeProperty(const QString& name);

private:
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

#endif
