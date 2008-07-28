/***************************************************************************
 *   Copyright (C) 2005-2006 by Anne-Marie Mahfouf   *
 *   tomaz.canabrava@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#ifndef GRAPH_ENGINE_H
#define GRAPH_ENGINE_H

#include <QtScript>

class DiagramScene;

class GraphEngine : public QScriptEngine
{
public:
	GraphEngine(DiagramScene *scene);
	void createNodeList();
	void createEdgeList();
	void createGroupsList();
	void createSelectedNodesList();
	void createSelectedEdgesList();
	void loadDefaultFile();
private:
	DiagramScene *scene;

};
#endif
