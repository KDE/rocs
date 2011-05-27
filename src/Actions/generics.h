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

#ifndef GENERICS_H
#define GENERICS_H

#include "../GraphicsItem/DataItem.h"
/*! for the AlignNodes class, checks if a node is more in the left than the other.
\p n1 first node
\p n2 second node
*/
bool leftLessThan(DataItem *n1, DataItem *n2) {
    return n1->x() < n2->x();
}

/*! for the AlignNodes class, checks if a node is more in the right than the other.
\p n1 first node
\p n2 second node
*/
bool rightLessThan(DataItem *n1, DataItem *n2) {
    return n1->x() > n2->x();
}

/*! for the AlignNodes class, checks if a node is more in the top than the other.
\p n1 first node
\p n2 second node
*/
bool topLessThan(DataItem *n1, DataItem *n2) {
    return n1->y() < n2->y();
}

/*! for the AlignNodes class, checks if a node is more in the bottom than the other.
\p n1 first node
\p n2 second node
*/
bool bottomLessThan(DataItem *n1, DataItem *n2) {
    return n1->y() > n2->y();
}

#endif
