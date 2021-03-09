/*
 *  SPDX-FileCopyrightText: 2014-2015 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include "codeeditorwidget.h"
#include <KTextEditor/View>
#include <KTextEditor/Editor>
#include <KTextEditor/Document>
#include <KLocalizedString>
#include <QInputDialog>
#include <QLineEdit>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QDebug>

CodeEditorWidget::CodeEditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_project(Q_NULLPTR)
{
    QLayout *layout = new QVBoxLayout();
    m_editor = KTextEditor::Editor::instance();
    if (!m_editor) {
        qCritical() << "KTextEditor could not be found, please check your installation";
    }
    m_viewWidgets = new QTabWidget(this);
    m_viewWidgets->setTabsClosable(true);
    layout->addWidget(m_viewWidgets);
    layout->setSpacing(0);
    setLayout(layout);
    connect(m_viewWidgets, &QTabWidget::tabBarDoubleClicked, this, &CodeEditorWidget::showDocumentNameDialog);
    connect(m_viewWidgets, &QTabWidget::tabCloseRequested, this, &CodeEditorWidget::closeTab);
}

void CodeEditorWidget::setProject(Project *project)
{
    if (m_project) {
        m_project->disconnect(this);
    }
    // remove all data
    while (m_viewWidgets->count() > 0) {
        m_viewWidgets->removeTab(0);
    }
    m_project = project;
    connect(project, &Project::codeDocumentAboutToBeAdded, this, &CodeEditorWidget::onCodeDocumentAboutToBeAdded);
    connect(project, &Project::codeDocumentAboutToBeRemoved, this, &CodeEditorWidget::onCodeDocumentAboutToBeRemoved);

    // initialize views
    for (int index = 0; index < m_project->codeDocuments().count(); ++index) {
        KTextEditor::Document *document = m_project->codeDocuments().at(index);
        m_viewWidgets->insertTab(index, document->createView(this), project->documentName(document));
    }
}

KTextEditor::Document * CodeEditorWidget::activeDocument() const
{
    return m_project->codeDocuments().at(m_viewWidgets->currentIndex());
}

void CodeEditorWidget::onCodeDocumentAboutToBeAdded(KTextEditor::Document* document, int index)
{
    m_viewWidgets->insertTab(index, document->createView(this), m_project->documentName(document));
}

void CodeEditorWidget::onCodeDocumentAboutToBeRemoved(int start, int end)
{
    for (int i = end; i >= start; --i) {
        m_viewWidgets->removeTab(i);
    }
}


void CodeEditorWidget::showDocumentNameDialog(int index)
{
    if (!m_project || m_project->codeDocuments().count() < index) {
        return;
    }
    auto document = m_project->codeDocuments().at(index);
    bool ok;
    QString name = QInputDialog::getText(this,
        i18nc("@title", "Code Document Name"),
        i18n("Enter the name of your code document"),
        QLineEdit::Normal,
        m_project->documentName(document),
        &ok);
    if (ok) {
        m_project->setDocumentName(document, name);
        m_viewWidgets->setTabText(index, name);
    }
}

void CodeEditorWidget::closeTab(int index)
{
    m_project->tryToRemoveCodeDocument(m_project->codeDocuments().at(index));
}
