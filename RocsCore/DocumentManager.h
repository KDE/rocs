/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

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

class KUrl;
class Data;
class Document;
#include "RocsCoreExport.h"

class ROCSLIB_EXPORT DocumentManager : public QObject
{
    Q_OBJECT

public:
    static DocumentManager* self();

    virtual ~DocumentManager();

    /**
     * Returns document with index \p i in document list. Note that index can change due to
     * various operations. Signal \see documentListChanged() is emitted if this happens.
     * \param index of document in document list
     * \return the document
     */
    Document* document(const int i) const;

    /**
     * Returns the currently active document, or 0 if there document list is empty.
     * \return currently active document
     */
    Document* activeDocument() const;

    /**
     * Returns the document list of the document manager.
     * Position of document in this list is temporary document index.
     * List order can change by \see removeDocument().
     * \return list with documents
     */
    QList< Document* > documentList() const ;

    int viewStyleDataNode();
    int viewStyleDataEdge();

public slots:
    /** Convert document to new data structure. */
    void convertToDataStructure();

    /**
     * Change active document to be \p document.
     * \param document is the document to be set active.
     */
    void changeDocument(Document* document);

    /**
     * Change to document with given \p index.
     * \param index is expected to be a valid index
     */
    void changeDocument(int index);

    void changeDocument();

    /**
     * Close all documents from document list.
     * This method does not write data to document files.
     */
    void closeAllDocuments();

    /**
     * Add document to document list and set this document as active document.
     * \param document that shall be added to document list
     */
    void addDocument(Document* document);

    /**
     * Remove document from document list. If the document is currently active,
     * then the last document from document list becomes active. If document
     * list contains only this document, a new (empty) document is created.
     * This method does not write data to document files.
     * \param document that shall be removed
     */
    void removeDocument(Document* document);

    /**
     * Creates and loads a new graph document.
     * \return created document
     */
    Document* newDocument();

    /**
     * Loads graph document specified by \p documentUrl and adds document to document list.
     * Sets loaded document as active document.
     * \param documentUrl is Url specifying the to be opened document
     * \return loaded document
     */
    Document* openDocument(const KUrl& documentUrl);

    /**
     * Save graph document \p document at url \p documentUrl. The internal file url for the
     * document is reset with \p documentUrl and the modification state is set to false.
     * To save document without changing its properties \see exportDocument(...).
     * \param document is the graph document to be save
     * \param documentUrl is the target file for saving
     */
    void saveDocumentAs(Document* document, const KUrl& documentUrl);

    /**
     * Save graph document \p document at url \p documentUrl. The document is not changed by this
     * operation.
     * \param document is the graph document to be save
     * \param documentUrl is the target file for saving
     */
    void exportDocument(Document* document, const KUrl& documentUrl);

signals:
    /**
     * Signal is emitted if the currently active document changes
     */
    void activateDocument();

    /**
     * Signal is emitted if the currently active document is deactivated.
     * (This can be caused by a removal or a change.)
     */
    void deactivateDocument(Document* document);

    /**
     * Signal is emitted if \p document was removed from list.
     * If \p document is the active document, both, deactivateDocument() and activeDocument()
     * were called before this signal.
     */
    void documentRemoved(Document* document);

    /**
     * Signal is emitted if the document list changes and older document index values get invalid.
     */
    void documentListChanged();

private:
    DocumentManager(QObject* parent = 0);
    static DocumentManager *_self;
    QList<Document*> _documents;
    Document *_activeDocument;
};

inline Document* DocumentManager::document(const int i) const
{
    return (i < _documents.count() && i >= 0) ? _documents.at(i) : 0;
}

inline Document* DocumentManager::activeDocument() const
{
    return _activeDocument;
}

inline QList< Document* > DocumentManager::documentList() const
{
    return _documents;
}
#endif // DOCUMENTMANAGER_H
