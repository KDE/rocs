#include "threadDocument.h"
#include "graphDocument.h"
#include "qtScriptBackend.h"
#include "qscriptenginedebugger.h"
#include <QMutex>
#include <KDebug>
#include <KLocale>
#include <QWaitCondition>
#include "DSPluginManager.h"
#include "DocumentManager.h"
#include <PluginManager.h>

ThreadDocument::ThreadDocument(QWaitCondition &docCondition, QMutex &mutex, QObject *parent):
	 QThread(parent),
	 _docCondition(docCondition),_mutex(mutex)
{
     m_docManager = 0;
     _graphDocument = 0;
     _engine = 0;
    // _loading = false;
    // _name = i18n("Untitled0");
}

ThreadDocument::~ThreadDocument(){
//     m_docManager->deleteLater();
//     Rocs::DSPluginManager::New()->deleteLater();
//     Rocs::PluginManager::New()->deleteLater();
//     if(_engine) delete _engine;
//     delete _graphDocument;

}

// ThreadDocument::setDocumentName(const QString& name){
//     _name = name;
// }

void ThreadDocument::terminate(){
  m_docManager->deleteLater();
  Rocs::DSPluginManager::New()->deleteLater();
  Rocs::PluginManager::New()->deleteLater();
  QThread::terminate();
}

bool ThreadDocument::isRunning() const{
    if (!engine()) return false;
    return engine()->isRunning();
}

GraphDocument* ThreadDocument::document() const
{
  return m_docManager->activeDocument();
}


QtScriptBackend *ThreadDocument::engine() const{

    return  m_docManager->activeDocument()->engineBackend();
}

// void ThreadDocument::releaseDocument(){
//    if (_graphDocument)
//        _graphDocument->deleteLater();
//     _graphDocument = 0;
//
//    if ( _engine )
//       _engine->deleteLater();
//     _engine = 0;
// }

// void ThreadDocument::createEmptyDocument(){
//   releaseDocument();
//   _graphDocument = new GraphDocument(i18n("Untitled"), 800, 600);
//   connect (Rocs::DSPluginManager::New(), SIGNAL(changingDS(QString)), _graphDocument, SLOT(convertToDS(QString)));
//   _engine = new QtScriptBackend();
//   _docCondition.wakeAll();
//   kDebug() << "Waking All";
// }

void ThreadDocument::loadDocument(const QString& name){
//     createEmptyDocument();
    if ( name.isEmpty() ){
      GraphDocument * doc = new GraphDocument( i18n ( "Untitled0" ));
      doc->addGraph ( i18n ( "Untitled0" ) );
      m_docManager->addDocument(doc);

    }else{
        m_docManager->activeDocument()->loadFromInternalFormat ( name );
    }
    _docCondition.wakeAll();
}

void ThreadDocument::setGraphDocument(GraphDocument * doc){
//   releaseDocument();
  m_docManager->addDocument(doc);
//   _graphDocument = doc;

//   _engine = new QtScriptBackend();
  _docCondition.wakeAll();
}




void ThreadDocument::run(){
  kDebug() << "############ Load Plugins ###############";
  Rocs::PluginManager::New()->loadPlugins();
  kDebug() << "############ Load DS Plugins ############";
  Rocs::DSPluginManager::New();
  kDebug() <<"End of loading plugins.";
  m_docManager = new DocumentManager();
  connect (Rocs::DSPluginManager::New(), SIGNAL(changingDS(QString)), m_docManager, SLOT(convertToDataStructure(QString)));
//   loadDocument();
  exec();
}