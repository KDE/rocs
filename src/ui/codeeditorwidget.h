/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
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
    explicit CodeEditorWidget(QWidget *parent = nullptr);
    void setProject(Project *project);
    KTextEditor::Document *activeDocument() const;

Q_SIGNALS:
    void activeDocumentChanged(int index);

private Q_SLOTS:
    void onCodeDocumentAboutToBeAdded(KTextEditor::Document *document, int index);
    void onCodeDocumentAboutToBeRemoved(int start, int end);
    void showDocumentNameDialog(int index);
    void closeTab(int index);

private:
    QTabWidget *m_viewWidgets;
    KTextEditor::Editor *m_editor;
    Project *m_project; //!< current project
};

#endif
