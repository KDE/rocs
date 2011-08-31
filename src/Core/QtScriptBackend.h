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

#include "abstractrunbackend.h"

#include <QtScript>
#include <QScriptValue>
#include <QScriptString>
#include "rocslib_export.h"

class Document;


class ToolsPluginInterface;


class  ROCSLIB_EXPORT QtScriptBackend : public AbstractRunBackend{
  Q_OBJECT
public:
//     QtScriptBackend();
    QtScriptBackend(QObject* parent = 0);
    QScriptEngine *engine(){ return _engine; }

    /** return true if is evaluating a script or running a tool script. */
    bool isRunning();
    virtual void start();
    
private:
    
    void createGraphList();
    QScriptEngine *_engine;
    bool _runningTool;

signals:
    void engineCreated(QScriptEngine* e);

  public slots:


    /** run a tool plugin in graph and later run it resulting script.*/
    void runTool(ToolsPluginInterface * plugin, Document *document);


    /** abort script evaluation. In case of a too is running, stop will not stop tool. But the script resulting from tool will not be runned.

    */
    void stop();
};

#endif
