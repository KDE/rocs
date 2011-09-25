/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2011       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
#include <PluginManager.h>
#include "DataStructurePluginInterface.h"

DocumentManager *DocumentManager::_self = 0;

DocumentManager* DocumentManager::self(){
    if (!_self){
        _self = new DocumentManager();
        connect (DataStructurePluginManager::self(), SIGNAL(changingDataStructure(QString)),
                _self, SLOT(convertToDataStructure()));
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
        doc->disconnect(SIGNAL(activeDataStructureChanged(DataStructurePtr)));
        doc->engineBackend()->disconnect(SIGNAL(sendDebug(QString)));
        doc->engineBackend()->disconnect(SIGNAL(sendOutput(QString)));
        doc->engineBackend()->disconnect(SIGNAL(finished()));
      }
      m_actualDocument = doc;
      if (m_actualDocument){
          emit activateDocument();
      }
    }
}

void DocumentManager::removeDocument(Document* doc){
    if (m_documents.removeOne(doc)){
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

void DocumentManager::convertToDataStructure() {
    Document * newDoc = 0;
    if (m_actualDocument){
        if (m_actualDocument->dataStructureTypeName() != DataStructurePluginManager::self()->actualPluginName()
                && DataStructurePluginManager::self()->actualPlugin()->canConvertFrom(m_actualDocument)) {
            //Verificar se é possível converter
            m_actualDocument->cleanUpBeforeConvert();
            newDoc = new Document(*m_actualDocument);
            emit deactivateDocument(m_actualDocument);
            addDocument(newDoc);
        }
    }else {
        loadDocument();
    }
}

void DocumentManager::loadDocument ( QString name ){
    Document * doc;
    if ( name.isEmpty() ){
        int docNumber = 0;
        forever{
            bool found = false;
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
    doc->setModified(false);
    addDocument(doc);
}

