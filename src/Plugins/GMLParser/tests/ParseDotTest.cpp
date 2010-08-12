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

#include "ParseDotTest.h"
#include <QTest>
#include <string>
#include <Core/graphDocument.h>
#include "../dotgrammar.h"
#include <Core/graph.h>
#include <Core/DynamicPropertiesList.h>
#include <KDebug>

static const std::string str_subgraph = "digraph trees {"
"  subgraph t {"
"    0 -> \"1\" [label = \"A\"];"
"    0 -> \"2\" [label = \"B\"];"
"  }"
"  subgraph u {"
"    Animal -> Cat [label = \"feline\"];"
"    Animal -> Dot [label = \"canine\"];"
"  }"
"}";

static const std::string str = "digraph GG {"
"node ["
"  fontsize = \"12\""
"];"
"    \"node5\" [ label=\"/usr/lib/libQtCore.so\", shape=\"ellipse\"];"
"    \"node6\" [ label=\"/usr/lib/libQtGui.so\", shape=\"ellipse\"];"
"    \"node0\" [ label=\"CCP\", shape=\"house\"];"
"    \"node3\" [ label=\"CCPAlgorithms\", shape=\"polygon\"];"
"    \"node4\" [ label=\"CCPClusterView\", shape=\"polygon\"];"
"    \"node2\" [ label=\"CCPIOLib\", shape=\"polygon\"];"
"    \"node1\" [ label=\"CCPModelLib\", shape=\"polygon\"];"
"    \"node0\" -> \"node5\""
"    \"node0\" -> \"node6\""
"    \"node0\" -> \"node1\""
"    \"node0\" -> \"node2\""
"    \"node0\" -> \"node3\""
"    \"node0\" -> \"node4\""
"    \"node2\" -> \"node5\""
"    \"node4\" -> \"node5\""
"    \"node4\" -> \"node6\""
"    \"node4\" -> \"node1\""
"}";

void ParseDotTest::parseCMakeGenerated()
{
    GraphDocument doc("A test");
    QVERIFY (parse(str, &doc));
}
void ParseDotTest::WithSubgraph()
{
    GraphDocument doc("A test");
    QVERIFY (parse(str_subgraph, &doc));
}


QTEST_MAIN ( ParseDotTest )
#include <ParseDotTest.moc>