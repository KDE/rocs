/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "kernel.h"
#include "documentwrapper.h"
#include "edgewrapper.h"
#include "kernel/modules/console/consolemodule.h"
#include "logging_p.h"
#include "nodewrapper.h"

#include <KLocalizedString>
#include <QScriptEngineDebugger>

using namespace GraphTheory;

class GraphTheory::KernelPrivate
{
public:
    KernelPrivate()
        : m_engine(new QScriptEngine)
        , m_debugger(new QScriptEngineDebugger)
    {
    }

    ~KernelPrivate()
    {
        m_debugger->detach();
    }

    QScriptValue registerGlobalObject(QObject *qobject, const QString &name);

    QScriptEngine *m_engine;
    QScriptEngineDebugger *m_debugger;
    ConsoleModule m_consoleModule;
};

QScriptValue KernelPrivate::registerGlobalObject(QObject *qobject, const QString &name)
{
    if (!m_engine) {
        qCCritical(GRAPHTHEORY_KERNEL) << "No engine set, aborting global object creation.";
        return 0;
    }
    QScriptValue globalObject = m_engine->newQObject(qobject);
    m_engine->globalObject().setProperty(name, globalObject);

    return globalObject;
}

/// BEGIN: Kernel
Kernel::Kernel()
    : d(new KernelPrivate)
{
    connect(&d->m_consoleModule, &ConsoleModule::message, this, &Kernel::processMessage);
}

Kernel::~Kernel()
{
}

QScriptValue Kernel::execute(GraphDocumentPtr document, const QString &script)
{
    // register meta types
    qScriptRegisterSequenceMetaType<QList<GraphTheory::NodeWrapper *>>(d->m_engine);
    qScriptRegisterSequenceMetaType<QList<GraphTheory::EdgeWrapper *>>(d->m_engine);
    qRegisterMetaType<GraphTheory::NodeWrapper *>();
    qRegisterMetaType<GraphTheory::EdgeWrapper *>();

    if (d->m_engine->isEvaluating()) {
        d->m_engine->abortEvaluation();
    }
    d->m_engine->collectGarbage();
    d->m_engine->pushContext();

    // add document
    DocumentWrapper documentWrapper(document, d->m_engine);
    d->m_engine->globalObject().setProperty("Document", d->m_engine->newQObject(&documentWrapper));
    connect(&documentWrapper, &DocumentWrapper::message, this, &Kernel::processMessage);

    // set modules
    d->m_engine->globalObject().setProperty("Console", d->m_engine->newQObject(&d->m_consoleModule));

    // set evaluation
    d->m_engine->setProcessEventsInterval(100); //! TODO: Make that changeable.

    QScriptValue result = d->m_engine->evaluate(script).toString();
    if (d->m_engine && d->m_engine->hasUncaughtException()) {
        Q_EMIT message(result.toString(), WarningMessage);
        Q_EMIT message(d->m_engine->uncaughtExceptionBacktrace().join("\n"), InfoMessage);
    }
    if (d->m_engine) {
        Q_EMIT message(i18nc("@info status message after successful script execution", "<i>Execution Finished</i>"), InfoMessage);
        Q_EMIT message(result.toString(), InfoMessage);
        d->m_engine->popContext();
    }
    // end processing messages
    disconnect(&documentWrapper, &DocumentWrapper::message, this, &Kernel::processMessage);

    Q_EMIT executionFinished();
    d->m_engine->globalObject().setProperty("Document", QScriptValue());

    return result;
}

void Kernel::stop()
{
    d->m_engine->abortEvaluation();
}

void Kernel::processMessage(const QString &messageString, Kernel::MessageType type)
{
    Q_EMIT message(messageString, type);
}

void Kernel::attachDebugger()
{
    d->m_debugger->attachTo(d->m_engine);
}

void Kernel::detachDebugger()
{
    d->m_debugger->detach();
    d->m_debugger = new QScriptEngineDebugger;
}

void Kernel::triggerInterruptAction()
{
    d->m_debugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
}

// END: Kernel
