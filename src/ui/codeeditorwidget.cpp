/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License or (at your option) version 3 or any later version
 *  accepted by the membership of KDE e.V. (or its successor approved
 *  by the membership of KDE e.V.), which shall act as a proxy
 *  defined in Section 14 of version 3 of the license.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "codeeditorwidget.h"
#include "project/project.h"
#include <KTextEditor/View>
#include <KTextEditor/Editor>
#include <KTextEditor/Document>
#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QDebug>

CodeEditorWidget::CodeEditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_project(0)
{
    QLayout *layout = new QVBoxLayout();
    m_editor = KTextEditor::Editor::instance();
    if (!m_editor) {
        qCritical() << "KTextEditor could not be found, please check your installation";
    }
    m_viewWidgets = new QTabWidget(this);
    m_viewWidgets->setTabsClosable(false);
    layout->addWidget(m_viewWidgets);
    layout->setSpacing(0);
    setLayout(layout);
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
    connect(project, SIGNAL(codeDocumentAboutToBeAdded(KTextEditor::Document*,int)), this, SLOT(onCodeDocumentAboutToBeAdded(KTextEditor::Document*,int)));
    connect(project, SIGNAL(codeDocumentAboutToBeRemoved(int,int)), this, SLOT(onCodeDocumentAboutToBeRemoved(int,int)));

    // initialize views
    for (int index = 0; index < m_project->codeDocuments().count(); ++index) {
        KTextEditor::Document *document = m_project->codeDocuments().at(index);
        m_viewWidgets->insertTab(index, document->createView(this), document->documentName());
    }
}

KTextEditor::Document * CodeEditorWidget::activeDocument() const
{
    return m_project->codeDocuments().at(m_viewWidgets->currentIndex());
}

void CodeEditorWidget::onCodeDocumentAboutToBeAdded(KTextEditor::Document* document, int index)
{
    m_viewWidgets->insertTab(index, document->createView(this), document->documentName());
}

void CodeEditorWidget::onCodeDocumentAboutToBeRemoved(int start, int end)
{
    for (int i = end; i >= start; --i) {
        m_viewWidgets->removeTab(i);
    }
}
