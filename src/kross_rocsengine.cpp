#include "kross_rocsengine.h"
#include "libgraph_GraphDocument.h"
#include "libgraph_Graph.h"
#include <QList>
#include <KDebug>

kross_rocsengine::kross_rocsengine(){

}

void kross_rocsengine::setDocument(libgraph::GraphDocument *gd){
  _graphDocument = gd;
}

void kross_rocsengine::setEngine(const QString& engine){
  _engine = engine;
}


void kross_rocsengine::execute(const QString& script){
  Kross::Action action(0, "myScript");

  action.setInterpreter(_engine);

  QList<libgraph::Graph*> graphList = _graphDocument->graphs();

 // action.addObject( graphList, "graphs");
  foreach(libgraph::Graph* g, graphList){
    action.addObject( g, g->name());
  }

  if(_engine == "python")          configureForPython(action);
  else if(_engine == "ruby")       configureForRuby(action);
  else if(_engine == "javascript") configureForJavaScript(action);
  
  action.setCode( script.toAscii() );

  action.trigger();
}

void kross_rocsengine::configureForJavaScript(Kross::Action& action){
  
}

void kross_rocsengine::configureForPython(Kross::Action& action){

}

void kross_rocsengine::configureForRuby(Kross::Action& action){

}