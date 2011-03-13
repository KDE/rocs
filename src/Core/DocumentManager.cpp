/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "DocumentManager.h"
#include "Document.h"
#include "DataStructurePluginManager.h"
#include <KDebug>
#include <QWaitCondition>
#include <QAction>
#include "QtScriptBackend.h"
#include <KLocale>

DocumentManager *DocumentManager::_self = 0;

DocumentManager* DocumentManager::self(){
    if (!_self){
        _self = new DocumentManager();
    }
    return _self;
}

DocumentManager::DocumentManager( QObject* parent):QObject(parent){
    m_actualDocument = 0;
}

DocumentManager::~DocumentManager(){
    foreach (Document * g, m_documents){
        removeDocument(g);
    }
}

void DocumentManager::addDocument(Document* newDoc){
    if (!m_documents.contains(newDoc)){
        if (newDoc->dataStructures().count() == 0){
            newDoc->addDataStructure();
        }
        m_documents.append(newDoc);
        changeDocument(newDoc);
    }
}

void DocumentManager::changeDocument(){
    QAction *action = qobject_cast<QAction *> ( sender() );

    if (! action ){
      return;
    }
    if (Document *doc = m_documents.value(action->data().toInt()) ){
      changeDocument(doc);
    }
}

void DocumentManager::changeDocument(Document* doc){
    if(!m_documents.contains(doc)){
      m_documents.append(doc);
    }
    if (m_actualDocument != doc){
      if (m_actualDocument){
         emit deactivateDocument(m_actualDocument);
        DataStructurePluginManager::self()->disconnect(m_actualDocument);
        doc->disconnect(SIGNAL(activeDataStructureChanged(DataStructure*)));
        doc->engineBackend()->disconnect(SIGNAL(sendDebug(QString)));
        doc->engineBackend()->disconnect(SIGNAL(sendOutput(QString)));
        doc->engineBackend()->disconnect(SIGNAL(finished()));
      }
      m_actualDocument = doc;
      if (m_actualDocument){
          emit activateDocument();
      }
      connect (DataStructurePluginManager::self(), SIGNAL(changingDataStructure(QString)),
               this, SLOT(convertToDataStructure()));
    }
}

void DocumentManager::removeDocument(Document* doc){
    if (m_documents.removeOne(doc) != 0){
        if (m_actualDocument == doc){
            if (m_documents.count() > 0){
                changeDocument(m_documents.last()); //
            }else{
                emit deactivateDocument(m_actualDocument);
                m_actualDocument = 0;
            }
        }
        emit documentRemoved(doc);
        doc->deleteLater();
    }
}

void DocumentManager::convertToDataStructure(){
  kDebug() << "-=-=-=-=-= Converting Data Structures -=-=-=-=-=";
  Document * newDoc = 0;
  if (m_actualDocument){
    newDoc = new Document(*m_actualDocument);
    emit deactivateDocument(m_actualDocument);
    addDocument(newDoc);
  }
  else{
    loadDocument();
  }
}

void DocumentManager::loadDocument ( QString name ){
  Document * doc;
  if ( name.isEmpty() ){
      bool found = false;
      int docNumber = 0;
      forever{
          name = QString("%1%2").arg(i18n("Untitled")).arg(docNumber);
          foreach( Document *data, m_documents){
              if( data->name() == name){
                  found = true;
                  docNumber += 1;
              }
          }
          if ( ! found ){
              break;
          }
      }
      doc = new Document( name );
      doc->addDataStructure ( i18n ( "Untitled0" ) );
  }else{
      doc = new Document( i18n ( "Untitled0" ) );
      doc->loadFromInternalFormat ( name );
   }
   addDocument(doc);
}
