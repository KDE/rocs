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

    text: i18n("Select & Move")
    iconName: "rocsselect"
    checkable: true
    tooltip: i18n("Select and move elements on the scene")
}
