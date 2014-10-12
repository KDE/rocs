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

#include "grapheditorwidget.h"
#include "libgraphtheory/typenames.h"
#include "libgraphtheory/graphdocument.h"
#include "libgraphtheory/view.h"
#include "project/project.h"
#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>

GraphEditorWidget::GraphEditorWidget(QWidget *parent)
    : QWidget(parent)
{
    QLayout *layout = new QVBoxLayout();
    m_viewWidgets = new QTabWidget(this);
    m_viewWidgets->setTabsClosable(false);
    layout->addWidget(m_viewWidgets);
    layout->setSpacing(0);
    setLayout(layout);
}

void GraphEditorWidget::setProject(Project *project)
{
    disconnect(0, 0, this, SLOT(onGraphDocumentAboutToBeAdded(GraphTheory::GraphDocumentPtr, int)));
    disconnect(0, 0, this, SLOT(onGraphDocumentAboutToBeRemoved(int,int)));

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

    // initialize views
    for (int index = 0; index < project->graphDocuments().count(); ++index) {
        GraphTheory::GraphDocumentPtr document = project->graphDocuments().at(index);
        QWidget *widget = document->createView(this);
        m_viewWidgets->insertTab(index, widget, document->documentName());
    }
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
