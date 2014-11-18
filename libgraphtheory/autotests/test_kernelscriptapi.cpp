/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
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
