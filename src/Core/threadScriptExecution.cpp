#include "threadScriptExecution.h"
#include "graphDocument.h"

ThreadScriptExecution::ThreadScriptExecution(const QString& script): QThread(), _script(script){}

ThreadScriptExecution::~ThreadScriptExecution(){

}

void ThreadScriptExecution::run(){

}

void ThreadScriptExecution::play(){
//     QtScriptBackend* engine = new QtScriptBackend( (*_activeGraphDocument) ,  _txtDebug);
//     QScriptEngineDebugger *e = new QScriptEngineDebugger(this);
//     e->attachTo(engine);
//     e->evaluate();
}

void ThreadScriptExecution::stop(){

}

void ThreadScriptExecution::pause(){

}