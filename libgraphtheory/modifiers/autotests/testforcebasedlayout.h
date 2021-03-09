/*
 *  SPDX-FileCopyrightText: 2020 Dilson Almeida Guimarães <dilsonguim@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef TESTFORCEBASEDLAYOUT_H
#define TESTFORCEBASEDLAYOUT_H

#include <QObject>
#include <QVector>
#include <QUrl>
#include "typenames.h"

class ForceBasedLayoutTest : public QObject
{
    Q_OBJECT;
private:
    void findGraphFiles(const QString& path, QVector<QUrl>& files);
private slots:
    void testMargin_data();
    void testMargin();
};

#endif
