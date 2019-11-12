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

Item {
    id: root
    property variant from: Qt.point(0, 0)
    property variant to: Qt.point(0, 0)

    signal changed

    function contains(x, y)
    {
        return (x >= rect.x && x <= rect.x + rect.width && y >= rect.y && y <= rect.y + rect.height)
    }

    onFromChanged: changed()
    onToChanged: changed()

    Rectangle {
        id: rect
        x: Math.min(from.x, to.x)
        y: Math.min(from.y, to.y)
        width: Math.abs(from.x - to.x)
        height: Math.abs(from.y - to.y)

        color: "#afc3deff"
        border.color: "steelblue"
        border.width: 2
    }
}
