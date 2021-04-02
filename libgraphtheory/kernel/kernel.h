/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KERNEL_H
#define KERNEL_H

#include "graphtheory_export.h"
#include "typenames.h"
#include "node.h"
#include "graphdocument.h"

#include <QScriptEngine>
#include <QObject>

#include <QAction>

namespace GraphTheory
{
class KernelPrivate;

/**
 * \class Kernel
 */
class GRAPHTHEORY_EXPORT Kernel : public QObject
{
    Q_OBJECT

public:
    enum MessageType {
        InfoMessage,
        WarningMessage,
        ErrorMessage
    };

    Kernel();

    virtual ~Kernel();

    /**
     * execute javascript @p script on @p document and @return result as reported by engine
     */
    QScriptValue execute(GraphTheory::GraphDocumentPtr document, const QString &script);
    void stop();

    void attachDebugger();
    void detachDebugger();
    void triggerInterruptAction();

private Q_SLOTS:
    /** process all incoming messages and resend them afterwards**/
    void processMessage(const QString &message, GraphTheory::Kernel::MessageType type);

Q_SIGNALS:
    void message(const QString &message, GraphTheory::Kernel::MessageType type);
    void executionFinished();

private:
    const QScopedPointer<KernelPrivate> d;
};
}

#endif
