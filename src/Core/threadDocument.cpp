#include "threadDocument.h"
#include "graphDocument.h"
#include "qtScriptBackend.h"
#include "qscriptenginedebugger.h"
#include <QMutex>
#include <KDebug>
#include <KLocale>
#include <QWaitCondition>

ThreadDocument::ThreadDocument(QWaitCondition &docCondition, QMutex &mutex, QObject *parent):
	 QThread(parent),
	 _docCondition(docCondition),_mutex(mutex)
{
     _graphDocument = 0;
     _engine = 0;
}

ThreadDocument::~ThreadDocument(){
    if(_engine) delete _engine;
}

bool ThreadDocument::isRunning(){
    if (!_engine) return false;
    return _engine->isRunning();
//     return _engine->engine()->isEvaluating();
}

QtScriptBackend *ThreadDocument::engine(){
    return _engine;
}

void ThreadDocument::releaseDocument(){
    delete _graphDocument;
    _graphDocument = 0;
}

void ThreadDocument::setGraphDocument(GraphDocument * doc){
  // must need block signals and events
  
  if (_engine){ 
	delete _engine;
  }
    
  releaseDocument(); //must 'freeze' thread
    
  _graphDocument = doc;
  doc->moveToThread(this);
  
  _engine = new QtScriptBackend();
  _docCondition.wakeAll();
}

void ThreadDocument::run(){
  _graphDocument = new GraphDocument(i18n("Untitled"), 800, 600);  
  _graphDocument->addGraph(i18n("Untitled0"));
  _engine = new QtScriptBackend();
  _docCondition.wakeAll();
  exec();
}


