/*
 *  SPDX-FileCopyrightText: 2013-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef CONSOLEMODULE_H
#define CONSOLEMODULE_H

#include "graphtheory_export.h"
#include "kernel/kernel.h"
#include <QList>
#include <QObject>
#include <QPair>

namespace GraphTheory
{
/**
 * \class ConsoleModule
 * This class provides an interface object for the scripting engine to write information to
 * the application console. The application widget that displays these information shall listen
 * to this object.
 */
class GRAPHTHEORY_EXPORT ConsoleModule : public QObject
{
    Q_OBJECT

public:
    explicit ConsoleModule(QObject *parent = nullptr);
    ~ConsoleModule() override;

    /**
     * Clear the backlog.
     */
    void clear();

    /**
     * Get all backlog since last clear.
     * \return the backlog
     */
    QList<QPair<GraphTheory::Kernel::MessageType, QString>> backlog() const;

public Q_SLOTS:
    /**
     * Print a log message \p message.
     */
    Q_INVOKABLE void log(const QString &message);

    /**
     * Print a debug message \p message.
     */
    Q_INVOKABLE void debug(const QString &message);

    /**
     * Print an error message \p message.
     */
    Q_INVOKABLE void error(const QString &message);

Q_SIGNALS:
    void message(const QString &message, GraphTheory::Kernel::MessageType type);

private:
    Q_DISABLE_COPY(ConsoleModule)
    QList<QPair<GraphTheory::Kernel::MessageType, QString>> m_backlog;
};
}

#endif
