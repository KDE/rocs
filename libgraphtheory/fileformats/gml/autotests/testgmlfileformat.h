/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2010 Wagner Reck <wagner.reck@gmail.com>
    SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef TESTGMLFILEFORMAT_H
#define TESTGMLFILEFORMAT_H

#include <QObject>

class TestGmlFileFormat : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void parseTest();
    void serializeTest();
};

#endif
