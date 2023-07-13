/*
 *  SPDX-FileCopyrightText: 2013-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "consolemodule.h"

using namespace GraphTheory;

ConsoleModule::ConsoleModule(QObject *parent)
    : QObject(parent)
{
}

ConsoleModule::~ConsoleModule()
{
}

QList<QPair<GraphTheory::Kernel::MessageType, QString>> ConsoleModule::backlog() const
{
    return m_backlog;
}

void ConsoleModule::clear()
{
    m_backlog.clear();
}

void ConsoleModule::log(const QString &messageString)
{
    m_backlog.append(qMakePair<Kernel::MessageType, QString>(Kernel::InfoMessage, messageString));
    Q_EMIT message(messageString, Kernel::InfoMessage);
}

void ConsoleModule::debug(const QString &messageString)
{
    m_backlog.append(qMakePair<Kernel::MessageType, QString>(Kernel::WarningMessage, messageString));
    Q_EMIT message(messageString, Kernel::WarningMessage);
}

void ConsoleModule::error(const QString &messageString)
{
    m_backlog.append(qMakePair<Kernel::MessageType, QString>(Kernel::ErrorMessage, messageString));
    Q_EMIT message(messageString, Kernel::ErrorMessage);
}

#include "moc_consolemodule.cpp"
