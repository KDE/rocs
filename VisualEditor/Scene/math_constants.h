/*
    This file is part of Rocs.
    Copyright 2008-2009 Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008      Ugo Sangiori <ugorox@gmail.com>

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

#ifndef MATH_CONSTANTS_H
#define MATH_CONSTANTS_H


static const qreal Pi = 3.14159265358979323846264338327950288419717;

/// The value of Pi * 2
static const qreal TwoPi = 2.0 * Pi;

/// The Value of PI / 3
static const qreal PI_3 = Pi / 3.0;

/// The Value of PI / 2
static const qreal PI_2 = Pi / 2.0;

/// The Value of PI_4
static const qreal PI_4 = Pi / 4.0;

/// The Value of PI_8
static const qreal PI_8 = Pi / 8.0;

qreal rabs(qreal x)
{
    if (x < 0) return x * -1;
    return x;
}
#endif
