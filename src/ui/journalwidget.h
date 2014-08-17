/*
 *  Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

#ifndef JOURNALWIDGET_H
#define JOURNALWIDGET_H

#include <QWidget>
#include "ui_journalwidget.h"

class Project;

/**
 * \class JournalEditorWidget
 *
 * This class contains a UI to edit a project's journal file. The journal is presented as
 * a rich text editor window and saved as HTML.
 */
class JournalEditorWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Default constructor.
     */
    explicit JournalEditorWidget(QWidget* parent);

    /**
     * Loads journal from specified project \p project. File path and file name changes of the
     * journal file are automatically recognized by this. Call \see saveJournal() later to save.
     *
     * \param project the project for which the journal shall be added to the widget
     */
    void openJournal(Project* project);

    /**
     * \return true if journal is modified since loading, otherwise false
     */
    bool isModified() const;

private slots:
    /**
     * Set the internal state of the widget to modified.
     */
    void setModified();

private:
    Project* _currentProject;
    bool _modified;
    Ui::JournalEditorWidget *ui;
};

#endif
