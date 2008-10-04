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

#include "UI_Separator.h"
#include <QScrollBar>
#include <QPainter>
#include <QVariant>
#include <QStyleOption>

Separator::Separator ( QWidget* parent ) : QWidget ( parent ){
  setSizePolicy ( QSizePolicy::Minimum, QSizePolicy::Minimum );
  setProperty ( "isSeparator", true );
}

QSize Separator::sizeHint() const{
  QStyleOption opt;
  opt.initFrom ( this );
  const int extent = style()->pixelMetric ( QStyle::PM_ToolBarSeparatorExtent, &opt, parentWidget() );
  return QSize ( extent, extent );
}

void Separator::paintEvent ( QPaintEvent * ){
  QPainter p ( this );
  QStyleOption opt;
  opt.initFrom ( this );
  style()->drawPrimitive ( QStyle::PE_IndicatorToolBarSeparator, &opt, &p, parentWidget() );
}
