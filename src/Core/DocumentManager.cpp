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
#include "graphDocument.h"
#include "DSPluginManager.h"
#include <KDebug>

DocumentManager::DocumentManager(QObject* parent): QObject(parent){
  m_actualDocument = 0;
  GraphDocument * doc = new GraphDocument(i18n("Untitled"), 800, 600);
  addDocument(doc);
}

DocumentManager::~DocumentManager(){
//   qDeleteAll(m_documents.begin(), m_documents.end());
}


void DocumentManager::addDocument(GraphDocument* newDoc){
    if (!m_documents.contains(newDoc)){
       if (newDoc->count() == 0){
          newDoc->addGraph();
       }
       m_documents.append(newDoc);
       changeDocument(newDoc);
    }
}

void DocumentManager::changeDocument(GraphDocument* doc){
    if(!m_documents.contains(doc)){
      m_documents.append(doc);
    }
    if (m_actualDocument != doc){
      if (m_actualDocument){
        Rocs::DSPluginManager::New()->disconnect(m_actualDocument);
        doc->disconnect(SIGNAL(activeGraphChanged(Graph*)));
        doc->engineBackend()->disconnect(SIGNAL(sendDebug(QString)));
        doc->engineBackend()->disconnect(SIGNAL(sendOutput(QString)));
        doc->engineBackend()->disconnect(SIGNAL(finished()));
//         m_actualDocument->deleteLater();
      }
      m_actualDocument = doc;
//       connect (Rocs::DSPluginManager::New(), SIGNAL(changingDS(QString)), m_actualDocument, SLOT(convertToDS(QString)));
      emit documentChanged(doc);
    }
}

GraphDocument* DocumentManager::document(const int& i)
{
  if (i < m_documents.count() && i >= 0){
      return (m_documents.at(i));
  }
  return 0;
}

GraphDocument* DocumentManager::activeDocument(){
  return m_actualDocument;
}



QList< GraphDocument* > DocumentManager::documentList()
{
  return m_documents;
}


void DocumentManager::removeDocument(GraphDocument* doc){
    if (m_documents.removeOne(doc) == 1){
      if (m_actualDocument == doc){
         if (m_documents.count() > 0){
            changeDocument(m_documents.last());
         }else{
            addDocument(new GraphDocument(i18n("Untitled"), 800, 600));
         }
      }
      emit documentRemoved(doc);
      doc->deleteLater();
    }/*else {
      kDebug() << "document not found on list."
    }*/
}

void DocumentManager::convertToDataStructure(QString ds){
  if (m_actualDocument)
  /*GraphDocument * doc = */m_actualDocument->convertToDS(ds);
  emit documentChanged(m_actualDocument);
//   if (doc != 0){
//         addDocument(doc);
//   }


}
