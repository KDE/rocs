/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import org.kde.rocs.graphtheory 1.0

Item {
    id: root

    property Node node: undefined

    Column {
        Repeater {
            model: NodePropertyModel { node: root.node }
            Text {
                text: {
                    if (model.visibility) {
                        return model.name + ": " + model.value;
                    } else {
                        return (model.value != undefined) ? model.value : "";
                    }
                }
            }
        }
    }
}
