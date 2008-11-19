/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "SUI_PaletteLayout.h"
#include <QWidget>
#include <QStyleOption>

PaletteLayout::PaletteLayout ( QWidget *parent, int margin, int spacing ) 
: QLayout ( parent ){
  setMargin ( margin );
  setSpacing ( spacing );
  resetCache();
}

PaletteLayout::PaletteLayout ( int spacing ){
  setSpacing ( spacing );
  resetCache();
}

PaletteLayout::~PaletteLayout(){
  QLayoutItem *item;
  while ( ( item = takeAt ( 0 ) ) ) delete item;
}

void PaletteLayout::addItem ( QLayoutItem *item ){
  itemList.append ( item );
  resetCache();
}

int PaletteLayout::count() const{
  return itemList.size();
}

QLayoutItem* PaletteLayout::itemAt ( int index ) const{
  return itemList.value ( index );
}

QLayoutItem* PaletteLayout::takeAt ( int index ){
  resetCache();
  if ( index >= 0 && index < itemList.size() ) return itemList.takeAt ( index );
  else return 0;
}

Qt::Orientations PaletteLayout::expandingDirections() const{
  return Qt::Vertical;
}

bool PaletteLayout::hasHeightForWidth() const{
  return true;
}

int PaletteLayout::heightForWidth ( int width ) const{
  if ( isCachedHeightForWidth && cachedHeightForWidth.width() == width ){
    return cachedHeightForWidth.height();
  }
  
  cachedHeightForWidth.setWidth ( width );
  cachedHeightForWidth.setHeight ( doLayout ( QRect ( 0, 0, width, 0 ), true ) );
  isCachedHeightForWidth = true;
  return cachedHeightForWidth.height();
}

void PaletteLayout::setGeometry ( const QRect &rect ){
  resetCache(); QLayout::setGeometry ( rect ); doLayout ( rect, false );
}

QSize PaletteLayout::sizeHint() const{
  return minimumSize();
}

QSize PaletteLayout::minimumSize() const{
  if ( isCachedMinimumSize ) return cachedMinimumSize;

  cachedMinimumSize = QSize();
  QLayoutItem *item;
  foreach ( item, itemList ){
    cachedMinimumSize = cachedMinimumSize.expandedTo ( item->minimumSize() );
  }
  isCachedMinimumSize = true;
  return cachedMinimumSize;
}

void PaletteLayout::setOneLine ( bool b ){
  oneLine = b; invalidate();
}

bool PaletteLayout::isOneLine() const{
  return oneLine;
}

void PaletteLayout::invalidate(){
  resetCache();
  QLayout::invalidate();
}

void PaletteLayout::resetCache(){
  isCachedMinimumSize = false;
  isCachedHeightForWidth = false;
}

int PaletteLayout::doLayout ( const QRect &rect, bool testOnly ) const{
  int x = rect.x();
  int y = rect.y();
  int lineHeight = 0;

  if ( oneLine ) foreach ( QLayoutItem* item, itemList ){
    y = y + lineHeight + spacing();
    lineHeight = item->sizeHint().height();
    if ( !testOnly ){
      item->setGeometry ( QRect ( rect.x(), y, rect.width(), lineHeight ) );
    }
  }

  else foreach ( QLayoutItem* item, itemList ){
    int w = item->sizeHint().width(); int h = item->sizeHint().height();
    int nextX = x + item->sizeHint().width() + spacing();
    if ( item->widget() && item->widget()->property ( "isSeparator" ).toBool() ){
      x = rect.x();
      y = y + lineHeight + spacing();
      nextX = x + rect.width();
      w = rect.width();
      lineHeight = 0;
    }
    else if ( nextX - spacing() > rect.right() && lineHeight > 0 ){
      x = rect.x();
      y = y + lineHeight + spacing();
      nextX = x + w + spacing();
      lineHeight = 0;
    }
    if ( !testOnly ) item->setGeometry ( QRect ( x, y, w, h ) );
    x = nextX;
    lineHeight = qMax ( lineHeight, h );
  }
  return y + lineHeight - rect.y();
}
