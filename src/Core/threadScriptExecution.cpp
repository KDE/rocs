#include "threadScriptExecution.h"
#include "graphDocument.h"
#include "qtScriptBackend.h"
#include "qscriptenginedebugger.h"
#include <QMutex>
#include <KTextBrowser>

ThreadScriptExecution * self;

ThreadScriptExecution::ThreadScriptExecution(KTextBrowser * debugView):
	 QThread(),
         _script(),
         _graphDocument(0),
         _txtDebug(debugView)
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

void ThreadScriptExecution::debug(const QString& str){
	_txtDebug->insertPlainText(str);
}

void ThreadScriptExecution::setData(QString script, GraphDocument * graphDocument){
    _script = script;
    _graphDocument = graphDocument;
    if (_engine) {
          delete _engine;
    }

    _engine = new QtScriptBackend( (*_graphDocument) ,  _txtDebug);
    //QScriptEngineDebugger *dbg = new QScriptEngineDebugger(this);
    //dbg->attachTo(_engine);
    _engine->globalObject().setProperty("debug", _engine->newFunction(debug_script));

}

void ThreadScriptExecution::run(){
     if (_mutex.tryLock()){
        QScriptValue results = _engine->evaluate(_script);
        _txtDebug->insertPlainText(results.toString());

        _mutex.unlock();
     }

}

void ThreadScriptExecution::abort(){
    if (!_mutex.tryLock()){
        _engine->abortEvaluation();
    //    this->terminate();
    }
    _mutex.unlock();
}

