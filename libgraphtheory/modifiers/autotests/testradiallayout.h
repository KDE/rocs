/*
 *  SPDX-FileCopyrightText: 2020 Dilson Almeida Guimarães <dilsonguim@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef TESTRADIALLAYOUT_H
#define TESTRADIALLAYOUT_H

#include <QObject>
#include <QVector>
#include "typenames.h"

class RadialLayoutTest : public QObject
{
    Q_OBJECT
private:
private Q_SLOTS:
    void emptyGraph();
    void cycleGraph();
    void disconnectedGraph();
    void singleNode();
    void unidirectionalEdge();
    void testMargin();
    void minimumNodeDistance();
};

#endif
