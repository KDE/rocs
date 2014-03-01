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
#include "DataStructure.h"
#include "DataStructureBackendManager.h"
#include "QtScriptBackend.h"
#include "LoadSave/GraphFileBackendManager.h"
#include "DataStructureBackendInterface.h"

#include <KUrl>
#include <QDebug>
#include <KLocale>
#include <QWaitCondition>
#include <QAction>
#include <QSvgRenderer>


// load catalog for library
// static const KCatalogLoader loader("rocscore"); //FIXME deactivate translation while porting

class DocumentManagerPrivate
{
public:
    bool _initialized;
    QList<Document*> _documents;
    Document *_activeDocument;

    // The list of shared renders provide resource efficient use of icon renders for same icon
    // packages. A possible future problem is that this list is never cleaned.
    QMap<QString, QSvgRenderer *> _sharedRenderers;

    DocumentManagerPrivate()
        : _initialized(false)
    {
    }
};

DocumentManager & DocumentManager::self()
{
    static DocumentManager instance;
    if(!instance.d->_initialized) {
        instance.d->_initialized = true;
        connect(&DataStructureBackendManager::self(), SIGNAL(backendChanged(QString)),
                &instance, SLOT(convertToDataStructure()));
    }

    return instance;
}

DocumentManager::DocumentManager(QObject *parent)
    : QObject(parent)
    , d(new DocumentManagerPrivate())
{
    d->_activeDocument = 0;
}

DocumentManager::~DocumentManager()
{
    foreach(Document *g, d->_documents) {
        removeDocument(g);
    }

    // remove shared renderers
    QMap <QString, QSvgRenderer* >::iterator iter = d->_sharedRenderers.begin();
    while (iter != d->_sharedRenderers.end()) {
        iter.value()->deleteLater();
        ++iter;
    }
    d->_sharedRenderers.clear();
}

Document * DocumentManager::document(const int index) const
{
    return (index < d->_documents.count() && index >= 0) ? d->_documents.at(index) : 0;
}

Document * DocumentManager::activeDocument() const
{
    return d->_activeDocument;
}

QList< Document* > DocumentManager::documentList() const
{
    return d->_documents;
}

void DocumentManager::addDocument(Document *document)
{
    if (!d->_documents.contains(document)) {
        if (document->dataStructures().count() == 0) {
            document->addDataStructure();
        }
        d->_documents.append(document);
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
    if (Document *doc = d->_documents.value(action->data().toInt())) {
        changeDocument(doc);
    }
}


void DocumentManager::changeDocument(Document *document)
{
    if (!d->_documents.contains(document)) {
        d->_documents.append(document);
    }
    if (d->_activeDocument != document) {
        if (d->_activeDocument) {
            emit deactivateDocument(d->_activeDocument);
            DataStructureBackendManager::self().disconnect(d->_activeDocument);
            document->disconnect(SIGNAL(activeDataStructureChanged(DataStructurePtr)));
            document->engineBackend()->disconnect(SIGNAL(sendDebug(QString)));
            document->engineBackend()->disconnect(SIGNAL(sendOutput(QString)));
            document->engineBackend()->disconnect(SIGNAL(finished()));
        }
        d->_activeDocument = document;
        if (d->_activeDocument) {
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
    if (d->_documents.removeOne(document)) {
        document->engineBackend()->stop();
        document->disconnect();

        if (d->_activeDocument == document) {
            if (d->_documents.count() > 0) {
                changeDocument(d->_documents.last()); //
            } else {
                emit deactivateDocument(d->_activeDocument);
                d->_activeDocument = 0;
            }
        }
        emit documentRemoved(document);
        emit documentListChanged();

        document->clear();
        document->deleteLater();
    }
}


void DocumentManager::convertToDataStructure()
{
    if (!d->_activeDocument) {
        qWarning() << "No active document found, creating new document with active backend.";
        newDocument();
        return;
    }

    //Check if need to convert (different DS) and if is possible to convert without data lost.
    if (d->_activeDocument->backend()->internalName() != DataStructureBackendManager::self().activeBackend()->internalName()
            && DataStructureBackendManager::self().activeBackend()->canConvertFrom(d->_activeDocument))
    {
        d->_activeDocument->changeBackend();
        qDebug() << "Data structure converted to " << DataStructureBackendManager::self().activeBackend()->name();
        emit activateDocument();
    }
}


Document * DocumentManager::newDocument()
{
    Document *doc;
    QString name;

    // find unused name
    QList<QString> usedNames;
    foreach(Document * document, d->_documents) {
        usedNames.append(document->name());
    }
    // For at least one i in this range, the name is not used, yet.
    for (int i = 0; i < d->_documents.length() + 1; ++i) {
        name = QString("%1 %2").arg(i18nc("document that contains graphs or data structures as a visual representation", "Document")).arg(i);
        if (!usedNames.contains(name)) {
            break;
        }
    }
    doc = new Document(name);
    doc->addDataStructure();
    doc->setModified(false);
    addDocument(doc);

    if (d->_activeDocument==0) {
        d->_activeDocument = doc;
        emit activateDocument();
    }
    return doc;
}


Document * DocumentManager::openDocument(const QUrl &documentUrl)
{
    GraphFilePluginInterface* loader = GraphFileBackendManager::self()->defaultBackend();
    loader->setFile(documentUrl);
    loader->readFile();
    if (loader->error() != GraphFilePluginInterface::None) {
        qDebug() << "Could not load file. Graph loader returned error: " << loader->errorString();
        return new Document(documentUrl.fileName());;
    }
    Document* doc = loader->graphDocument();
    doc->setName(documentUrl.fileName());
    doc->setModified(false);
    addDocument(doc);
    emit activateDocument();
    return doc;
}


void DocumentManager::saveDocumentAs(Document *document, const QUrl &documentUrl)
{
    exportDocument(document, documentUrl);
    document->setFileUrl(documentUrl);
    document->setModified(false);
    return;
}

void DocumentManager::exportDocument(Document *document, const QUrl &documentUrl)
{
    GraphFilePluginInterface* serializer = GraphFileBackendManager::self()->defaultBackend();
    serializer->setFile(documentUrl);
    serializer->writeFile(*document);
    if (serializer->error() != GraphFilePluginInterface::None) {
        qDebug() << "Could not save file. Serializer returned error: " << serializer->errorString();
        return;
    }
    return;
}


QSvgRenderer * DocumentManager::sharedRenderer(const QString &iconPackage)
{
    if (d->_sharedRenderers.count(iconPackage) == 0 || !d->_sharedRenderers.contains(iconPackage)) {
        registerSharedRenderer(iconPackage);
    }
    return d->_sharedRenderers.value(iconPackage);
}


QSvgRenderer * DocumentManager::registerSharedRenderer(const QString &iconPackage)
{
    if (!d->_sharedRenderers.contains(iconPackage)) {
        QSvgRenderer *z = new QSvgRenderer(iconPackage);
        d->_sharedRenderers.insert(iconPackage, z);
    }
    return d->_sharedRenderers.value(iconPackage);
}


void DocumentManager::removeSharedRenderer(const QString &iconPackage)
{
    d->_sharedRenderers[iconPackage]->deleteLater();
    d->_sharedRenderers.remove(iconPackage);
}

