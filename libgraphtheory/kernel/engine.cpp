// SPDX-FileCopyrightText: 2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "engine.h"
#include <QDebug>

using namespace GraphTheory;

Engine::Engine()
    : m_documentIdentifier("Document")
    , m_consoleIdentifier("Console")
{
    connect(&m_consoleModule, &ConsoleModule::message, this, &Engine::processMessage);
}

Engine::State Engine::state() const
{
    return m_state;
}

QJSValue Engine::execute(GraphDocumentPtr document, const QString &script)
{
    m_engine.collectGarbage();
    setState(State::Running);

    // setup engine
    m_documentWrapper = std::make_unique<DocumentWrapper>(document, &m_engine);
    setGlobalProperty(m_documentIdentifier, m_documentWrapper.get());
    setGlobalProperty(m_consoleIdentifier, &m_consoleModule);
    connect(m_documentWrapper.get(), &DocumentWrapper::message, this, &Engine::processMessage);

    QJSValue result = m_engine.evaluate(script).toString();
    if (result.isError()) {
        Q_EMIT message(result.toString(), MessageType::WarningMessage);
    } else {
        Q_EMIT message("Execution Finished", MessageType::InfoMessage); // TODO this used to be translated!
        Q_EMIT message(result.toString(), MessageType::InfoMessage);
    }

    m_documentWrapper->releaseScriptValue();

    m_engine.globalObject().deleteProperty(m_documentIdentifier);
    disconnect(m_documentWrapper.get(), &DocumentWrapper::message, this, &Engine::processMessage);
    m_documentWrapper.reset();

    return result;
}

void Engine::stop()
{
    // TODO this is the previous unchanged behavior, but stop() is supposed to do more than interrupt
    m_engine.setInterrupted(true);
}

QJSValue Engine::setGlobalProperty(const QString &name, QObject *qobject)
{
    QJSValue globalObject = m_engine.newQObject(qobject);
    m_engine.globalObject().setProperty(name, m_engine.newQObject(qobject));
    return globalObject;
}

void Engine::setState(State state)
{
    if (m_state == state) {
        return;
    }
    m_state = State::Running;
    Q_EMIT stateChanged(m_state);
}

void Engine::processMessage(const QString &messageString, GraphTheory::MessageType type)
{
    Q_EMIT message(messageString, type);
}
