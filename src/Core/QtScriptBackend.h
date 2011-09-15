/* 
    This file is part of Rocs.
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either 
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public 
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef QTSCRIPTBACKEND_H
#define QTSCRIPTBACKEND_H

#include <QtScript>
#include <QScriptValue>
#include <QScriptString>
#include "rocslib_export.h"

class Document;


class ToolsPluginInterface;


class  ROCSLIB_EXPORT QtScriptBackend : public QObject{
  Q_OBJECT
public:
//     QtScriptBackend();
    QtScriptBackend(QObject* parent = 0);
    void setScript(const QString& s, Document *document);
    void setDocument(Document *document);
    void loadFile(const QString& file);
    void debug(const QString& s);
    void output(const QString& s);
    QScriptEngine *engine(){ return _engine; }

    /** return true if is evaluating a script or running a tool script. */
    bool isRunning();

private:
    void createGraphList();
    
    QString _script;
    Document *_document;
    QScriptEngine *_engine;

    bool _runningTool;

signals:
    void sendOutput(const QString& s);
    void sendDebug(const QString& s);
    void scriptError();
    void engineCreated(QScriptEngine* e);
    void finished();

  public slots:
    void start();

    /** run a tool plugin in graph and later run it resulting script.*/
    void runTool(ToolsPluginInterface * plugin, Document *document);


    /** abort script evaluation. In case of a too is running, stop will not stop tool. But the script resulting from tool will not be runned.

    */
    void stop();
};

#endif
