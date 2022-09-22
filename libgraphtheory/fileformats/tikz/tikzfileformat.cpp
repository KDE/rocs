/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "tikzfileformat.h"
#include "edge.h"
#include "graphdocument.h"
#include "logging_p.h"
#include "node.h"
#include <KAboutData>
#include <KLocalizedString>
#include <KPluginFactory>
#include <QFile>
#include <QTextStream>
#include <QUrl>

using namespace GraphTheory;

K_PLUGIN_CLASS_WITH_JSON(TikzFileFormat, "tikzfileformat.json")

TikzFileFormat::TikzFileFormat(QObject *parent, const KPluginMetaData &data, const QVariantList &)
    : FileFormatInterface(parent, data)
{
}

TikzFileFormat::~TikzFileFormat()
{
}

const QStringList TikzFileFormat::extensions() const
{
    return QStringList{i18n("TikZ (PGF) Format (%1)", QString("*.pgf"))};
}

FileFormatInterface::PluginType TikzFileFormat::pluginCapability() const
{
    return FileFormatInterface::ExportOnly;
}

void TikzFileFormat::readFile()
{
    qCWarning(GRAPHTHEORY_FILEFORMAT) << "This plugin cannot import documents.";
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
    const auto nodeTypes = graph->nodeTypes();
    for (int i = 0; i < nodeTypes.size(); ++i) {
        nodeTypeIdMap.insert(nodeTypes.at(i), i);
        // TODO set type specific style information
        QString dataTypeStyle = QString("\\tikzstyle{nodetype%1}=[circle,thin,fill=black!25,minimum size=20pt,inner sep=0pt]").arg(i);
        out << dataTypeStyle << "\n";
    }
    const auto edgeTypes = graph->edgeTypes();
    for (int i = 0; i < edgeTypes.size(); ++i) {
        EdgeTypePtr type = edgeTypes.at(i);
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
    // y-axis is mirrored in tikz-format
    for (const NodeTypePtr &type : nodeTypes) {
        const auto nodes = graph->nodes(type);
        for (const NodePtr &node : nodes) {
            QString nodeStr = QString("\\node[nodetype%1] (%2) at (%3,%4) [label=left:%5]  {%6};")
                                  .arg(nodeTypeIdMap[type])
                                  .arg(node->id())
                                  .arg(node->x() / resize)
                                  .arg(node->y() * (-1) / resize)
                                  .arg(node->property("name").toString())
                                  .arg(node->property("value").toString());
            out << nodeStr;
            out << '\n';
        }
    }

    // export pointers
    for (const EdgeTypePtr &type : edgeTypes) {
        const auto edges = graph->edges(type);
        for (const EdgePtr &edge : edges) {
            QString edgeStr = QString("\\path[edgetype%1] (%2) -- node[value] {%3} (%4);")
                                  .arg(edgeTypeIdMap[type])
                                  .arg(edge->from()->id())
                                  .arg(edge->property("value").toString())
                                  .arg(edge->to()->id());
            out << edgeStr;
            out << '\n';
        }
    }

    out << "\\end{tikzpicture}\n";

    setError(None);
}

#include "tikzfileformat.moc"
