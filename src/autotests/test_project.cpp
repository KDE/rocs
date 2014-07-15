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

#include "test_project.h"
#include "libgraphtheory/typenames.h"
#include "libgraphtheory/graphdocument.h"
#include "libgraphtheory/nodetype.h"
#include "libgraphtheory/edgetype.h"
#include "libgraphtheory/node.h"
#include "libgraphtheory/edge.h"
#include "project/project.h"
#include <QTemporaryFile>
#include <QUrl>
#include <QTest>

using namespace GraphTheory;

void TestProject::createProject()
{
    Project *project = new Project();

    QTemporaryFile codeFile;
    codeFile.open();
    project->addCodeFile(QUrl::fromLocalFile(codeFile.fileName()));
    QCOMPARE(project->codeFiles().length(), 1);
    QCOMPARE(project->codeFiles().first().toLocalFile(), QString(codeFile.fileName()));
}


QTEST_MAIN(TestProject)
