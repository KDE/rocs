/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "rocs2fileformat.h"
#include "fileformats/fileformatinterface.h"
#include "modifiers/topology.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include <KLocalizedString>
#include <KPluginFactory>
#include <QFile>
#include <QUrl>

using namespace GraphTheory;

K_PLUGIN_FACTORY_WITH_JSON( FilePluginFactory,
                            "rocs2fileformat.json",
                            registerPlugin<Rocs2FileFormat>();)

Rocs2FileFormat::Rocs2FileFormat(QObject* parent, const QList< QVariant >&)
    : FileFormatInterface("rocs_tgffileformat", parent)
{
}

Rocs2FileFormat::~Rocs2FileFormat()
{
}

const QStringList Rocs2FileFormat::extensions() const
{
    return QStringList()
           << i18n("Rocs Graph Format (%1)", QString("*.graph2")); // do not confuse with Rocs-1 format
}


void Rocs2FileFormat::readFile()
{
    GraphDocumentPtr document = GraphDocument::create();

    //TODO read
    
    setGraphDocument(document);
    setError(None);
}

void Rocs2FileFormat::writeFile(GraphDocumentPtr document)
{
    QFile fileHandle(file().toLocalFile());
    if (!fileHandle.open(QFile::WriteOnly | QFile::Text)) {
        setError(FileIsReadOnly, i18n("Could not open file \"%1\" in write mode: %2", file().fileName(), fileHandle.errorString()));
        return;
    }
    //TODO serialization
    setError(None);
}

#include "rocs2fileformat.moc"
