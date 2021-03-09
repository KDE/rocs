/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
