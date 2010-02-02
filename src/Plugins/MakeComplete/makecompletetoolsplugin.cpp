/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "makecompletetoolsplugin.h"
#include <KLocalizedString>
#include <QString>
#include <QStringList>


QString MakeCompleteToolsPlugin::run(QWidget* /*parent*/) const
{
  return QString (
      "function makeComplete(graph){"
      "  nodes = graph.list_nodes();"
      "  edges = graph.list_edges();"
      "  for (var e = 0; e < edges.length; e++){"
      "      graph.remove(edges[e]);"
      "  }"
      "  for (var i = 0; i < nodes.length; i++){"
      "      for (var j = i+1; j < nodes.length; j++){"
      "          graph.add_edge(nodes[i], nodes[j]);"
      "      }"
      "  }"
      "}"
      "for (var k = 0; k < graphs.length; k++){"
      "   makeComplete(graphs[k]);"
      "}" 
     );
}

QStringList MakeCompleteToolsPlugin::authors() const
{
  return QStringList() << i18n("Wagner Reck - wagner.reck@gmail.com");
}

QString MakeCompleteToolsPlugin::category()
{
  return i18n("Add Components");
}

QString MakeCompleteToolsPlugin::about() const
{
  return i18n("remove all edges of the graphs and add news one to make graphs completes");
}

QString MakeCompleteToolsPlugin::tooltip() const
{
  return i18n("Add edges and make graph complete.");
}

QString MakeCompleteToolsPlugin::name() const
{
  return i18n("Make Complete");
}

Q_EXPORT_PLUGIN2(makeCompleteToolsPlugin, MakeCompleteToolsPlugin);