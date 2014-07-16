/*
    This file is part of Rocs.
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "JournalEditorWidget.h"
#include "project/project.h"
#include "ui_JournalEditorWidget.h"

#include <QSaveFile>
#include <QDebug>

#include <QTextStream>
#include <QFile>
#include <QUrl>
#include <KTextEditor/Document>


JournalEditorWidget::JournalEditorWidget(QWidget* parent)
    : QWidget(parent)
    , _currentProject(0)
{
    ui = new Ui::JournalEditorWidget;
    ui->setupUi(this);

    connect(ui->editor, SIGNAL(textChanged()), this, SLOT(setModified()));
}

void JournalEditorWidget::openJournal(Project *project)
{
    _currentProject = project;
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
    _modified = false;
}

void JournalEditorWidget::setModified()
{
    _modified = true;
}

bool JournalEditorWidget::isModified() const
{
    return _modified;
}
