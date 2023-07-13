/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "test_project.h"
#include "libgraphtheory/edge.h"
#include "libgraphtheory/edgetype.h"
#include "libgraphtheory/editor.h"
#include "libgraphtheory/graphdocument.h"
#include "libgraphtheory/node.h"
#include "libgraphtheory/nodetype.h"
#include "libgraphtheory/typenames.h"
#include "project/project.h"
#include <KTextEditor/Document>
#include <KTextEditor/Editor>
#include <QTemporaryFile>
#include <QTest>
#include <QUrl>

using namespace GraphTheory;

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
    project.setDocumentName(codeDoc1, "Test");

    QCOMPARE(project.codeDocuments().length(), 2); // check correct adding

    qDebug() << "Project working dir  =" << project.workingDir();
    qDebug() << "Project document path=" << project.codeDocuments().first()->url().toLocalFile();
    qDebug() << "Test working dir     =" << QDir::currentPath();

    QVERIFY(project.codeDocuments().first()->url().toLocalFile().startsWith(QDir::currentPath())); // check working dir
    QVERIFY(codeDoc1->url().toLocalFile() != codeDoc2->url().toLocalFile()); // check for distinct names
    project.tryToRemoveCodeDocument(codeDoc1); // check removal
    QCOMPARE(project.codeDocuments().length(), 1);
    QCOMPARE(project.documentName(codeDoc1), QString("Test"));

    // test import of graph
    QTemporaryFile graphFile;
    graphFile.setFileTemplate("XXXXXXX.graph2");
    graphFile.open();
    GraphTheory::GraphDocumentPtr graphDoc = project.importGraphDocument(QUrl::fromLocalFile(graphFile.fileName()));
    QCOMPARE(project.graphDocuments().length(), 1);
    QVERIFY(project.graphDocuments().first()->documentUrl().toLocalFile().startsWith(project.workingDir()));
    project.removeGraphDocument(graphDoc);
    QCOMPARE(project.graphDocuments().length(), 0);

    // cleanup
    graphEditor->deleteLater();
}

void TestProject::loadSave()
{
    GraphTheory::Editor *graphEditor = new GraphTheory::Editor;
    Project project(graphEditor);

    QTemporaryFile projectFile;
    projectFile.open();
    project.setProjectUrl(QUrl::fromLocalFile(projectFile.fileName()));

    // add files
    QTemporaryFile codeFile;
    codeFile.setFileTemplate("XXXXXXX.js");
    codeFile.open();
    project.importCodeDocument(QUrl::fromLocalFile(codeFile.fileName()));
    project.addGraphDocument(GraphTheory::GraphDocument::create());
    QVERIFY(project.projectSave());

    // Now the documents live in the test dir
    const auto docs = project.codeDocuments();
    for (const auto &d : docs) {
        qDebug() << "Code document path=" << d->url().toLocalFile();
        QVERIFY(d->url().toLocalFile().startsWith(QDir::currentPath()));
    }

    Project loadedProject(QUrl::fromLocalFile(projectFile.fileName()), graphEditor);
    QCOMPARE(loadedProject.codeDocuments().count(), project.codeDocuments().count());
    QCOMPARE(loadedProject.graphDocuments().count(), project.graphDocuments().count());

    // After load, they live in the project dir
    const auto loadedDocs = loadedProject.codeDocuments();
    for (const auto &d : loadedDocs) {
        qDebug() << "Code document path=" << d->url().toLocalFile();
        QVERIFY(d->url().toLocalFile().startsWith(loadedProject.workingDir()));
    }

    graphEditor->deleteLater();
}

void TestProject::loadSaveMultipleGraphDocuments()
{
    GraphTheory::Editor *graphEditor = new GraphTheory::Editor;
    Project project(graphEditor);
    GraphDocumentPtr docA = graphEditor->createDocument();
    GraphDocumentPtr docB = graphEditor->createDocument();
    docA->setDocumentName("docA");
    project.addGraphDocument(docA);
    project.addGraphDocument(docB);

    // add nodes to identify documents later
    NodePtr nodeA = Node::create(docA);
    NodePtr nodeB1 = Node::create(docB);
    NodePtr nodeB2 = Node::create(docB);
    QCOMPARE(docA->nodes().count(), 1);
    QCOMPARE(docB->nodes().count(), 2);

    // save & load this project
    QTemporaryFile projectFile;
    projectFile.open();
    project.setProjectUrl(QUrl::fromLocalFile(projectFile.fileName()));
    QVERIFY(project.projectSave());

    Project loadedProject(QUrl::fromLocalFile(projectFile.fileName()), graphEditor);
    QCOMPARE(loadedProject.graphDocuments().count(), 2);
    QCOMPARE(loadedProject.graphDocuments().at(0)->nodes().count(), 1);
    QCOMPARE(loadedProject.graphDocuments().at(0)->documentName(), QString("docA"));
    QCOMPARE(loadedProject.graphDocuments().at(1)->nodes().count(), 2);

    graphEditor->deleteLater();
}

void TestProject::loadSaveMultipleScriptDocuments()
{
    GraphTheory::Editor *graphEditor = new GraphTheory::Editor;
    Project project(graphEditor);
    GraphDocumentPtr graphDoc = graphEditor->createDocument();
    project.addGraphDocument(graphDoc);

    // add files
    QTemporaryFile codeFileA, codeFileB;
    codeFileA.setFileTemplate("XXXXXXX.js");
    codeFileA.open();
    KTextEditor::Document *docA = project.importCodeDocument(QUrl::fromLocalFile(codeFileA.fileName()));
    project.setDocumentName(docA, "A");
    docA->setText("1");

    codeFileB.setFileTemplate("XXXXXXX.js");
    codeFileB.open();
    KTextEditor::Document *docB = project.importCodeDocument(QUrl::fromLocalFile(codeFileB.fileName()));
    project.setDocumentName(docB, "B");
    docB->setText("2");

    // save & load this project
    QTemporaryFile projectFile;
    projectFile.open();
    project.setProjectUrl(QUrl::fromLocalFile(projectFile.fileName()));
    QVERIFY(project.projectSave());

    Project loadedProject(QUrl::fromLocalFile(projectFile.fileName()), graphEditor);
    QCOMPARE(loadedProject.codeDocuments().count(), 2);
    QCOMPARE(loadedProject.documentName(loadedProject.codeDocuments().at(0)), QString("A"));
    QCOMPARE(loadedProject.codeDocuments().at(0)->text().trimmed(), QString("1"));
    QCOMPARE(loadedProject.codeDocuments().at(1)->text().trimmed(), QString("2"));

    graphEditor->deleteLater();
}

void TestProject::loadBrokenFilesWithoutCrashing01()
{
    GraphTheory::Editor *graphEditor = new GraphTheory::Editor;
    Project project(QUrl::fromLocalFile("testfiles/broken_01.rocs"), graphEditor);
    QCOMPARE(project.graphDocuments().count(), 0);
}

QTEST_MAIN(TestProject)

#include "moc_test_project.cpp"
