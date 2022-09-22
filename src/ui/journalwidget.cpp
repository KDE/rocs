/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include "journalwidget.h"
#include "project/project.h"
#include <KTextEditor/Document>
#include <QDebug>
#include <QFile>
#include <QSaveFile>
#include <QTextStream>
#include <QUrl>

JournalEditorWidget::JournalEditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_currentProject(nullptr)
    , m_modified(false)
{
    ui = new Ui::JournalEditorWidget;
    ui->setupUi(this);

    connect(ui->editor, &KRichTextWidget::textChanged, this, &JournalEditorWidget::setModified);
}

void JournalEditorWidget::openJournal(Project *project)
{
    m_currentProject = project;
    if (!project) {
        qCritical() << "No project specified! Cannot set journal widget.";
        return;
    }
    if (!project->journalDocument()) {
        qDebug() << "Skipping loading of journal file, project does not contain any, yet.";
        ui->editor->setHtml(QString());
    } else {
        ui->editor->setHtml(project->journalDocument()->text());
    }
    // explicitly set journal to be unmodified, since setting of text to editor caused modifed
    // value to be true
    m_modified = false;
}

void JournalEditorWidget::setModified()
{
    m_modified = true;
}

bool JournalEditorWidget::isModified() const
{
    return m_modified;
}
