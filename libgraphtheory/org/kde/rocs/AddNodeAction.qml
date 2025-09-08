// SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

import QtQuick
import QtQuick.Controls
import org.kde.rocs.graphtheory

Action {
    id: root
    text: i18n("Create Node")
    icon.source: "qrc:/icons/node"
    checkable: true
    //TODO tooltip: i18n("Add a node to the scene")
}
