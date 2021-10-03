/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef TESTTIKZFILEFORMAT_H
#define TESTTIKZFILEFORMAT_H

#include <QObject>

class TestTikzFileFormat : public QObject
{
    Q_OBJECT
public:
    TestTikzFileFormat();

private Q_SLOTS:
    void serializeTest();
};

#endif
