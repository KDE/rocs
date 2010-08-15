/* This file is part of KGraphViewer.
   Copyright (C) 2006-2007 Gael de Chalendar <kleag@free.fr>

   KGraphViewer is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation, version 2.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA
*/

#ifndef DOT_GRAPHPARSINGHELPER_H
#define DOT_GRAPHPARSINGHELPER_H

#include <map>
#include <list>
#include <string>
#include <QStringList>
#include "Core/DataType.h"
#include "Core/Data.h"
#include "Core/graphDocument.h"
#include "Core/Pointer.h"
// class Node;
// class Graph;

#include <QObject>
namespace KGraphViewer
{
// class DotGraph;
// class GraphSubgraph;
// class GraphNode;
// class GraphEdge;
// class GraphElement;

struct DotGraphParsingHelper
{
  typedef std::map< std::string, std::string > AttributesMap;

  DotGraphParsingHelper();

  void createdatum(const std::string& nodeid);
  void createsubgraph();
  void setgraphattributes();
  void setsubgraphattributes();
      void setdatumattributes();
  void setedgeattributes();
  void setattributedlist();
  void createedges();
  void edgebound(const std::string& bound) {edgebounds.push_back(bound);}
  void finalactions();
  void setgraphelementattributes(QObject* ge, const KGraphViewer::DotGraphParsingHelper::AttributesMap& attributes);

  std::string attrid;
  std::string valid;
  std::string attributed;
  QStringList subgraphid;
//   std::string subgraphid;
  unsigned int uniq;

  AttributesMap attributes;
  AttributesMap graphAttributes;
  AttributesMap datumAttributes;
  AttributesMap pointersAttributes;
  std::list< AttributesMap > graphAttributesStack;
  std::list< AttributesMap > datumAttributesStack;
  std::list< AttributesMap > pointersAttributesStack;

  std::list< std::string > edgebounds;

  unsigned int z;
  unsigned int maxZ;

  DataType* graph;

//   GraphSubgraph* gs;
  Datum* gn;
  Pointer* ge;
    DataTypeDocument* gd;
};

}

#endif



