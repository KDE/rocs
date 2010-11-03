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
//      m_docManager = 0;
     _dataTypeDocument = 0;
     _engine = 0;
    // _loading = false;
    // _name = i18n("Untitled0");

}

ThreadDocument::~ThreadDocument(){
//     m_docManager->deleteLater();
//     Rocs::DSPluginManager::instance()->deleteLater();
//     Rocs::PluginManager::instance()->deleteLater();
//     if(_engine) delete _engine;
//     delete _graphDocument;

}

// ThreadDocument::setDocumentName(const QString& name){
//     _name = name;
// }

void ThreadDocument::terminate(){
//   m_docManager->deleteLater();
  Rocs::DSPluginManager::instance()->deleteLater();
  Rocs::PluginManager::instance()->deleteLater();
//   QThread::terminate();
  exit();
}

bool ThreadDocument::isRunning() const{
    if (!engine()) return false;
    return engine()->isRunning();
}

DataTypeDocument* ThreadDocument::document() const
{
    return _dataTypeDocument;
}


QtScriptBackend *ThreadDocument::engine() const{

    return  _dataTypeDocument->engineBackend();
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
//   connect (Rocs::DSPluginManager::instance(), SIGNAL(changingDS(QString)), _graphDocument, SLOT(convertToDS(QString)));
//   _engine = new QtScriptBackend();
//   _docCondition.wakeAll();
//   kDebug() << "Waking All";
// }

// void ThreadDocument::loadDocument(const QString& name){
// //     createEmptyDocument();
//
// }

// void ThreadDocument::setGraphDocument(GraphDocument * doc){
// //   releaseDocument();
//   m_docManager->addDocument(doc);
// //   _graphDocument = doc;
//
// //   _engine = new QtScriptBackend();
//   _docCondition.wakeAll();
// }


// // DocumentManager* ThreadDocument::documentManager()
// // {
// //   return m_docManager;
// // }

void ThreadDocument::loadDocument(const QString& name){
  DataTypeDocument * doc;
  if ( name.isEmpty() ){
      doc = new DataTypeDocument( i18n("UntitledDoc") );
      doc->addDataType ( i18n ( "Untitled0" ) );
//      _dataTypeDocument = doc;
  }else{
      doc = new DataTypeDocument( i18n ( "Untitled0" ) );
      doc->loadFromInternalFormat ( name );
//       _dataTypeDocument = doc;
/*
      if (m_actualDocument){
          emit deactivateDocument(m_actualDocument);
      }
      m_actualDocument = doc;*/


   }
   changeDocument(doc);

}

void ThreadDocument::releaseDocument(){
  if (_dataTypeDocument){
      emit deactivateDocument(_dataTypeDocument);
//       _dataTypeDocument->deleteLater();
  }
  _dataTypeDocument = 0;
}



void ThreadDocument::changeDocument(DataTypeDocument* arg1){
     releaseDocument();
      _dataTypeDocument = arg1;
      emit activateDocument(_dataTypeDocument);
}


void ThreadDocument::convertToDataStructure(/*QString dsName*/){
//     kDebug() << "-=-=-=-=-= Converting Data Structures -=-=-=-=-=";
//   QString old = Rocs::DSPluginManager::instance()->actualPlugin();
//   Rocs::DSPluginManager::instance()->changeActiveDS(dsName);

  DataTypeDocument * newDoc = new DataTypeDocument(*_dataTypeDocument);

//   kDebug() << "Releasing Document";
//   emit deactivateDocument(_dataTypeDocument);

/*
  kDebug() << "Converting Document";
  if (_dataTypeDocument)
      _dataTypeDocument->convertToDS(dsName);*/


  kDebug() << "Activing it!";
  changeDocument(newDoc);
//   Rocs::DSPluginManager::instance()->changeActiveDS(old);

}

void ThreadDocument::init(){
kDebug() << "############ Load Plugins ###############";
   Rocs::PluginManager::instance()->loadPlugins();
  kDebug() << "############ Load DS Plugins ############";
  Rocs::DSPluginManager::instance(this->thread());
  kDebug() <<"End of loading plugins.";
  if (Rocs::DSPluginManager::instance()->listOfDS().count() == 0){
    return;
  }

//   m_docManager = new DocumentManager(_docCondition ,_mutex);

  connect (Rocs::DSPluginManager::instance(), SIGNAL(changingDS(QString)), this, SLOT(convertToDataStructure()));

  emit initializeComplete();
}

void ThreadDocument::run(){

  connect(this, SIGNAL(initialize()), this, SLOT(init()),Qt::QueuedConnection);
  emit initialize();
  exec();
}
