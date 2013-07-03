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
#include "Document.h"
#include "DocumentManager.h"

#include <KLocale>
#include <KDebug>
#include <QAction>
#include <QScriptEngineDebugger>


class QtScriptBackendPrivate
{
public:
    QString _currentScript;
    Document *_document;
    QScriptEngine *_engine;
    QScriptEngineDebugger *_engineSteps;    // used for stepped execution
    IncludeManager _includeManager;
    bool _runningTool;

    QtScriptBackendPrivate()
        : _engine(new QScriptEngine())
        , _engineSteps(0)
        , _runningTool(false)
    {
    }

    ~QtScriptBackendPrivate()
    {
        delete _engine;
    }

    void createGraphList()
    {
        QScriptValue graphList = _engine->newArray();
        _engine->globalObject().setProperty("graphs", graphList);

        // Add all the graphs on the array as an array, and if it has a name,
        // also add it for direct access with it's name.
        int size = _document->dataStructures().size();
        for (int i = 0; i < size; i++) {
            graphList.property("push").call(graphList, QScriptValueList() << _document->dataStructures().at(i)->scriptValue());
        }
    }
};


static QScriptValue debug_script(QScriptContext* context, QScriptEngine* /*engine*/)
{
    DocumentManager::self().activeDocument()->engineBackend()->debug(QString("%1").arg(context->argument(0).toString()));
    return QScriptValue();
}

static QScriptValue output_script(QScriptContext *context, QScriptEngine* /*engine*/)
{
    DocumentManager::self().activeDocument()->engineBackend()->output(QString("%1").arg(context->argument(0).toString()));
    return QScriptValue();
}

static QScriptValue interrupt_script(QScriptContext *context, QScriptEngine* /*engine*/)
{
    Q_UNUSED(context);
    DocumentManager::self().activeDocument()->engineBackend()->interrupt();
    return QScriptValue();
}

static QScriptValue include_script(QScriptContext *context, QScriptEngine* /*engine*/)
{
    DocumentManager::self().activeDocument()->engineBackend()->include(QString("%1").arg(context->argument(0).toString()));
    return QScriptValue();
}

QtScriptBackend::QtScriptBackend(QObject* parent)
    : QObject(parent)
    , d(new QtScriptBackendPrivate)
{
}

QtScriptBackend::~QtScriptBackend()
{
}

QScriptEngine* QtScriptBackend::engine() const
{
    return d->_engine;
}

void QtScriptBackend::loadFile(const QString& file)
{
    d->_currentScript.clear();
    QFile f(file);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        kDebug() << "File not found";
        return;
    }

    while (! f.atEnd()) {
        QByteArray line = f.readLine();
        d->_currentScript += line;
    }
    d->_currentScript += '\n';
}

QScriptValue QtScriptBackend::registerGlobalObject(QObject *qobject, const QString &name)
{
    if (!d->_engine) {
        d->_engine = new QScriptEngine(this);
        emit engineCreated(d->_engine);
    }
    QScriptValue globalObject = d->_engine->newQObject(qobject);
    d->_engine->globalObject().setProperty(name, globalObject);

    return globalObject;
}

IncludeManager& QtScriptBackend::includeManager() const
{
    return d->_includeManager;
}

void QtScriptBackend::stop()
{
    // check for stepped execution
    if (d->_engineSteps) {
        d->_engineSteps->action(QScriptEngineDebugger::ContinueAction)->trigger();
        d->_engineSteps->detach();
        d->_engineSteps->deleteLater();
        d->_engineSteps = 0;
    }

    // abort possibly running execution
    if (d->_engine && d->_engine->isEvaluating()) {
        d->_engine->abortEvaluation();
    }

    if (d->_engine) {
        d->_engine->deleteLater();
        d->_engine = 0;
    }
    emit finished();
}

QString QtScriptBackend::execute()
{
    if (!d->_engine) {
        d->_engine = new QScriptEngine(this);
        emit engineCreated(d->_engine);
    }

    if (d->_engine->isEvaluating()) {
        d->_engine->abortEvaluation();
    }
    d->_engine->collectGarbage();
    d->_engine->pushContext();
    d->_engine->globalObject().setProperty("debug",  engine()->newFunction(debug_script));
    d->_engine->globalObject().setProperty("output", engine()->newFunction(output_script));
    d->_engine->globalObject().setProperty("interrupt", engine()->newFunction(interrupt_script));
    d->_engine->globalObject().setProperty("include", engine()->newFunction(include_script));

    int size = d->_document->dataStructures().size();
    for (int i = 0; i < size; i++) {
        d->_document->dataStructures().at(i)->setEngine(d->_engine);
        connect(d->_document->dataStructures().at(i).get(), SIGNAL(scriptError(QString)), this, SIGNAL(scriptError(QString)));
    }
    d->createGraphList();
    d->_engine->setProcessEventsInterval(100); //! TODO: Make that changeable.

    QString result = d->_engine->evaluate(d->_currentScript, i18n("Rocs Console script")).toString();
    if (d->_engine && d->_engine->hasUncaughtException()) {
        emit scriptError(i18n("Script Error: %1", result));
        emit scriptError(d->_engine->uncaughtExceptionBacktrace().join("\n"));
    }
    if (d->_engine) {
        emit scriptInfo(i18nc("@info status message after successful script execution", "<i>Execution Finished</i>"));
        d->_engine->popContext();
        for (int i = 0; i < size; i++) {
            d->_document->dataStructures().at(i).get()->disconnect(this);
        }
    }
    emit finished();
    return result;
}

