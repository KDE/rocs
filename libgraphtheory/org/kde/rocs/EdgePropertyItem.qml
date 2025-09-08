// SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

import QtQuick
import org.kde.rocs.graphtheory

Item {
    id: root

    property Edge edge: undefined

    Column {
        Repeater {
            model: EdgePropertyModel { edge: root.edge }
            Text {
                id: propertyText
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
