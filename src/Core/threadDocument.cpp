#include "threadDocument.h"
#include "graphDocument.h"
#include "qtScriptBackend.h"
#include "qscriptenginedebugger.h"
#include <QMutex>
#include <KDebug>
#include <KLocale>
#include <QWaitCondition>
#include "DSPluginManager.h"

ThreadDocument::ThreadDocument(QWaitCondition &docCondition, QMutex &mutex, QObject *parent):
	 QThread(parent),
	 _docCondition(docCondition),_mutex(mutex)
{
     _graphDocument = 0;
     _engine = 0;
    // _loading = false;
    // _name = i18n("Untitled0");
}

ThreadDocument::~ThreadDocument(){
    if(_engine) delete _engine;
    delete _graphDocument;
}

// ThreadDocument::setDocumentName(const QString& name){
//     _name = name;
// }
bool ThreadDocument::isRunning(){
    if (!_engine) return false;
    return _engine->isRunning();
}

QtScriptBackend *ThreadDocument::engine(){
    return _engine;
}

void ThreadDocument::releaseDocument(){
   if (_graphDocument)
       _graphDocument->deleteLater();
    _graphDocument = 0;

   if ( _engine )
      _engine->deleteLater();
    _engine = 0;
}

void ThreadDocument::createEmptyDocument(){
  releaseDocument();
  _graphDocument = new GraphDocument(i18n("Untitled"), 800, 600);
  connect (Rocs::DSPluginManager::New(), SIGNAL(changingDS(QString)), _graphDocument, SLOT(convertToDS(QString)));
  _engine = new QtScriptBackend();
  _docCondition.wakeAll();
  kDebug() << "Waking All";
}

void ThreadDocument::loadDocument(const QString& name){
    createEmptyDocument();
    if ( name.isEmpty() ){
        _graphDocument->addGraph ( i18n ( "Untitled0" ) );
    }else{
        _graphDocument->loadFromInternalFormat ( name );
    }
    _docCondition.wakeAll();
}

void ThreadDocument::setGraphDocument(GraphDocument * doc){
  releaseDocument();
  _graphDocument = doc;
  doc->moveToThread(this);
  _engine = new QtScriptBackend();
  _docCondition.wakeAll();
}




void ThreadDocument::run(){

  Rocs::DSPluginManager::New();

  loadDocument();
  exec();
}