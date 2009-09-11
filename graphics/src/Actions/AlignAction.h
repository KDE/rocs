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
#ifndef ALIGNACTION_H
#define ALIGNACTION_H

#include <KAction>
#include "NodeItem.h"
#include "node.h"


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
    /*! run the align algorithm in the selected nodes. */
    void align();

private:
    /*! align the nodes in the Y axis.
      \p l the list of selected nodes.
      */
    void allignY(QList<NodeItem*>& l);

    /*! align the nodes in the X axis.
    \p l the list of selected nodes.
    */
    void allignX(QList<NodeItem*>& l);

    /*! the orientation that this button will work on. */
    Orientation m_orientation;

};

#endif
