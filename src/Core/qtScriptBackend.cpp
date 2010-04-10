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
      if (!_engine) return;
      if (_engine->isEvaluating()){
        _engine->abortEvaluation();
      }
      _engine->deleteLater();
}

void QtScriptBackend::start()
{
    if (_engine){
        _engine->disconnect();
        delete _engine;
    }
    
    _engine = new QScriptEngine();
    QScriptEngineDebugger *dbg = new QScriptEngineDebugger(this);
    dbg->attachTo( _engine );
    _engine->globalObject().setProperty("debug",  engine()->newFunction(debug_script));
    _engine->globalObject().setProperty("output", engine()->newFunction(output_script));
    kDebug() << "ScriptBackend Created";
    
    int size = _graphs->size();
    for (int i = 0; i < size; i++) {
        _graphs->at(i)->setEngine(_engine);
    }
    createGraphList();
    _engine->evaluate(_script);
    
    delete dbg;
}

QtScriptBackend::QtScriptBackend(){
    self = this;
    _engine = 0;
}

void QtScriptBackend::setScript(const QString& s,GraphDocument *graphs ) {
    _script = s;
    _graphs = graphs;
    kDebug() << "script Set";
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

void QtScriptBackend::setProperty( QScriptValue& object, const QScriptString& name, uint , const QScriptValue& value ) {
    QObject *obj = object.toQObject();
    QString s(name.toString());
    QVariant v(value.toVariant());
    if (obj == 0) {
        kDebug () << "Object is zero";
        return;
    }

    obj->setProperty(s.toAscii(), v);
}

QScriptValue QtScriptBackend::property ( const QScriptValue & object, const QScriptString & name, uint ) {
    QObject *obj = object.toQObject();
    QString s(name.toString());
    if (obj == 0) {
        kDebug () << "Object is zero";
        return QScriptValue();
    }

    QScriptValue  value = object.property(s);
    return value;
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

