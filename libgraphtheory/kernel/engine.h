// SPDX-FileCopyrightText: 2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#ifndef ENGINE_H
#define ENGINE_H

#include "documentwrapper.h"
#include "graphtheory_export.h"
#include "kernel/kerneltypes.h"
#include "kernel/modules/console/consolemodule.h"
#include "typenames.h"
#include <QJSEngine>
#include <QObject>
#include <QStringLiteral>
#include <memory>

namespace GraphTheory
{

/**
 * @brief Computing engine for scripts
 *
 * This class is a facade of the underlying computing engine (today, only the Qt JS engine,
 * and provides a reduced interaction API that ensures correct setup of engine objects, updates,
 * and blocking during execution.
 */
class GRAPHTHEORY_EXPORT Engine : public QObject
{
    Q_OBJECT
public:
    enum class State {
        NotStarted,
        Running,
        Interrupted,
        Error
    };
    Q_ENUM(State)

    Engine();
    State state() const;
    QJSValue execute(GraphDocumentPtr document, const QString &script);
    void stop();

private Q_SLOTS:
    void processMessage(const QString &message, GraphTheory::MessageType type);

Q_SIGNALS:
    void stateChanged(State state);
    void message(const QString &message, GraphTheory::MessageType type);

private:
    QJSValue setGlobalProperty(const QString &name, QObject *object);
    void setState(State state);

    QJSEngine m_engine;
    State m_state{State::NotStarted};

    ConsoleModule m_consoleModule;
    std::unique_ptr<DocumentWrapper> m_documentWrapper;

    const QString m_documentIdentifier; // TODO Qt6:: make constexpr
    const QString m_consoleIdentifier; // TODO Qt6:: make constexpr
};
};

#endif // ENGINE_H