void QtScriptBackend::executeStep()
{
    // prepare new engine if execution not running yet
    if (!d->_engine) {
        d->_engine = new QScriptEngine(this);
        emit engineCreated(d->_engine);
    }

    // create step-executor if not present
    if (!d->_engineSteps) {
        d->_engineSteps = new QScriptEngineDebugger(this);
        d->_engineSteps->setAutoShowStandardWindow(false); // we do not want to have a debugger window
        d->_engineSteps->attachTo(d->_engine);
    }

    if (!d->_engine->isEvaluating()) {
        d->_engine->globalObject().setProperty("debug",  engine()->newFunction(debug_script));
        d->_engine->globalObject().setProperty("output", engine()->newFunction(output_script));
        d->_engine->globalObject().setProperty("interrupt", engine()->newFunction(interrupt_script));

        int size = d->_document->dataStructures().size();
        for (int i = 0; i < size; i++) {
            d->_document->dataStructures().at(i)->setEngine(d->_engine);
            connect(d->_document->dataStructures().at(i).get(), SIGNAL(scriptError(QString)), this, SIGNAL(scriptError(QString)));
        }
        d->createGraphList();
        d->_engine->setProcessEventsInterval(100);

        QString error = d->_engine->evaluate(d->_currentScript).toString();
        if (d->_engine && d->_engine->hasUncaughtException()) {
            emit scriptError(i18n("Script Error: %1", error));
        }
    }

    if (!d->_engine || !d->_engine->isEvaluating()) {
        scriptInfo(i18nc("@info status message after successful script execution", "<i>Execution Finished</i>"));
        int size = d->_document->dataStructures().size();
        for (int i = 0; i < size; i++) {
            d->_document->dataStructures().at(i).get()->disconnect(this);
        }
        emit finished();
    }
}

bool QtScriptBackend::isRunning() const
{
    if ((d->_engine) && (d->_engine->isEvaluating())) {
        return true;
    }
    return d->_runningTool;
}

void QtScriptBackend::setScript(const QString& s, Document *graphs)
{
    d->_currentScript = s;
    d->_document = graphs;
}

void QtScriptBackend::debug(const QString& message)
{
    emit scriptError(i18n("The global method \"%1\" is deprecated, please use \"%2\" instead.", QString("debug(message)"), QString("Console.debug(message)")));
    emit sendDebug(message);
}

void QtScriptBackend::output(const QString& message)
{
    emit scriptError(i18n("The global method \"%1\" is deprecated, please use \"%2\" instead.", QString("output(message)"), QString("Console.log(message)")));
    emit sendOutput(message);
    emit sendDebug(message);
}

void QtScriptBackend::continueExecutionStep()
{
    if (d->_engine && d->_engineSteps && d->_engine->isEvaluating()) {
        d->_engineSteps->action(QScriptEngineDebugger::ContinueAction)->trigger();
    }
}

void QtScriptBackend::include(const QString& filePath)
{
    //TODO switch to URL usage
    QString fileName = d->_includeManager.seekFile(filePath);

    if (d->_includeManager.checkIfWasIncluded(fileName)) {
        return;
    }

    QFile f(fileName);
    if (f.open(QFile::ReadOnly)) {
        QString script = d->_includeManager.include(f.readAll(), fileName.section('/', 0, -2), fileName.section('/', -1));
        d->_engine->currentContext()->setActivationObject(d->_engine->currentContext()->parentContext()->activationObject());
        QString error = d->_engine->evaluate(script, filePath).toString();
        if (d->_engine && d->_engine->hasUncaughtException()) {
            emit scriptError(i18n("Script error in included file %1", filePath));
            emit scriptError(d->_engine->uncaughtExceptionBacktrace().join("\n"));
        }
    }
}

void QtScriptBackend::interrupt()
{
    if (!d->_engine->isEvaluating()) {
        return;
    }

    //FIXME workaround for now: two signals needed to trigger interrupt
    if (d->_engineSteps)
        d->_engineSteps->action(QScriptEngineDebugger::InterruptAction)->trigger();
    if (d->_engineSteps)
        d->_engineSteps->action(QScriptEngineDebugger::InterruptAction)->trigger();
}
