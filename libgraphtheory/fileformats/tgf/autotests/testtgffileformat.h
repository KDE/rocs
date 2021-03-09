/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef TESTTGFFILEFORMATPLUGIN_H
#define TESTTGFFILEFORMATPLUGIN_H

#include <QObject>

class TestTgfFileFormat : public QObject
{
    Q_OBJECT
public:
    TestTgfFileFormat();

private slots:
    void serializeUnserializeTest();
};

#endif
