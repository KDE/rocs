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
#include <Project.h>
#include "ui_JournalEditorWidget.h"

#include <QSaveFile>
#include <QDebug>

#include <QTextStream>
#include <QFile>
#include <QUrl>


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
    if (project->journalFile().isEmpty()) {
        qDebug() << "Skipping loading of journal file, project does not contain any, yet.";
        ui->editor->setHtml(QString());
    } else {
        QFile fileHandle(project->journalFile().toLocalFile());
        if (!fileHandle.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCritical() << "Could not open file"
                << project->journalFile().toLocalFile()
                << " in read mode: "
                << fileHandle.errorString();
                return;
        }
        QTextStream stream(&fileHandle);
        stream.setCodec("UTF-8");
        ui->editor->setHtml(stream.readAll());
    }
    // explicitly set journal to be unmodified, since setting of text to editor caused modifed
    // value to be true
    _modified = false;
}

void JournalEditorWidget::saveJournal()
{
    Q_ASSERT(_currentProject);
    if (!_currentProject) {
        qCritical() << "Associated project is not set: cannot save journal file and hence aborting saving.";
        qDebug() << "Journal must be created with JournalEditorWidget::openJournal(...).";
        return;
    }

    QSaveFile saveFile(_currentProject->journalFile().toLocalFile());
    saveFile.open(QIODevice::WriteOnly);

    QTextStream stream(&saveFile);
    stream.setCodec("UTF-8");
    stream << ui->editor->toHtml();
    stream.flush();
    if (!saveFile.commit()) {
        qCritical() << "Error while writing journal file, aborting write. Journal file was not changed.";
    }
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
