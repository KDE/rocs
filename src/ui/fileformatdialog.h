/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2009-2011 Tomaz Canabrava <tomaz.canabrava@gmail.com>
    SPDX-FileCopyrightText: 2010 Wagner Reck <wagner.reck@gmail.com>
    SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef FILEFORMATDIALOG_H
#define FILEFORMATDIALOG_H

#include "libgraphtheory/typenames.h"
#include <QObject>

class Document;

class FileFormatDialog : public QObject
{
    Q_OBJECT

public:
    explicit FileFormatDialog(QObject *parent = 0);
    GraphTheory::GraphDocumentPtr importFile();
    bool exportFile(GraphTheory::GraphDocumentPtr document) const;
    void dialogExec();
};

#endif
