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

void ThreadDocument::run(){
  _graphDocument = new GraphDocument(i18n("Untitled"), 800, 600);  
  _graphDocument->addGraph(i18n("Untitled0"));
  _engine = new QtScriptBackend((*_graphDocument));
  _docCondition.wakeAll();
  exec();
}


