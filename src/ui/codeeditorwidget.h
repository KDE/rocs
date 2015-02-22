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

#ifndef CODEEDITORWIDGET_H
#define CODEEDITORWIDGET_H

#include <QWidget>
#include "project/project.h"

namespace KTextEditor
{
class Document;
class View;
class Editor;
}
class QTabWidget;

class CodeEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CodeEditorWidget(QWidget *parent = 0);
    void setProject(Project *project);
    KTextEditor::Document *activeDocument() const;

Q_SIGNALS:
    void activeDocumentChanged(int index);

private Q_SLOTS:
    void onCodeDocumentAboutToBeAdded(KTextEditor::Document *document, int index);
    void onCodeDocumentAboutToBeRemoved(int start, int end);
    void showDocumentNameDialog(int index);

private:
    QTabWidget *m_viewWidgets;
    KTextEditor::Editor *m_editor;
    Project *m_project; //!< current project
};

#endif
