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

#include <KUrl>
#include <KSaveFile>
#include <KDebug>

#include <QTextStream>
#include <QFile>


JournalEditorWidget::JournalEditorWidget(QWidget* parent)
    : QWidget(parent)
    , _currentProject(0)
{
    ui = new Ui::JournalEditorWidget;
    ui->setupUi(this);

    connect(ui->editor, SIGNAL(textChanged()), this, SLOT(setModified()));
}

void JournalEditorWidget::openJournal(Project* project)
{
    _modified = false;
    if (!project) {
        kError() << "No project specified! Cannot set journal widget.";
        return;
    }
    if (project->journalFile().isEmpty()) {
        kDebug() << "Skipping loading of journal file, project does not contain any, yet.";
        return;
    }
    _currentProject = project;
    QFile fileHandle(project->journalFile().toLocalFile());
    if (!fileHandle.open(QIODevice::ReadOnly | QIODevice::Text)) {
        kError() << "Could not open file"
            << project->journalFile().toLocalFile()
            << " in read mode: "
            << fileHandle.errorString();
            return;
    }
    QTextStream stream(&fileHandle);
    stream.setCodec("UTF-8");
    ui->editor->setHtml(stream.readAll());
}

void JournalEditorWidget::saveJournal()
{
    if (!_currentProject) {
        return;
    }
    KSaveFile saveFile(_currentProject->journalFile().toLocalFile());
    saveFile.open();

    QTextStream stream(&saveFile);
    stream.setCodec("UTF-8");
    stream << ui->editor->toHtml();
    stream.flush();
    if (!saveFile.finalize()) {
        kError() << "Error while writing journal file, aborting write. Journal file was not changed.";
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
