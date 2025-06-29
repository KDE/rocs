// SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "kernel.h"
#include "documentwrapper.h"
#include "edgewrapper.h"
#include "nodewrapper.h"
#include <KLocalizedString>
#include <QDebug>

using namespace GraphTheory;

Kernel::Kernel()
{
    connect(&m_engine, &Engine::stateChanged, this, &Kernel::handleEngineStateChanged);
}

QJSValue Kernel::execute(GraphDocumentPtr document, const QString &script)
{
    // register meta types
    qRegisterMetaType<QList<GraphTheory::NodeWrapper *>>("QList<GraphTheory::NodeWrapper *>");
    qRegisterMetaType<QList<GraphTheory::EdgeWrapper *>>("QList<GraphTheory::EdgeWrapper *>");
    qRegisterMetaType<GraphTheory::NodeWrapper *>("GraphTheory::NodeWrapper *");
    qRegisterMetaType<GraphTheory::EdgeWrapper *>("GraphTheory::EdgeWrapper *");

    QJSValue result = m_engine.execute(document, script);
    Q_EMIT executionFinished();

    return result;
}

void Kernel::stop()
{
    m_engine.stop();
}

void Kernel::handleEngineStateChanged(Engine::State state)
{
    // TODO forward state
}

void Kernel::triggerInterruptAction()
{
    // FIXME pause engine due to interrupt
    qDebug() << "trigger handling not yet implemented";
}

#include "moc_kernel.cpp"
