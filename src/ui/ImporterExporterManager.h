/*
    This file is part of Rocs.
    Copyright 2009-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2014       Andreas Cord-Landwehr <cordlandwehr@kde.org>

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

#ifndef IMPORTEREXPORTERMANAGER_H
#define IMPORTEREXPORTERMANAGER_H

#include "libgraphtheory/typenames.h"
#include <QObject>

class Document;

class ImporterExporterManager : public QObject
{
    Q_OBJECT

public:
    explicit ImporterExporterManager(QObject *parent = 0);
    GraphTheory::GraphDocumentPtr importFile();
    bool exportFile(GraphTheory::GraphDocumentPtr document) const;
    void dialogExec();
};

#endif
