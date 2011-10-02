/*  
    This file is part of Rocs.
    Copyright 2008  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008  Ugo Sangiori <ugorox@gmail.com>
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

#ifndef ALIGNACTION_H
#define ALIGNACTION_H

#include <KAction>
#include "DataItem.h"
#include "Data.h"


class AlignAction : public KAction {
    Q_OBJECT
public:
    /*! this enum has all the possibilities of orientations for the aligns. */
    enum Orientation {Left, Right, Top, Bottom, HCenter, VCenter, Circle, MinCutTree};

    /*! creates a new align button.
      \param actionName the name of the button
      \param tooltip some helper text.
      \param o the orientation that this button will work on.
      \param parent the parent widget
    */
    AlignAction(const QString& tooltip,AlignAction::Orientation o, QWidget *parent);

private slots:
    /** Run the previously specified align algorithm for the selected set of data. items.
     */
    void align();

private:
    /** Align the data items on y-axis.
     * \param l the non-empty list of selected data.
     */
    void alignY(QList<DataItem*>& dataItemList);

    /** Align the data items on x-axis.
     *\param l the non-empty list of selected data.
    */
    void alignX(QList<DataItem*>& dataItemList);

    /** Align the data items on circle, given by maximal diameter of input set.
     * the data is moved according the previous  angles.
     * \param dataList the non-empty list of selected data
     */
    void alignCircle(QList<DataItem*>& dataItemList);

    /** Align the data items as a forest-like structure with (approximately) minimal number of cuts.
     * This utilizes the Fruchtman-Reingold algorithm, implemented by the Boost Graph Library.
     * \param dataList the non-empty list of selected data
     */
    void alignMinCutTree(QList<DataItem*>& dataItemList);

    /*! the orientation that this button will work on. */
    Orientation m_orientation;

};

#endif
