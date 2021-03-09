/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "test_kernelscriptapi.h"
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QString>
#include <QTest>

void TestKernelScriptApi::initTestCase()
{

}

void TestKernelScriptApi::cleanupTestCase()
{

}

void TestKernelScriptApi::xmlValidity()
{
    QXmlSchema schema;
    QVERIFY(schema.load(QUrl::fromLocalFile("kernelapi/kernelapi.xsd")));

    QXmlSchemaValidator validator(schema);
    QVERIFY(validator.validate(QUrl::fromLocalFile("kernelapi/console.xml")));
    QVERIFY(validator.validate(QUrl::fromLocalFile("kernelapi/document.xml")));
    QVERIFY(validator.validate(QUrl::fromLocalFile("kernelapi/node.xml")));
    QVERIFY(validator.validate(QUrl::fromLocalFile("kernelapi/edge.xml")));
}

QTEST_MAIN(TestKernelScriptApi)
