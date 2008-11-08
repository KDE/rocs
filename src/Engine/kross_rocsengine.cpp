/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "kross_rocsengine.h"
#include "GraphDocument.h"
#include "Graph.h"
#include <QList>
#include <KDebug>
#include <KTextBrowser>

kross_rocsengine::kross_rocsengine(KTextBrowser *debugArea){
  _debugArea = debugArea;
}

void kross_rocsengine::setDocument( GraphDocument *gd){
  _graphDocument = gd;
}

void kross_rocsengine::setEngine(const QString& engine){
  _engine = engine;
}


void kross_rocsengine::execute(const QString& script){
  Kross::Action action(0, "myScript");

  action.setInterpreter(_engine);

  QList< Graph*> graphList = _graphDocument->graphs();

 // action.addObject( graphList, "graphs");
  foreach(Graph* g, graphList){
    action.addObject( g, g->name());
  }

  foreach(const QString& s, Kross::Manager::self().interpreters()){
    kDebug() << "BACKEND FOUND" << s;
  }
  
  action.addObject( _debugArea, "debug");

  QString codeToBeExecuted = "";
  if(_engine == "python"){
    foreach( Graph* g, graphList){
      codeToBeExecuted += "import " + g->name() + " \n";
    }
    kDebug() << "Python imports : " << codeToBeExecuted;
  }
  else if(_engine == "ruby"){
    foreach( Graph* g, graphList){
      codeToBeExecuted += "require '" + g->name() + "' \n";
    }
    kDebug() << "ruby requires : " << codeToBeExecuted;
  }
    
  codeToBeExecuted += script;
  action.setCode( codeToBeExecuted.toAscii() );

  action.trigger();
  kDebug()  << "Should Have Worked";
}
