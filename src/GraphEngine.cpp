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

#include "GraphEngine.h"
#include "DiagramEdge.h"
#include "DiagramNode.h"
#include "DiagramScene.h"

#include <kstandarddirs.h>
GraphEngine::GraphEngine(DiagramScene *scene) : QScriptEngine()
{
  this -> scene = scene;
  //! Create the nodes and edges array.
  createNodeList();
  createEdgeList();
  createGroupsList();
  createSelectedNodesList();
  createSelectedEdgesList();
 
   //! Create the scene object.
   QScriptValue script_scene =  newQObject(scene);
   globalObject().setProperty("scene", script_scene);

   // load the default file.
   loadDefaultFile();
}

void GraphEngine::loadDefaultFile()
{
  QString path = KStandardDirs::locate("appdata", "GraphScript.js");
  QFile graph_file(path);
  
  if (!graph_file.exists()){ scene->debug(path); return; }
  QString initialProgram = "";

  if (!graph_file.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

  QTextStream in(&graph_file);
  while (!in.atEnd()) {
      initialProgram += in.readLine();
  initialProgram += "\n";
  }
  evaluate(initialProgram);

}

void GraphEngine::createNodeList()
{
   QScriptValue nodeList =  newArray();              //! All Nodes
   globalObject().setProperty("nodes", nodeList);
  
  //! Puts every Node inside of the scene on the edges array;
  QList<Node*> nodes = scene->getNodes();
  foreach(Node *n, nodes){
    QScriptValue node =  newQObject(n);
    nodeList.property("push").call(nodeList, QScriptValueList() << node);
    if (n -> isStartPoint )
  {
    globalObject().setProperty("start_node", node);
    }
    if (n -> isEndPoint )
  {
    globalObject().setProperty("end_node", node);
  }
  }  
}
  
void GraphEngine::createEdgeList()
{
   QScriptValue edgeList =  newArray();              //! All Edges
   globalObject().setProperty("edges", edgeList);
   //! Puts every Edge inside of the scene on the edges array;
  QList<Edge*> edges = scene->getEdges();
  foreach(Edge *e, edges){
    QScriptValue edge =  newQObject(e);
    edgeList.property("push").call(edgeList, QScriptValueList() << edge);
  }
}

//! TODO: fazer esse aqui =p
void GraphEngine::createGroupsList()
{
	QScriptValue groupList =  newArray();              //! All Edges
   globalObject().setProperty("groups", groupList);
   //! Puts every Edge inside of the scene on the edges array;
  QList<Group*> group = scene->getGroups();
  foreach(Group *g, group){
    QScriptValue group =  newQObject(g);
    groupList.property("push").call(groupList, QScriptValueList() << group);
  }
}

void GraphEngine::createSelectedNodesList()
{
    QScriptValue selected_nodes = newArray();        //! Selected Nodes
    globalObject().setProperty("selected_nodes", selected_nodes);
  
  //! Puts every Edge inside of the scene on the edges array;
  int length =  evaluate("nodes.length;").toInt32();
  QList<Node*> selectedNodes = scene->getSelectedNodes();
  foreach(Node *_n, selectedNodes)
  {
    for(int i = 0; i < length; i++)
    {
      QString n = "nodes["+QString::number(i)+"];";
      QScriptValue node =   evaluate(n);
      Node *_x = qobject_cast<Node*>(node.toQObject());

      if (_n == _x)
      {
        selected_nodes.property("push").call(selected_nodes, QScriptValueList() << node);
      } 
    } 
  }
}

void GraphEngine::createSelectedEdgesList()
{
  QScriptValue selected_edges =  newArray();        //! Selected Edges
    globalObject().setProperty("selected_edges", selected_edges);

  //! Puts every Edge inside of the scene on the edges array;
  int length =  evaluate("edges.length;").toInt32();
  QList<Edge*> selectedEdges = scene->getSelectedEdges();
  
  foreach(Edge *_e, selectedEdges)
  {
    for(int i = 0; i < length; i++)
    {
      QString n = "edges["+QString::number(i)+"];";
      QScriptValue edge =   evaluate(n);
      Edge *x = qobject_cast<Edge*>(edge.toQObject());

      if (x == _e)
      {
        selected_edges.property("push").call(selected_edges, QScriptValueList() << edge);
      } 
    } 
  }
}
