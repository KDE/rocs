/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "TgfFileFormatPlugin.h"
#include "Document.h"
#include <KAboutData>
#include <KGenericFactory>
#include <KUrl>
#include <QFile>
#include <DataStructure.h>
#include <Data.h>
#include <Pointer.h>


static const KAboutData aboutdata("rocs_tgffileformat", 0, ki18n("Open and Save Graph Documents in Trivial Graph Format") , "0.1");

K_PLUGIN_FACTORY(FilePluginFactory, registerPlugin<TgfFileFormatPlugin>();)
K_EXPORT_PLUGIN(FilePluginFactory(aboutdata))


TgfFileFormatPlugin::TgfFileFormatPlugin(QObject* parent, const QList< QVariant >&) :
    GraphFilePluginInterface(FilePluginFactory::componentData(), parent)
{

}

TgfFileFormatPlugin::~TgfFileFormatPlugin()
{
}

const QStringList TgfFileFormatPlugin::extensions() const
{
    return QStringList()
           << i18n("*.tgf|Trivial Graph Format") + '\n';
}


void TgfFileFormatPlugin::readFile()
{
    Document * graphDoc = new Document("Untitled");
    //TODO select graph data structure
    DataStructurePtr graph = graphDoc->addDataStructure();
    QList < QPair<QString, QString> > edges;
    QFile fileHandle(file().toLocalFile());
    if (!fileHandle.open(QFile::ReadOnly)) {
        setError(CouldNotOpenFile, i18n("Could not open file \"%1\" in read mode: %2", file().toLocalFile(), fileHandle.errorString()));
        delete graphDoc;
        return;
    }
    while (!fileHandle.atEnd()) {
        QStringList list = QString(fileHandle.readLine()).trimmed().split(' ', QString::SkipEmptyParts);
        switch (list.count()) {
        case 1:
            graph->addData(list.at(0));
            break;
        case 2:
            edges << QPair<QString, QString> (list.at(0), list.at(1)) ;
            break;
        case 3: {
            DataPtr d = graph->addData(list.at(0));
            d->setX(list.at(1).toInt());
            d->setY(list.at(2).toInt());
            break;
        }
        default:
            kDebug() << QString("Ignoring line: %1").arg(list.join(" "));
            break;

        }
    }
    for (int i = 0; i < edges.count(); ++i) {
        graph->addPointer(edges.at(i).first, edges.at(i).second);
        kDebug() << edges.at(i);
    }
    setError(None);
}

void TgfFileFormatPlugin::writeFile(Document &graph )
{
    // TODO allow selection which data structure shall be exported
    QFile fileHandle(file().toLocalFile());
    if (!fileHandle.open(QFile::WriteOnly | QFile::Text)) {
        setError(FileIsReadOnly, i18n("Could not open file \"%1\" in write mode: %2", file().fileName(), fileHandle.errorString()));
        return;
    }

    QTextStream out(&fileHandle);
    DataStructurePtr g = graph.activeDataStructure();
    if (!g) {
        setError(NoGraphFound, i18n("No data structure specified for output in this document."));
        return;
    }

    // export data elements
    foreach(DataPtr n, g->dataList()) {
        out << n->identifier();
        out << " ";
        out << n->name();
        out << '\n';
    }
    out << "#\n";
    // export pointers
    foreach(PointerPtr e, g->pointers()) {
        out << e->from()->identifier() << " " << e->to()->identifier() << " " << e->value() <<'\n';
    }
    setError(None);
}
