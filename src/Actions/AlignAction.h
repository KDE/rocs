/*
    This file is part of Rocs.
    Copyright 2008       Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
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

#ifndef ALIGNACTION_H
#define ALIGNACTION_H

#include <KAction>
#include "Data.h"
#include "GraphScene.h"

class AlignAction : public KAction
{
    Q_OBJECT
public:
    /*! enumerates available alignments supported by this action */
    enum Orientation {Left, Right, Top, Bottom, HCenter, VCenter, Circle, MinCutTree};

    /** Creates a new align button. If a data is registered, this data is aligned
      according to the specified topology. Else, the currently selected set of nodes is aligned.
      \param actionName the name of the button
      \param tooltip some helper text.
      \param o the orientation that this button will work on.
      \param gc the graph scene containing the nodes
      \param parent the parent widget
     */
    AlignAction(const QString& tooltip, AlignAction::Orientation o, GraphScene *gc);

    /** register data to be aligned
     * \param dataList
     */
    void registerData(DataList dataList);

    /** unset registered data */
    void unsetData();

public slots:
    /** Run the previously specified align algorithm for the selected set of data.
     * If data is set by \see registerData(...) this data is aligned and the
     * registered data is unset after this operation.
     */
    void align();

private:
    void setupOrientation(Orientation o);

    /** Align the data items on y-axis.
     * \param l the non-empty list of selected data.
     */
    void alignY(DataList dataList);

    /** Align the data items on x-axis.
     *\param l the non-empty list of selected data.
    */
    void alignX(DataList dataList);

    /** Align the data items on circle, given by maximal diameter of input set.
     * the data is moved according the previous  angles.
     * \param dataList the non-empty list of selected data
     */
    void alignCircle(DataList dataList);

    /** Align the data items as a forest-like structure with (approximately) minimal number of cuts.
     * This utilizes the Fruchtman-Reingold algorithm, implemented by the Boost Graph Library.
     * \param dataList the non-empty list of selected data
     */
    void alignMinCutTree(DataList dataList);

    /** checks if a node is more in the left than the other.
     * \param n1 first node
     * \param n2 second node
     */
    static bool leftLessThan(DataPtr n1, DataPtr n2) {
        return n1->x() < n2->x();
    }

    /** checks if a node is more in the right than the other.
     * \param n1 first node
     * \param n2 second node
     */
    static bool rightLessThan(DataPtr n1, DataPtr n2) {
        return n1->x() > n2->x();
    }

    /** checks if a node is more in the top than the other.
     * \param n1 first node
     * \param n2 second node
     */
    static bool topLessThan(DataPtr n1, DataPtr n2) {
        return n1->y() < n2->y();
    }

    /** checks if a node is more in the bottom than the other.
     * \param n1 first node
     * \param n2 second node
     */
    static bool bottomLessThan(DataPtr n1, DataPtr n2) {
        return n1->y() > n2->y();
    }


    Orientation _orientation;   // the configured orientation
    DataList _registeredData;   // possibly registered
    GraphScene *_graphScene;    // graph scene that contains selected nodes
};

#endif
