/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "editor.h"
#include "edge.h"
#include "fileformats/fileformatmanager.h"
#include "logging_p.h"
#include "node.h"
#include "typenames.h"
#include <QFileInfo>
#include <QUrl>

using namespace GraphTheory;

class GraphTheory::EditorPrivate
{
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

#include "moc_editor.cpp"
