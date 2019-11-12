/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

import QtQuick 2.2

Rectangle {
    id: line
    property alias fromX: line.x
    property alias fromY: line.y

    property real toX: line.x
    property real toY: line.y

    color: "black"
    height: 2
    smooth: true;

    transformOrigin: Item.TopLeft;

    width: getWidth(fromX,fromY,toX,toY);
    rotation: getSlope(fromX,fromY,toX,toY);

    function getWidth(sx1,sy1,sx2,sy2)
    {
        var w = Math.sqrt(Math.pow((sx2-sx1),2)+Math.pow((sy2-sy1),2));
        return w;
    }

    function getSlope(sx1,sy1,sx2,sy2)
    {
        var a,m,d;
        var b = sx2-sx1;
        if (b === 0) {
            return 0;
        }
        a = sy2 - sy1;
        m = a/b;
        d = Math.atan(m) * 180 / Math.PI;

        if (a < 0 && b < 0) {
            return d + 180;
        }
        else if (a >= 0 && b >= 0) {
            return d;
        }
        else if (a < 0 && b >= 0) {
            return d;
        }
        else if (a >= 0 && b < 0) {
            return d + 180;
        }
        else {
            return 0;
        }
    }
}
