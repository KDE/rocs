/***************************************************************************
 * main.cpp
 * This file is part of the KDE project
 * copyright (C)2004-2007 by Tomaz Canabrava (tomaz.canabrava@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/

#include "krossBackend.h"
#include "graph.h"
#include "node.h"
#include <QDebug>
#include <QTextCodec>
#include <KTextBrowser>
#include "graphDocument.h"

QtScriptBackend::QtScriptBackend(GraphDocument *graphs, KTextBrowser *debugArea)
{
	_debugArea = debugArea;
	_graphs = graphs;
	foreach(Graph *g, (*_graphs) ){
		g -> setEngine(this);
	}
	createGraphList();
}

void QtScriptBackend::setScript(const QString& s){
	_script = s;
}

void QtScriptBackend::createGraphList(){
	QScriptValue graphList = newArray();
	globalObject().setProperty("graphs", graphList);

	// Add all the graphs on the array as an array, and if it has a name, 
	// also add it for direct acess with it's name.
	foreach(Graph *g, (*_graphs) ){
		graphList.property("push").call(graphList, QScriptValueList() << g->scriptValue());
	}
}

void QtScriptBackend::loadFile(const QString& file){
	_script.clear();
	QFile f(file);
	if  (  !f.open(QIODevice::ReadOnly | QIODevice::Text ) ){
		qDebug() << "File not found";
		return;
	}
  
	while( ! f.atEnd() ){
		QByteArray line = f.readLine();
		_script += line;
	}
	_script += "\n";
}
