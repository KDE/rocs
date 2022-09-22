/*
 *  SPDX-FileCopyrightText: 2020 Dilson Almeida Guimarães <dilsonguim@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef TESTFORCEBASEDLAYOUT_H
#define TESTFORCEBASEDLAYOUT_H

#include "typenames.h"
#include <QCoreApplication>
#include <QObject>
#include <QUrl>
#include <QVector>

class ForceBasedLayoutTest : public QObject
{
    Q_OBJECT
public:
    ForceBasedLayoutTest()
    {
        QCoreApplication::addLibraryPath(ADDITIONAL_PLUGIN_DIR);
    }

private:
    void findGraphFiles(const QString &path, QVector<QUrl> &files);
private Q_SLOTS:
    void testMargin_data();
    void testMargin();
};

#endif
