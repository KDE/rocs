/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "editor.h"
#include "typenames.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include "fileformats/fileformatmanager.h"
#include "logging_p.h"
#include <QUrl>
#include <QFileInfo>

using namespace GraphTheory;

class GraphTheory::EditorPrivate {
public:
    EditorPrivate()
    {
    }

    ~EditorPrivate()
    {

    }

    QList<GraphDocumentPtr> m_documents;
    FileFormatManager m_fileFormatManager;
};

Editor::Editor()
    : QObject()
    , d(new EditorPrivate)
{

}

Editor::~Editor()
{

}

QList<GraphDocumentPtr> Editor::documents() const
{
    return d->m_documents;
}

GraphDocumentPtr Editor::createDocument()
{
    GraphDocumentPtr document = GraphDocument::create();
    d->m_documents.append(document);
    return document;
}

GraphDocumentPtr Editor::openDocument(const QUrl &documentUrl)
{
    // get file extension/format
    QFileInfo fi(documentUrl.toLocalFile());
    QString ext = fi.completeSuffix();

    GraphTheory::FileFormatInterface *importer = d->m_fileFormatManager.backendByExtension(ext);
    if (!importer) {
        qCCritical(GRAPHTHEORY_GENERAL) << "No graph file backend found for extension" << ext << ", aborting.";
        return GraphDocumentPtr();
    }
    importer->setFile(documentUrl);
    importer->readFile();
    if (importer->hasError()) {
        qCCritical(GRAPHTHEORY_GENERAL) << "Graph file importer reported the following error, aborting.";
        importer->errorString();
        return GraphDocumentPtr();
    }
    importer->graphDocument()->setDocumentUrl(documentUrl);
    return importer->graphDocument();
}
