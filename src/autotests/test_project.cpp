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
#include "libgraphtheory/editor.h"
#include "libgraphtheory/graphdocument.h"
#include "libgraphtheory/nodetype.h"
#include "libgraphtheory/edgetype.h"
#include "libgraphtheory/node.h"
#include "libgraphtheory/edge.h"
#include "project/project.h"
#include <QTemporaryFile>
#include <QUrl>
#include <QTest>
#include <KTextEditor/Document>
#include <KTextEditor/Editor>


void TestProject::projectOperations()
{
    GraphTheory::Editor *graphEditor = new GraphTheory::Editor;
    Project project(graphEditor);

    // test import of codefile
    QTemporaryFile codeFile1;
    QTemporaryFile codeFile2;
    codeFile1.setFileTemplate("XXXXXXX.js");
    codeFile2.setFileTemplate("XXXXXXX.js");
    codeFile1.open();
    codeFile2.open();
    KTextEditor::Document *codeDoc1 = project.importCodeDocument(QUrl::fromLocalFile(codeFile1.fileName()));
    KTextEditor::Document *codeDoc2 = project.importCodeDocument(QUrl::fromLocalFile(codeFile2.fileName()));

    QCOMPARE(project.codeDocuments().length(), 2); // check correct adding
    QVERIFY(project.codeDocuments().first()->url().toLocalFile().startsWith(project.workingDir())); // check working dir
    QVERIFY(codeDoc1->url().toLocalFile() != codeDoc2->url().toLocalFile()); // check for distinct names
    project.removeCodeDocument(codeDoc1);     // check removal
    QCOMPARE(project.codeDocuments().length(), 1);

    // test import of graph
    QTemporaryFile graphFile;
    graphFile.setFileTemplate("XXXXXXX.graph");
    graphFile.open();
    GraphTheory::GraphDocumentPtr graphDoc = project.importGraphDocument(QUrl::fromLocalFile(graphFile.fileName()));
    QCOMPARE(project.graphDocuments().length(), 1);
    QVERIFY(project.graphDocuments().first()->documentUrl().toLocalFile().startsWith(project.workingDir()));
    project.removeGraphDocument(graphDoc);
    QCOMPARE(project.graphDocuments().length(), 0);

    //cleanup
    graphEditor->deleteLater();
}

void TestProject::loadSave()
{
    GraphTheory::Editor *graphEditor = new GraphTheory::Editor;
    Project project(graphEditor);

    QTemporaryFile projectFile;
    projectFile.open();
    project.setProjectUrl(QUrl::fromLocalFile(projectFile.fileName()));
    project.setName("testname");

    // add files
    QTemporaryFile codeFile;
    codeFile.setFileTemplate("XXXXXXX.js");
    codeFile.open();
    project.importCodeDocument(QUrl::fromLocalFile(codeFile.fileName()));
    project.addGraphDocument(GraphTheory::GraphDocument::create());
    QVERIFY(project.projectSave());

    Project loadedProject(QUrl::fromLocalFile(projectFile.fileName()), graphEditor);
    QCOMPARE(loadedProject.name(), QString("testname"));
    QCOMPARE(loadedProject.codeDocuments().count(), project.codeDocuments().count());
    QCOMPARE(loadedProject.graphDocuments().count(), project.graphDocuments().count());

    graphEditor->deleteLater();
}

QTEST_MAIN(TestProject)
