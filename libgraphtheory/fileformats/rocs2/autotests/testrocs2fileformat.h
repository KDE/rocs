/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef TESTROCS2FILEFORMATPLUGIN_H
#define TESTROCS2FILEFORMATPLUGIN_H

#include <QObject>

class TestRocs2FileFormat : public QObject
{
    Q_OBJECT
public:
    TestRocs2FileFormat();

private slots:
    void documentTypesTest();
    void nodeAndEdgeTest();
    void parseVersion1Format();
};

#endif
