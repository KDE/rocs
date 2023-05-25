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
#include <QDebug>

using namespace GraphTheory;

class GraphTheory::KernelPrivate
{
public:
    KernelPrivate()
        : m_engine(new QJSEngine)
    {
    }

    QJSValue registerGlobalObject(QObject *qobject, const QString &name);

    QJSEngine *m_engine;
    ConsoleModule m_consoleModule;
};

QJSValue KernelPrivate::registerGlobalObject(QObject *qobject, const QString &name)
{
    if (!m_engine) {
        qCCritical(GRAPHTHEORY_KERNEL) << "No engine set, aborting global object creation.";
        return 0;
    }
    QJSValue globalObject = m_engine->newQObject(qobject);
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

QJSValue Kernel::execute(GraphDocumentPtr document, const QString &script)
{
    // register meta types
    qRegisterMetaType<QList<GraphTheory::NodeWrapper *>>("QList<GraphTheory::NodeWrapper *>");
    qRegisterMetaType<QList<GraphTheory::EdgeWrapper *>>("QList<GraphTheory::EdgeWrapper *>");
    qRegisterMetaType<GraphTheory::NodeWrapper *>("GraphTheory::NodeWrapper *");
    qRegisterMetaType<GraphTheory::EdgeWrapper *>("GraphTheory::EdgeWrapper *");

    if (d->m_engine->isInterrupted()) {
        d->m_engine->setInterrupted(false);
    }

    d->m_engine->collectGarbage();

    // add document
    DocumentWrapper documentWrapper(document, d->m_engine);
    d->m_engine->globalObject().setProperty("Document", d->m_engine->newQObject(&documentWrapper));
    connect(&documentWrapper, &DocumentWrapper::message, this, &Kernel::processMessage);

    // set modules
    d->m_engine->globalObject().setProperty("Console", d->m_engine->newQObject(&d->m_consoleModule));

    QJSValue result = d->m_engine->evaluate(script).toString();
    if (result.isError()) {
        Q_EMIT message(result.toString(), WarningMessage);
    }
    else {
        Q_EMIT message(i18nc("@info status message after successful script execution", "<i>Execution Finished</i>"), InfoMessage);
        Q_EMIT message(result.toString(), InfoMessage);
    }
    // end processing messages
    disconnect(&documentWrapper, &DocumentWrapper::message, this, &Kernel::processMessage);

    Q_EMIT executionFinished();
    d->m_engine->globalObject().setProperty("Document", QJSValue());

    return result;
}

void Kernel::stop()
{
    d->m_engine->setInterrupted(true);
}

void Kernel::processMessage(const QString &messageString, Kernel::MessageType type)
{
    Q_EMIT message(messageString, type);
}

void Kernel::attachDebugger()
{
    QJSValue result = d->m_engine->evaluate("GraphTheory");
    if(result.isError())
        qDebug()
            << "Uncaught exception at line"
            << result.property("lineNumber").toInt()
            << ":" << result.toString();;
}

void Kernel::detachDebugger()
{
    qDebug() << "detach\n";
}

void Kernel::triggerInterruptAction()
{
    qDebug()
        << "Dunno what's goin'on here";
}

// END: Kernel
