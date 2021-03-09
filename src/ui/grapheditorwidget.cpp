/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include "grapheditorwidget.h"
#include "libgraphtheory/graphdocument.h"
#include "libgraphtheory/view.h"
#include <KLocalizedString>
#include <QInputDialog>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QDebug>

using namespace GraphTheory;

GraphEditorWidget::GraphEditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_viewWidgets(new QTabWidget(this))
    , m_project(0)
    , m_editor(0)
{
    QLayout *layout = new QVBoxLayout();
    m_viewWidgets->setTabsClosable(false);
    m_viewWidgets->setMovable(true);
    layout->addWidget(m_viewWidgets);
    layout->setSpacing(0);
    setLayout(layout);
}

void GraphEditorWidget::setProject(Project *project)
{
    if (m_project) {
        disconnect(m_project, &Project::graphDocumentAboutToBeAdded, this, &GraphEditorWidget::onGraphDocumentAboutToBeAdded);
        disconnect(m_project, &Project::graphDocumentAboutToBeRemoved, this, &GraphEditorWidget::onGraphDocumentAboutToBeRemoved);
    }

    // cleanup
    while (m_viewWidgets->count() > 0) {
        m_viewWidgets->removeTab(0);
    }

    connect(project, &Project::graphDocumentAboutToBeAdded,
        this, &GraphEditorWidget::onGraphDocumentAboutToBeAdded);
    connect(project, &Project::graphDocumentAboutToBeRemoved,
        this, &GraphEditorWidget::onGraphDocumentAboutToBeRemoved);
    connect(m_viewWidgets, &QTabWidget::currentChanged,
        project, &Project::setActiveGraphDocument);
    connect(m_viewWidgets, &QTabWidget::tabBarDoubleClicked,
        this, &GraphEditorWidget::showDocumentNameDialog);

    // initialize views
    for (int index = 0; index < project->graphDocuments().count(); ++index) {
        GraphTheory::GraphDocumentPtr document = project->graphDocuments().at(index);
        QWidget *widget = document->createView(this);
        m_viewWidgets->insertTab(index, widget, document->documentName());
    }
    m_project = project;
}

void GraphEditorWidget::onGraphDocumentAboutToBeAdded(GraphTheory::GraphDocumentPtr document, int index)
{
    QWidget *widget = document->createView(this);
    m_viewWidgets->insertTab(index, widget, document->documentName());
}

void GraphEditorWidget::onGraphDocumentAboutToBeRemoved(int start, int end)
{
    for (int i = end; i >= start; --i) {
        m_viewWidgets->removeTab(i);
    }
}

void GraphEditorWidget::showDocumentNameDialog(int index)
{
    if (!m_project || m_project->graphDocuments().count() < index) {
        return;
    }
    GraphDocumentPtr document = m_project->graphDocuments().at(index);
    bool ok;
    QString name = QInputDialog::getText(this,
        i18nc("@title", "Graph Document Name"),
        i18n("Enter the name of your graph document"),
        QLineEdit::Normal,
        document->documentName(),
        &ok);
    if (ok) {
        document->setDocumentName(name);
        m_viewWidgets->setTabText(index, name);
    }
}
