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

#include "DocumentManager.h"
#include "Document.h"
#include "DataStructureBackendManager.h"
#include <KDebug>
#include <QWaitCondition>
#include <QAction>
#include "QtScriptBackend.h"
#include <KLocale>
#include "LoadSave/GraphFileBackendManager.h"
#include "DataStructurePluginInterface.h"

// load catalog for library
static const KCatalogLoader loader("rocscore");

DocumentManager * DocumentManager::_self = 0;

DocumentManager * DocumentManager::self()
{
    if (!_self) {
        _self = new DocumentManager();
        connect(DataStructureBackendManager::self(), SIGNAL(backendChanged(QString)),
                _self, SLOT(convertToDataStructure()));
    }
    return _self;
}

DocumentManager::DocumentManager(QObject *parent)
    : QObject(parent)
{
    _activeDocument = 0;
}

DocumentManager::~DocumentManager()
{
    foreach(Document *g, _documents) {
        removeDocument(g);
    }
}

void DocumentManager::addDocument(Document *document)
{
    if (!_documents.contains(document)) {
        if (document->dataStructures().count() == 0) {
            document->addDataStructure();
        }
        _documents.append(document);
        changeDocument(document);
    }
    emit documentListChanged();
}


void DocumentManager::changeDocument(int index)
{
//FIXME It crash in some systems, in other no. Let's try reactive it in future
//     Q_ASSERT(index >= 0 && index < documentList().length());
    if (index < 0 || index >= documentList().length()) {
        return;
    }
    changeDocument(documentList().at(index));
}


void DocumentManager::changeDocument()
{
    QAction *action = qobject_cast<QAction *> (sender());

    if (! action) {
        return;
    }
    if (Document *doc = _documents.value(action->data().toInt())) {
        changeDocument(doc);
    }
}


void DocumentManager::changeDocument(Document *document)
{
    if (!_documents.contains(document)) {
        _documents.append(document);
    }
    if (_activeDocument != document) {
        if (_activeDocument) {
            emit deactivateDocument(_activeDocument);
            DataStructureBackendManager::self()->disconnect(_activeDocument);
            document->disconnect(SIGNAL(activeDataStructureChanged(DataStructurePtr)));
            document->engineBackend()->disconnect(SIGNAL(sendDebug(QString)));
            document->engineBackend()->disconnect(SIGNAL(sendOutput(QString)));
            document->engineBackend()->disconnect(SIGNAL(finished()));
        }
        _activeDocument = document;
        if (_activeDocument) {
            emit activateDocument();
        }
    }
}


void DocumentManager::closeAllDocuments()
{
    foreach(Document *document, documentList()) {
        removeDocument(document);
    }
}


void DocumentManager::removeDocument(Document *document)
{
    if (_documents.removeOne(document)) {
        document->engineBackend()->stop();
        document->disconnect();

        if (_activeDocument == document) {
            if (_documents.count() > 0) {
                changeDocument(_documents.last()); //
            } else {
                emit deactivateDocument(_activeDocument);
                _activeDocument = 0;
            }
        }
        emit documentRemoved(document);
        emit documentListChanged();

        document->deleteLater();
    }
}


void DocumentManager::convertToDataStructure()
{
    if (!_activeDocument) {
        kWarning() << "No active document found, creating new document with active backend.";
        newDocument();
        return;
    }

    //Check if need to convert (different DS) and if is possible to convert without data lost.
    if (_activeDocument->dataStructureTypeName() != DataStructureBackendManager::self()->activeBackend()->internalName()
            && DataStructureBackendManager::self()->activeBackend()->canConvertFrom(_activeDocument))
    {
        _activeDocument->changeBackend();
        kDebug() << "Data structure converted to " << DataStructureBackendManager::self()->activeBackend()->name();
        emit activateDocument();
    }
}


Document * DocumentManager::newDocument()
{
    Document *doc;
    QString name;

    // find unused name
    QList<QString> usedNames;
    foreach(Document * document, _documents) {
        usedNames.append(document->name());
    }
    // For at least one i in this range, the name is not used, yet.
    for (int i = 0; i < _documents.length() + 1; ++i) {
        name = QString("%1 %2").arg(i18nc("document that contains graphs or data structures as a visual representation", "Document")).arg(i);
        if (!usedNames.contains(name)) {
            break;
        }
    }
    doc = new Document(name);
    doc->addDataStructure();
    doc->setModified(false);
    addDocument(doc);

    if (_activeDocument==0) {
        _activeDocument = doc;
        emit activateDocument();
    }
    return doc;
}


Document * DocumentManager::openDocument(const KUrl &documentUrl)
{
    GraphFilePluginInterface* loader = GraphFileBackendManager::self()->defaultBackend();
    loader->setFile(documentUrl);
    loader->readFile();
    if (loader->error() != GraphFilePluginInterface::None) {
        kDebug() << "Could not load file. Graph loader returned error: " << loader->errorString();
        return new Document(documentUrl.fileName());;
    }
    Document* doc = loader->graphDocument();
    doc->setName(documentUrl.fileName());
    doc->setModified(false);
    addDocument(doc);
    emit activateDocument();
    return doc;
}


void DocumentManager::saveDocumentAs(Document *document, const KUrl &documentUrl)
{
    exportDocument(document, documentUrl);
    document->setFileUrl(documentUrl);
    document->setModified(false);
    return;
}

void DocumentManager::exportDocument(Document *document, const KUrl &documentUrl)
{
    GraphFilePluginInterface* serializer = GraphFileBackendManager::self()->defaultBackend();
    serializer->setFile(documentUrl);
    serializer->writeFile(*document);
    if (serializer->error() != GraphFilePluginInterface::None) {
        kDebug() << "Could not save file. Serializer returned error: " << serializer->errorString();
        return;
    }
    return;
}
