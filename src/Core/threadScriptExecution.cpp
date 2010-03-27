#include "threadScriptExecution.h"
#include "graphDocument.h"
#include "qtScriptBackend.h"
#include "qscriptenginedebugger.h"
#include <QMutex>
#include <KTextBrowser>
// #include <QScriptEngineDebugger> //give-me a error ¬¬, realy need it?
#include <KDebug>
#include <KLocale>
static ThreadScriptExecution *self;

ThreadScriptExecution::ThreadScriptExecution(QMutex &mutex):
	 QThread(),
	 _mutex(mutex),
     _script(),
     _graphDocument(0),
     _engine(0)
{
	self = this;
}

void ThreadScriptExecution::setActiveGraphDocument(GraphDocument *d){
    kDebug() << "Entering on active document.";
    if (_graphDocument == 0 && d == 0){
        _graphDocument = new GraphDocument(i18n("Untitled"), 800, 600);
        emit graphDocumentCreated( _graphDocument );
        
        connect(_graphDocument, SIGNAL(graphCreated(Graph*)),   this, SIGNAL(graphCreated(Graph*)));
        connect(_graphDocument, SIGNAL(graphRemoved(int)),      this, SIGNAL(graphRemoved(int)));
        connect(_graphDocument, SIGNAL(heightChanged(qreal)),   this, SIGNAL(documentHeigthChanged(qreal)));
        connect(_graphDocument, SIGNAL(widthChanged(qreal)),    this, SIGNAL(documentHeightChanged(qreal)));
        
        _graphDocument->addGraph(i18n("Untitled0"));
     } else {
        _graphDocument = d;
     }
}

ThreadScriptExecution::~ThreadScriptExecution(){
    if(_engine) delete _engine;
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

void ThreadScriptExecution::setScript(const QString& script){
    _script = script;
    if (_engine) {
          delete _engine;
    }

    _engine = new QtScriptBackend( (*_graphDocument) );
    QScriptEngineDebugger *dbg = new QScriptEngineDebugger(this);
    dbg->attachTo(_engine);
    _engine->globalObject().setProperty("debug", _engine->newFunction(debug_script));
    _engine->globalObject().setProperty("output", _engine->newFunction(output_script));

}

void ThreadScriptExecution::run(){
  exec();
}

// script - related.
void ThreadScriptExecution::startScript(){
    
}

bool ThreadScriptExecution::documentCreated()
{
  return _graphDocument ? true : false;
  
}

    /*
// graph - related.
void addGraph(const QString& s){

}

void removeGraph(Graph *g){

}
    
void addNode(const QString& s, Graph *g = 0){

}

void removeNode(Node *n){

}


void addEdge(Node *n1, Node *n2);

void removeEdge(Edge *e);*/

void ThreadScriptExecution::abortScript(){
    if (!_mutex.tryLock()){
      _engine->abortEvaluation();
    }
    _mutex.unlock();
}

