// SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

import QtQuick

Rectangle {
    id: root

    property point from
    property point to

    x: Math.min(root.from.x, root.to.x)
    y: Math.min(root.from.y, root.to.y)
    width: Math.abs(root.from.x - root.to.x)
    height: Math.abs(root.from.y - root.to.y)

    color: "#afc3deff"
    border.color: "steelblue"
    border.width: 2

    signal changed

    function contains(x, y)
    {
        return (x >= root.x && x <= root.x + root.width && y >= root.y && y <= root.y + root.height)
    }

    onFromChanged: changed()
    onToChanged: changed()
}
