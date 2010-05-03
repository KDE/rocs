/***************************************************************************
 * main.cpp
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
#ifndef QTSCRIPTBACKEND_H
#define QTSCRIPTBACKEND_H

#include <QtScript>
#include <QScriptValue>
#include <QScriptString>
#include "rocslib_export.h"

class Graph;
class GraphDocument;
class KTextBrowser;

namespace Rocs{
  class ToolsPluginInterface;
}

class  ROCSLIB_EXPORT QtScriptBackend : public QObject{
  Q_OBJECT
public:
    QtScriptBackend();
    void setScript(const QString& s, GraphDocument *graph);
    void setDocument(GraphDocument *document);
    void loadFile(const QString& file);
    void debug(const QString& s);
    void output(const QString& s);
    QScriptEngine *engine(){ return _engine; }

    /** return true if is evaluating a script or running a tool script. */
    bool isRunning();
    
private:
    QString _script;
    GraphDocument *_graphs;
    void createGraphList();
    QScriptEngine *_engine;

    bool _runningTool;
    
signals:
    void sendOutput(const QString& s);
    void sendDebug(const QString& s);
    void engineCreated(QScriptEngine* e);
    
  public slots:
    void start();

    /** run a tool plugin in graph and later run it resulting script.*/
    void runTool(Rocs::ToolsPluginInterface * plugin, GraphDocument *graphs);
        
    
    /** abort script evaluation. In case of a too is running, stop will not stop tool. But the script resulting from tool will not be runned.

    */
    void stop();
};

#endif
