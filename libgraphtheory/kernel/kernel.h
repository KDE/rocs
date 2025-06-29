// SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#ifndef KERNEL_H
#define KERNEL_H

#include "graphtheory_export.h"
#include "typenames.h"
#include "engine.h"
#include <QObject>
#include <QJSEngine>
#include <QAction>

namespace GraphTheory
{

/**
 * \class Kernel
 */
class GRAPHTHEORY_EXPORT Kernel : public QObject
{
    Q_OBJECT

public:
    Kernel();

    /**
     * execute javascript @p script on @p document and @return result as reported by engine
     */
    QJSValue execute(GraphTheory::GraphDocumentPtr document, const QString &script);
    void stop();

    void triggerInterruptAction();

private Q_SLOTS:
    void handleEngineStateChanged(Engine::State state);

Q_SIGNALS:
    void message(const QString &message, GraphTheory::MessageType type);
    void executionFinished();

private:
    Engine m_engine;
};
}

#endif
