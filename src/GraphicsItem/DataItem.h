/* 
    This file is part of Rocs.
    Copyright 2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2011  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
#include <QStateMachine>
#include <QState>
#include <QPropertyAnimation>
#include <QSvgRenderer>
#include <QGraphicsTextItem>
#include <QGraphicsColorizeEffect>
#include <QMap>
#include <QString>
#include <QFont>
#include <boost/shared_ptr.hpp>

#include "rocslib_export.h"
#include "Rocs_Typedefs.h"

class Data;

class ROCSLIB_EXPORT DataItem : public QGraphicsSvgItem{
  Q_OBJECT

  public:
    DataItem(DataPtr n);
    virtual ~DataItem();
    DataPtr data() const { return _data; }
    static QMap<QString, QSvgRenderer*> _renders;
    QGraphicsSimpleTextItem *name() const;
    QGraphicsSimpleTextItem *value() const;
    void remove();
    
  private slots:
    void setupNode();
    void updateRenderer();
    void updateIcon();
    void updateName();
    void updateValue();
    void updateColor();
    void updatePos();
    void updateSize();

private:
    DataPtr _data;
    QString _iconPackage;
    QGraphicsSimpleTextItem *_name;
    QGraphicsSimpleTextItem *_value;
    QGraphicsColorizeEffect *_colorizer;
    QGraphicsRectItem *_boundingRect;
    QFont _font;
    int _oldStyle;

    qreal _originalWidth;
    qreal _width;
};

#endif
