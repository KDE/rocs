/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

import QtQuick 2.2
import QtQuick.Controls 1.1
import org.kde.rocs.graphtheory 1.0

Action {
    id: root

    property Node from: null
    property Node to: null

    signal createEdge(Node from, Node to);

    function apply()
    {
        if (from && to) {
            createEdge(from, to);
        }
        from = null;
        to = null;
    }

    text: i18n("Create Edge")
    iconSource: "qrc:/icons/edge"
    checkable: true
    tooltip: i18n("Create an edge between two nodes")
}
