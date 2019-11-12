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
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
    explicit GraphEditorWidget(QWidget *parent = 0);
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
