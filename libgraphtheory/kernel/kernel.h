/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
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
