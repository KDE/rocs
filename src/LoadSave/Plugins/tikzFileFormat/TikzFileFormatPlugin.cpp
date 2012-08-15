/*
    This file is part of Rocs.
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

#include "TikzFileFormatPlugin.h"
#include "Document.h"
#include <KAboutData>
#include <KGenericFactory>
#include <KUrl>
#include <QFile>
#include <DataStructure.h>
#include <Data.h>
#include <Pointer.h>
#include <Modifiers/Topology.h>
#include <DataStructurePluginManager.h>


static const KAboutData aboutdata("rocs_tikzfileformat", 0, ki18n("Open and Save Graph Documents in PGF/TikZ Format") , "0.1");

K_PLUGIN_FACTORY(FilePluginFactory, registerPlugin<TikzFileFormatPlugin>();)
K_EXPORT_PLUGIN(FilePluginFactory(aboutdata))


TikzFileFormatPlugin::TikzFileFormatPlugin(QObject* parent, const QList< QVariant >&) :
    GraphFilePluginInterface(FilePluginFactory::componentData(), parent)
{

}

TikzFileFormatPlugin::~TikzFileFormatPlugin()
{
}

const QStringList TikzFileFormatPlugin::extensions() const
{
    return QStringList()
           << i18n("*.pgf|PGF TikZ format") + '\n';
}


GraphFilePluginInterface::PluginType TikzFileFormatPlugin::pluginCapability() const
{
    return GraphFilePluginInterface::ExportOnly;
}


void TikzFileFormatPlugin::readFile()
{
    kWarning() << "This plugin cannot import documents.";
    setError(NotSupportedOperation);
}


void TikzFileFormatPlugin::writeFile(Document &graph )
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

    // use this value to scale all coordinates
    int resize = 50;

    // start picture
    out << "\\begin{tikzpicture}\n";

    // style information
    out << "\\tikzstyle{value} = [font=\\small]\n";
    foreach(int type, graph.dataTypeList()) {
        // TODO set type specific style information
        QString dataTypeStyle = QString("\\tikzstyle{data%1}=[circle,thin,fill=black!25,minimum size=20pt,inner sep=0pt]").
            arg(type);
        out << dataTypeStyle << "\n";
    }
    foreach(int type, graph.pointerTypeList()) {
        // TODO set type specific style information
        QString pointerTypeStyle = QString("\\tikzstyle{edge%1} = [draw,thick,-]").
            arg(type);
        out << pointerTypeStyle << "\n";
    }

    // export data elements
    // y-axis is mirrowed in tikz-format
    foreach(int type, graph.dataTypeList()) {
        foreach(DataPtr n, g->dataList(type)) {
            QString dataStr = QString("\\node[data%1] (%2) at (%3,%4) [label=left:%5]  {%6};").
                arg(n->dataType()).
                arg(n->identifier()).
                arg(n->x()/resize).
                arg(n->y()*(-1)/resize).
                arg(n->name()).
                arg(n->value().toString());
            out << dataStr;
            out << '\n';
        }
    }

    // export pointers
    foreach(int type, graph.pointerTypeList()) {
        foreach(PointerPtr e, g->pointers(type)) {
            QString pointerStr = QString("\\path[edge%1] (%2) -- node[value] {%3} (%4);").
                arg(e->pointerType()).
                arg(e->from()->identifier()).
                arg(e->value()).
                arg(e->to()->identifier());
            out << pointerStr;
            out << '\n';
        }
    }

    out << "\\end{tikzpicture}\n";

    setError(None);
}
