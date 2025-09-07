/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef TESTROCS1FILEFORMAT_H
#define TESTROCS1FILEFORMAT_H

#include <QObject>

class TestRocs1FileFormat : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void serializeUnserializeTest();
    void serializeUnserializeTypesTest();
    //     void projectLoadSaveTest();
};

#endif
