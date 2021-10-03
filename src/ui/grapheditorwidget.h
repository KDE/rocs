/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef GRAPHEDITORWIDGET_H
#define GRAPHEDITORWIDGET_H

#include <QWidget>
#include "project/project.h"
#include "libgraphtheory/typenames.h"

namespace GraphTheory
{
class Editor;
class GraphDocument;
}
class Project;
class QTabWidget;

class GraphEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphEditorWidget(QWidget *parent = nullptr);
    void setProject(Project *project);

Q_SIGNALS:
    void activeDocumentChanged(int index);

private Q_SLOTS:
    void onGraphDocumentAboutToBeAdded(GraphTheory::GraphDocumentPtr document, int index);
    void onGraphDocumentAboutToBeRemoved(int start, int end);

    /**
     * Show dialog to set name for document with index \p index
     */
    void showDocumentNameDialog(int index);

private:
    QTabWidget *m_viewWidgets;
    Project *m_project;
    GraphTheory::Editor *m_editor;
};

#endif
