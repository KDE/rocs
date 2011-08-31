/*  
    This file is part of Rocs.
    Copyright 2008  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008  Ugo Sangiori <ugorox@gmail.com>

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

#ifndef ALIGNACTION_H
#define ALIGNACTION_H

#include <KAction>
#include "DataItem.h"
#include "Data.h"


class AlignAction : public KAction {
    Q_OBJECT
public:
    /*! this enum has all the possibilities of orientations for the aligns. */
    enum Orientation {Left, Right, Top, Bottom, HCenter, VCenter};

    /*! creates a new align button.
      \p actionName the name of the button
      \p tooltip some helper text.
      \p o the orientation that this button will work on.
      \p parent the parent widget
    */
    AlignAction(const QString& tooltip,AlignAction::Orientation o, QWidget *parent);

private slots:
    /*! run the align algorithm in the selected data. */
    void align();

private:
    /*! align the data in the Y axis.
      \p l the list of selected data.
      */
    void allignY(QList<DataItem*>& l);

    /*! align the data in the X axis.
    \p l the list of selected data.
    */
    void allignX(QList<DataItem*>& l);

    /*! the orientation that this button will work on. */
    Orientation m_orientation;

};

#endif
