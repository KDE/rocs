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

#include "kernel.h"
#include "graphdocument.h"
#include "nodetype.h"
#include "edge.h"

#include <KLocalizedString>
#include <QPointF>
#include <QColor>
#include <QDebug>

using namespace GraphTheory;

class GraphTheory::KernelPrivate {
public:
    KernelPrivate()
    {
    }

    ~KernelPrivate()
    {
    }

    QScriptValue registerGlobalObject(QObject *qobject, const QString &name);

    QString m_script;
    QScriptEngine *m_engine;
};

QScriptValue KernelPrivate::registerGlobalObject(QObject *qobject, const QString &name)
{
    if (!m_engine) {
        qCritical() << "No engine set, aborting global object creation.";
        return 0;
    }
    QScriptValue globalObject = m_engine->newQObject(qobject);
    m_engine->globalObject().setProperty(name, globalObject);

    return globalObject;
}

///BEGIN: Kernel
Kernel::Kernel()
    : d(new KernelPrivate)
{

}

Kernel::~Kernel()
{

}

void Kernel::processMessage(const QString &message, MessageType type)
{
    //TODO do something with the messages
}

void Kernel::execute()
{
    if (!d->m_engine) {
        d->m_engine = new QScriptEngine(this);
    }

    if (d->m_engine->isEvaluating()) {
        d->m_engine->abortEvaluation();
    }
    d->m_engine->collectGarbage();
    d->m_engine->pushContext();

    // add elements


    //TODO setup plugins

    d->m_engine->setProcessEventsInterval(100); //! TODO: Make that changeable.

    QString result = d->m_engine->evaluate(d->m_script).toString();
    if (d->m_engine && d->m_engine->hasUncaughtException()) {
        processMessage(result, WarningMessage);
        processMessage(d->m_engine->uncaughtExceptionBacktrace().join("\n"), InfoMessage);
    }
    if (d->m_engine) {
        processMessage(i18nc("@info status message after successful script execution", "<i>Execution Finished</i>"), InfoMessage);
        processMessage(result, InfoMessage);
        d->m_engine->popContext();
    }
    emit executionFinished();
}

//END: Kernel
