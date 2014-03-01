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
#include <DataStructureBackendManager.h>

//FIXME
// static const KAboutData aboutdata("rocs_tikzfileformat",
//                                   0,
//                                   ki18nc("@title Displayed plugin name", "PGF/TikZ Export File Backend"),
//                                   "0.1",
//                                   ki18n("Export graph documents in PGF/TikZ format."),
//                                   KAboutData::License_GPL_V2);

K_PLUGIN_FACTORY(FilePluginFactory, registerPlugin<TikzFileFormatPlugin>();)

TikzFileFormatPlugin::TikzFileFormatPlugin(QObject* parent, const QList<QVariant>&) :
    GraphFilePluginInterface("rocs_tikzfileformat", parent)
{
}

TikzFileFormatPlugin::~TikzFileFormatPlugin()
{
}

const QStringList TikzFileFormatPlugin::extensions() const
{
    return QStringList()
           << i18n("*.pgf|TikZ (PGF) Format") + '\n';
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


void TikzFileFormatPlugin::writeFile(Document &graph)
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
        // set style attributes
        QString styleAttributes = QString("draw,thick");

        // direction
        if (graph.pointerType(type)->direction() == PointerType::Unidirectional) {
            styleAttributes.append(",->");
        } else {
            styleAttributes.append(",-");
        }

        // line style
        switch(graph.pointerType(type)->lineStyle()) {
        case Qt::SolidLine:
            styleAttributes.append(",solid");
            break;
        case Qt::DashLine:
            styleAttributes.append(",dashed");
            break;
        case Qt::DotLine:
            styleAttributes.append(",dotted");
            break;
        default:
            styleAttributes.append(",solid");
        }

        // set style
        QString pointerTypeStyle = QString("\\tikzstyle{edge%1} = [%2]").arg(type).arg(styleAttributes);
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
                arg(n->property("name").toString()).
                arg(n->property("value").toString());
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
                arg(e->property("value").toString()).
                arg(e->to()->identifier());
            out << pointerStr;
            out << '\n';
        }
    }

    out << "\\end{tikzpicture}\n";

    setError(None);
}

#include "TikzFileFormatPlugin.moc"
