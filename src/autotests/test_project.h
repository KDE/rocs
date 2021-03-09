/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef TESTPROJECT_H
#define TESTPROJECT_H

#include <QObject>

class TestProject : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void projectOperations();
    void loadSave();
    void loadSaveMultipleGraphDocuments();
    void loadSaveMultipleScriptDocuments();
    /** no graph document exists in project **/
    void loadBrokenFilesWithoutCrashing01();
};

#endif
