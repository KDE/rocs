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

#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QObject>

class Data;
class Document;
#include "rocslib_export.h"

class ROCSLIB_EXPORT DocumentManager : public QObject
{
  Q_OBJECT
    QList<Document*> m_documents;
    Document * m_actualDocument;

   
  public:
    static DocumentManager* self();
    
    virtual ~DocumentManager();

    //!inlines.
    /** Gets the document on the index i, returns null if notfound. */
    Document* document(const int i) const; 
    
    /** returns the active document, or null if there's none */
    Document* activeDocument() const; 
    
    /** returns the document list */
    QList< Document* > documentList() const ;

  public slots:
    void changeDocument(Document*);

    void changeDocument();
    /** Add a document to list and set as active document */
    void addDocument(Document*);
    
    /** Remove the document from list. if the document is the active one,
     * then active document pass to be the last from list.
     * If it is the last one, an empty is created and set as active.
     */
    void removeDocument(Document*);
    
    /** Convert document to new data structure. */
    void convertToDataStructure();

    void loadDocument(QString fileName = QString());


  signals:
    /** signal emited when a new document is made active (ex. when changeDocument() or addDocument() was called)*/
    void activateDocument ();
    /** this signal is emited when actual active document is deactivate (by a removal or a change)*/
    void deactivateDocument(Document* doc);
    /** signal emited when a document was removed from list. if doc is the active document, both, deactivateDocument() and activeDocument() is called first */
    void documentRemoved(Document* doc);
    
private:
    DocumentManager(QObject* parent = 0);
    static DocumentManager *_self;
};

inline Document* DocumentManager::document(const int i) const { return (i < m_documents.count() && i >= 0) ? m_documents.at(i) : 0; }
inline Document* DocumentManager::activeDocument() const {   return m_actualDocument; }
inline QList< Document* > DocumentManager::documentList() const {  return m_documents; }
#endif // DOCUMENTMANAGER_H
