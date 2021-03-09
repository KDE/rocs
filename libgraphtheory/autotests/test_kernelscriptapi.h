/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef TEST_KERNELSCRIPTAPI_H
#define TEST_KERNELSCRIPTAPI_H

#include <QObject>

namespace GraphTheory
{
class GraphDocument;
class Node;
class Edge;
}

using namespace GraphTheory;

class TestKernelScriptApi : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    /** test validity of all installed files **/
    void xmlValidity();
};

#endif
