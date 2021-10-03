/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
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

private Q_SLOTS:
    /**
     * Set the internal state of the widget to modified.
     */
    void setModified();

private:
    Project *m_currentProject;
    bool m_modified;
    Ui::JournalEditorWidget *ui;
};

#endif
