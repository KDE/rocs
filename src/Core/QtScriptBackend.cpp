/* 
    This file is part of Rocs.
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "QtScriptBackend.h"
#include "DataStructure.h"
#include "Data.h"
#include <KDebug>
#include <KTextBrowser>
#include "Document.h"
#include <QAction>
#include <QScriptEngineDebugger>
#include <unistd.h> // usleep
#include <ToolsPluginInterface.h>

static QtScriptBackend *self;

static QScriptValue debug_script(QScriptContext* context, QScriptEngine* /*engine*/) {
    self->debug(QString("%1").arg(context->argument(0).toString()));
    return QScriptValue();
}

static QScriptValue output_script(QScriptContext *context, QScriptEngine* /*engine*/) {
    self->output(QString("%1").arg(context->argument(0).toString()));
    return QScriptValue();
}

static QScriptValue interrupt_script(QScriptContext *context, QScriptEngine* /*engine*/) {
    self->interrupt();
    return QScriptValue();
}

void QtScriptBackend::stop() {
    if (!_engine) return;

    // abort possibly running execution
    if ( _engine->isEvaluating() ){
        _engine->abortEvaluation();
    }
    // check for stepped execution
    if( _engineSteps ) {
        disconnect(this, SIGNAL(interrupted()), 0, 0);
        _engineSteps->action(QScriptEngineDebugger::ContinueAction)->trigger();
        _engineSteps->detach();
        _engineSteps->deleteLater();
        _engineSteps = 0;
    }

    _engine->deleteLater();
    _engine = 0;
    emit finished();
}

void QtScriptBackend::execute() {
    stop();

    _engine = new QScriptEngine();
    emit engineCreated(_engine);

    _engine->globalObject().setProperty("debug",  engine()->newFunction(debug_script));
    _engine->globalObject().setProperty("output", engine()->newFunction(output_script));
    _engine->globalObject().setProperty("interrupt", engine()->newFunction(interrupt_script));

    int size = _document->dataStructures().size();
    for (int i = 0; i < size; i++) {
        _document->dataStructures().at(i)->setEngine(_engine);
    }
    createGraphList();
    _engine->setProcessEventsInterval(100); //! TODO: Make that changable.
    
    QString error = _engine->evaluate(_script).toString();
    if (_engine && _engine->hasUncaughtException()) {
        emit scriptError();
        emit sendDebug("<b style=\"color: red\">"+error+"</b>");
    }
    emit finished();
}

void QtScriptBackend::executeStep() {
    // prepare new engine if execution not running yet
    if ( !_engine ) {
        _engine = new QScriptEngine( this );
        emit engineCreated( _engine );
    }

    // create step-executor if not present
    if( !_engineSteps ) {
        _engineSteps = new QScriptEngineDebugger( this );
         _engineSteps->setAutoShowStandardWindow(false); // we do not want to have a debugger window
        _engineSteps->attachTo( _engine );
    }

    if( !_engine->isEvaluating() ) {
        _engine->globalObject().setProperty("debug",  engine()->newFunction(debug_script));
        _engine->globalObject().setProperty("output", engine()->newFunction(output_script));
        _engine->globalObject().setProperty("interrupt", engine()->newFunction(interrupt_script));

        int size = _document->dataStructures().size();
        for (int i = 0; i < size; i++) {
            _document->dataStructures().at(i)->setEngine(_engine);
        }
        createGraphList();
        _engine->setProcessEventsInterval(100);
    
        QString error = _engine->evaluate(_script).toString();
        if (_engine && _engine->hasUncaughtException()) {
            emit scriptError();
            emit sendDebug("<b style=\"color: red\">"+error+"</b>");
        }
    }

    if( !_engine || !_engine->isEvaluating() ) {
        emit finished();
    }
}

bool QtScriptBackend::isRunning(){
    if ((_engine) && (_engine->isEvaluating())){
        return true;
    }
    return _runningTool;
}

QtScriptBackend::QtScriptBackend(QObject* parent): QObject(parent){
    self = this;
    _engine = 0;
    _engineSteps = 0;
    _runningTool = false;
}


void QtScriptBackend::runTool(ToolsPluginInterface * plugin, Document *graphs){
    _runningTool = true;
    _document = graphs;
    _script = plugin->run(graphs);
    if ( !_script.isEmpty()){
        execute();
    }
    _runningTool = false;
}

void QtScriptBackend::setScript(const QString& s,Document *graphs ) {
    _script = s;
    _document = graphs;
    kDebug() << "script Set" << _script;
}

void QtScriptBackend::createGraphList() {
    QScriptValue graphList = _engine->newArray();
    _engine->globalObject().setProperty("graphs", graphList);

    // Add all the graphs on the array as an array, and if it has a name,
    // also add it for direct acess with it's name.
    int size = _document->dataStructures().size();
    for (int i = 0; i < size; i++) {
        graphList.property("push").call(graphList, QScriptValueList() << _document->dataStructures().at(i)->scriptValue());
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
    emit sendDebug("<b>"+str+"</b>");
}

void QtScriptBackend::continueExecutionStep() {
    if( _engine && _engineSteps && _engine->isEvaluating() ) {
        _engineSteps->action( QScriptEngineDebugger::ContinueAction )->trigger();
    }
}

void QtScriptBackend::interrupt(){
    if (!_engine->isEvaluating())
        return;

    //FIXME workaround for now: two signals needed to trigger interrupt
    if (_engineSteps)
        _engineSteps->action(QScriptEngineDebugger::InterruptAction)->trigger();
    if (_engineSteps)
        _engineSteps->action(QScriptEngineDebugger::InterruptAction)->trigger();
}
