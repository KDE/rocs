/***************************************************************************
 * This file is part of the KDE project
 * copyright (C)2004-2007 by Tomaz Canabrava (tomaz.canabrava@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/

#ifndef THREADSCRIPTEXECUTION_H
#define THREADSCRIPTEXECUTION_H

#include <QThread>
#include <QMutex>
#include "rocslib_export.h"

class Graph;
class Node;
class Edge;

class KTextBrowser;
class GraphDocument;
class QtScriptBackend;

class ROCSLIB_EXPORT ThreadScriptExecution : public QThread{
  Q_OBJECT
  public:
    ThreadScriptExecution();
    virtual ~ThreadScriptExecution();
    bool documentCreated();
    
  public slots:
    void run();
    
    // script - related.
    void abortScript();
    void startScript();
    void setScript(const QString& s);
    
    void setActiveGraphDocument(GraphDocument *g = 0);
    
    // ui related.
    void debug(const QString& s);
    void output(const QString& s);
    /*
    // graph - related.
    void addGraph(const QString& s);
    void removeGraph(Graph *g);
    
    void addNode(const QString& s, Graph *g = 0);
    void removeNode(Node *n);

    void addEdge(Node *n1, Node *n2);
    void removeEdge(Edge *e);
    */
  private:
    QString _script;
    QtScriptBackend* _engine;

    GraphDocument * _graphDocument;
    QMutex _mutex;
    
  signals:
    void debugString(const QString& s);
    void outputString(const QString& s);
    void graphDocumentChanged(GraphDocument *d);
    void graphCreated(Graph *g);
    void graphRemoved(int i);
    void documentHeightChanged(qreal q);
    void documentHeigthChanged(qreal q);
    void graphDocumentCreated(GraphDocument *g);
};

#endif
