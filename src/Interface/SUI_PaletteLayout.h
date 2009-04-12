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

#ifndef SUI_PALETTELAYOUT_H
#define SUI_PALETTELAYOUT_H

#include <QVBoxLayout>

/*! \brief defines the layout on the pallete bar */
class PaletteLayout: public QLayout {
public:
    explicit PaletteLayout ( QWidget *parent, int margin = 0, int spacing = -1 );
    PaletteLayout ( int spacing = -1 ) ;
    ~PaletteLayout();
    void addItem ( QLayoutItem *item ) ;
    int count() const;
    QLayoutItem* itemAt ( int index ) const ;
    QLayoutItem* takeAt ( int index ) ;
    Qt::Orientations expandingDirections() const ;
    bool hasHeightForWidth() const ;
    int heightForWidth ( int width ) const ;
    void setGeometry ( const QRect &rect ) ;
    QSize sizeHint() const ;
    QSize minimumSize() const ;
    void setOneLine ( bool b ) ;
    bool isOneLine() const ;
    void invalidate() ;

protected:
    void resetCache() ;
    int doLayout ( const QRect &rect, bool testOnly ) const;
    QList<QLayoutItem *> itemList;
    bool oneLine;

    mutable bool isCachedMinimumSize;
    mutable bool isCachedHeightForWidth;
    mutable QSize cachedMinimumSize;
    mutable QSize cachedHeightForWidth;
};

#endif
