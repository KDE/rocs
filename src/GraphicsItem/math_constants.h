
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

#ifndef MATH_CONSTANTS_H
#define MATH_CONSTANTS_H


static const double Pi = 3.14159265358979323846264338327950288419717;

/// The value of Pi * 2
static const double TwoPi = 2.0 * Pi;

/// The Value of PI / 3
static const qreal PI_3 = Pi / 3.0;

/// The Value of PI / 2
static const qreal PI_2 = Pi / 2.0;

/// The Value of PI_4
static const qreal PI_4 = Pi / 4.0;

/// The Value of PI_8
static const qreal PI_8 = Pi / 8.0;

qreal rabs(qreal x) {
    if (x<0) return x*-1;
    return x;
}
#endif
