#include "threadScriptExecution.h"
#include "graphDocument.h"
#include "qtScriptBackend.h"
#include "qscriptenginedebugger.h"
#include <QMutex>
#include <KTextBrowser>
// #include <QScriptEngineDebugger> //give-me a error ¬¬, realy need it?
#include <KDebug>

ThreadScriptExecution * self;

ThreadScriptExecution::ThreadScriptExecution():
	 QThread(),
         _script(),
         _graphDocument(0)
{
        _engine = 0;
	self = this;
}



ThreadScriptExecution::~ThreadScriptExecution(){
        delete _engine;
}

static QScriptValue debug_script(QScriptContext* context, QScriptEngine* /*engine*/) {
    self->debug(QString("%1 \n").arg(context->argument(0).toString()));
    return QScriptValue();
}

static QScriptValue output_script(QScriptContext *context, QScriptEngine* /*engine*/){
    self->output(QString("%1 \n").arg(context->argument(0).toString()));
    return QScriptValue();
}

void ThreadScriptExecution::debug(const QString& str){
	emit debugString(str);
}

void ThreadScriptExecution::output(const QString& str){
	emit outputString(str);
}

void ThreadScriptExecution::setData(QString script, GraphDocument * graphDocument){
    _script = script;
    _graphDocument = graphDocument;
    if (_engine) {
          delete _engine;
    }

    _engine = new QtScriptBackend( (*_graphDocument) );
//    QScriptEngineDebugger *dbg = new QScriptEngineDebugger(this);
//    dbg->attachTo(_engine);
    _engine->globalObject().setProperty("debug", _engine->newFunction(debug_script));
    _engine->globalObject().setProperty("output", _engine->newFunction(output_script));

}

void ThreadScriptExecution::run(){
     if (_mutex.tryLock()){
       kDebug() << _script;
	if(!_engine){
	  return;
	}
        QScriptValue results = _engine->evaluate(_script);
        emit debugString(results.toString());
        _mutex.unlock();
     }
}

void ThreadScriptExecution::abort(){
    if (!_mutex.tryLock()){
      _engine->abortEvaluation();
    }
    _mutex.unlock();
}

