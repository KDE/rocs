/***************************************************************************
 *
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

#include "krossBackend.h"
#include "graph.h"
#include "node.h"
#include <KDebug>
#include <KTextBrowser>
#include "graphDocument.h"
#include <QScriptEngineDebugger>
#include <unistd.h> // usleep

static QtScriptBackend *self;

static QScriptValue debug_script(QScriptContext* context, QScriptEngine* /*engine*/) {
    self->debug(QString("%1 \n").arg(context->argument(0).toString()));
    return QScriptValue();
}

static QScriptValue output_script(QScriptContext *context, QScriptEngine* /*engine*/){
    self->output(QString("%1 \n").arg(context->argument(0).toString()));
    return QScriptValue();
}

void QtScriptBackend::stop(){
      kDebug() << "Stop requested.";
      if (!_engine) return;
      
      if (_engine->isEvaluating()){
        _engine->abortEvaluation();
      }
      _engine->deleteLater();
      _engine = 0;
      emit finished();
}

void QtScriptBackend::start()
{
    stop();
    
    _engine = new QScriptEngine();
    emit engineCreated(_engine);
    usleep(500);
    
    _engine->globalObject().setProperty("debug",  engine()->newFunction(debug_script));
    _engine->globalObject().setProperty("output", engine()->newFunction(output_script));
    kDebug() << "ScriptBackend Created";
    
    int size = _graphs->size();
    for (int i = 0; i < size; i++) {
	kDebug() << "Setting graph" << i << "as global object";
        _graphs->at(i)->setEngine(_engine);
    }
    kDebug() << "Setting the graph list";
    createGraphList();
   
    kDebug() << "Evaluating the script";
    QString error = _engine->evaluate(_script).toString();
    while( _engine && _engine->isEvaluating() ){
	usleep(200000); /// rest a bit.
    }
   kDebug() << "Script evaluated to " << error;
   
    emit finished(); kDebug() << "Finished emmited";
    emit sendDebug(error); kDebug() << "send debug emmited";
}

bool QtScriptBackend::isRunning(){
  if ((_engine) && (_engine->isEvaluating())){
        return true;
  }
  return _runningTool;
}
    

QtScriptBackend::QtScriptBackend(){
    self = this;
    _engine = 0;
    _runningTool = false;
}

void QtScriptBackend::runTool(Rocs::ToolsPluginInterface * plugin, GraphDocument *graphs){
    _runningTool = true;
    _graphs = graphs;
    _script = plugin->run(graphs);
    if ( !_script.isEmpty()){
      start();
    }
    _runningTool = false;
}

void QtScriptBackend::setScript(const QString& s,GraphDocument *graphs ) {
    _script = s;
    _graphs = graphs;
    kDebug() << "script Set" << _script;
}

void QtScriptBackend::createGraphList() {
    QScriptValue graphList = _engine->newArray();
    _engine->globalObject().setProperty("graphs", graphList);

    // Add all the graphs on the array as an array, and if it has a name,
    // also add it for direct acess with it's name.
    int size = _graphs->size();
    for (int i = 0; i < size; i++) {
        graphList.property("push").call(graphList, QScriptValueList() << _graphs->at(i)->scriptValue());
    }
}

void QtScriptBackend::loadFile(const QString& file) {
    _script.clear();
    QFile f(file);
    if  (  !f.open(QIODevice::ReadOnly | QIODevice::Text ) ) {
        kDebug() << "File not found";
        return;
    }

    while ( ! f.atEnd() ) {
        QByteArray line = f.readLine();
        _script += line;
    }
    _script += '\n';
}

void QtScriptBackend::debug(const QString& str){
     emit sendDebug(str);
}

void QtScriptBackend::output(const QString& str){
    emit sendOutput(str);
}

