/*
 *  Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

#include "tikzfileformat.h"
#include "fileformats/fileformatinterface.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include <KAboutData>
#include <KPluginFactory>
#include <KLocalizedString>
#include <QTextStream>
#include <QFile>
#include <QUrl>
#include <QDebug>

using namespace GraphTheory;

K_PLUGIN_FACTORY_WITH_JSON( FilePluginFactory,
                            "tikzfileformat.json",
                            registerPlugin<TikzFileFormat>();)

TikzFileFormat::TikzFileFormat(QObject* parent, const QList< QVariant >&)
    : FileFormatInterface("rocs_tgffileformat", parent)
{
}

TikzFileFormat::~TikzFileFormat()
{
}

const QStringList TikzFileFormat::extensions() const
{
    return QStringList()
           << i18n("TikZ (PGF) Format (%1)", QString("*.pgf"));
}

FileFormatInterface::PluginType TikzFileFormat::pluginCapability() const
{
    return FileFormatInterface::ExportOnly;
}

void TikzFileFormat::readFile()
{
    qWarning() << "This plugin cannot import documents.";
    setError(NotSupportedOperation);
}

void TikzFileFormat::writeFile(GraphDocumentPtr graph)
{
    // TODO allow selection which data structure shall be exported
    QFile fileHandle(file().toLocalFile());
    if (!fileHandle.open(QFile::WriteOnly | QFile::Text)) {
        setError(FileIsReadOnly, i18n("Could not open file \"%1\" in write mode: %2", file().fileName(), fileHandle.errorString()));
        return;
    }

    QTextStream out(&fileHandle);
    QMap<NodeTypePtr, int> nodeTypeIdMap;
    QMap<EdgeTypePtr, int> edgeTypeIdMap;

    // use this value to scale all coordinates
    int resize = 50;

    // start picture
    out << "\\begin{tikzpicture}\n";

    // style information
    out << "\\tikzstyle{value} = [font=\\small]\n";
    for (int i = 0; i < graph->nodeTypes().length(); ++i) {
        nodeTypeIdMap.insert(graph->nodeTypes().at(i), i);
        // TODO set type specific style information
        QString dataTypeStyle = QString("\\tikzstyle{nodetype%1}=[circle,thin,fill=black!25,minimum size=20pt,inner sep=0pt]").
            arg(i);
        out << dataTypeStyle << "\n";
    }
    for (int i = 0; i < graph->edgeTypes().length(); ++i) {
        EdgeTypePtr type = graph->edgeTypes().at(i);
        edgeTypeIdMap.insert(type, i);

        // set style attributes
        QString styleAttributes = QString("draw,thick");

        // direction
        if (type->direction() == EdgeType::Unidirectional) {
            styleAttributes.append(",->");
        } else {
            styleAttributes.append(",-");
        }

        // set style
        QString pointerTypeStyle = QString("\\tikzstyle{edgetype%1} = [%2]").arg(i).arg(styleAttributes);
        out << pointerTypeStyle << "\n";
    }

    // export data elements
    // y-axis is mirrowed in tikz-format
    foreach(NodeTypePtr type, graph->nodeTypes()) {
        foreach(NodePtr node, graph->nodes(type)) {
            QString nodeStr = QString("\\node[nodetype%1] (%2) at (%3,%4) [label=left:%5]  {%6};").
                arg(nodeTypeIdMap[type]).
                arg(node->id()).
                arg(node->x()/resize).
                arg(node->y()*(-1)/resize).
                arg(node->property("name").toString()).
                arg(node->property("value").toString());
            out << nodeStr;
            out << '\n';
        }
    }

    // export pointers
    foreach(EdgeTypePtr type, graph->edgeTypes()) {
        foreach(EdgePtr edge, graph->edges(type)) {
            QString edgeStr = QString("\\path[edgetype%1] (%2) -- node[value] {%3} (%4);").
                arg(edgeTypeIdMap[type]).
                arg(edge->from()->id()).
                arg(edge->property("value").toString()).
                arg(edge->to()->id());
            out << edgeStr;
            out << '\n';
        }
    }

    out << "\\end{tikzpicture}\n";

    setError(None);
}

#include "tikzfileformat.moc"
