// /*
//     <one line to give the program's name and a brief idea of what it does.>
//     Copyright (C) <year>  <name of author>
//
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License
//     along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// */
//
// #ifndef DOCUMENTMANAGER_H
// #define DOCUMENTMANAGER_H
//
// #include <QObject>
//
// class QMutex;
// class QWaitCondition;
// class Pointer;
// class Graph;
// class Datum;
// class QGridLayout;
// class DataTypeDocument;
// #include "rocslib_export.h"
//
// class ROCSLIB_EXPORT DocumentManager : public QObject
// {
//   Q_OBJECT
//     QWaitCondition &_docCondition;
//     QMutex &_mutex;
//     QList<DataTypeDocument*> m_documents;
//     DataTypeDocument * m_actualDocument;
//
//
//   public:
//     DocumentManager(QWaitCondition &docCondition, QMutex &mutex,QObject* parent = 0);
//     virtual ~DocumentManager();
//
//     QList <DataTypeDocument * > documentList();
//     /**Return document at index i */
//     DataTypeDocument * document(const int& i);
//     /**Return actual document.*/
//     DataTypeDocument * activeDocument();
//
//   public slots:
//     void changeDocument(DataTypeDocument*);
//
//     void changeDocument();
//     /** Add a document to list and set as active document */
//     void addDocument(DataTypeDocument*);
//     /** Remove the document from list. if the document is the active one, then active document pass to be the last from list. If it is the last one, an empty is created and set as active.*/
//     void removeDocument(DataTypeDocument*);
//     /** Convert document to new data structure. */
//     void convertToDataStructure(QString ds);
//
//     void loadDocument(QString fileName = QString());
//
//
//
//   signals:
//     /** signal emited when a new document is made active (ex. when changeDocument() or addDocument() was called)*/
//     void activateDocument (DataTypeDocument*);
//     /** this signal is emited when actual active document is deactivate (by a removal or a change)*/
//     void deactivateDocument(DataTypeDocument* doc);
//     /** signal emited when a document was removed from list. if doc is the active document, both, deactivateDocument() and activeDocument() is called first */
//     void documentRemoved(DataTypeDocument* doc);
// };
//
// #endif // DOCUMENTMANAGER_H
