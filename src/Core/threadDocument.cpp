#include "threadDocument.h"
#include "graphDocument.h"
#include "qtScriptBackend.h"
#include "qscriptenginedebugger.h"
#include <QMutex>
#include <KTextBrowser>
// #include <QScriptEngineDebugger> //give-me a error ¬¬, realy need it?
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
    if (!_engine->engine()) return false;
    return _engine->engine()->isEvaluating();
}

QtScriptBackend *ThreadDocument::engine(){
    return _engine;
}

void ThreadDocument::run(){
  _graphDocument = new GraphDocument(i18n("Untitled"), 800, 600);  
  _graphDocument->addGraph(i18n("Untitled0"));
  _engine = new QtScriptBackend();
  _docCondition.wakeAll();
  exec();
}


