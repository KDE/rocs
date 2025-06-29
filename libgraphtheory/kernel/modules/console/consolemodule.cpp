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

ConsoleModule::~ConsoleModule() = default;

QList<QPair<GraphTheory::MessageType, QString>> ConsoleModule::backlog() const
{
    return m_backlog;
}

void ConsoleModule::clear()
{
    m_backlog.clear();
}

void ConsoleModule::log(const QString &messageString)
{
    m_backlog.append(qMakePair<MessageType, QString>(MessageType::InfoMessage, messageString));
    Q_EMIT message(messageString, MessageType::InfoMessage);
}

void ConsoleModule::debug(const QString &messageString)
{
    m_backlog.append(qMakePair<MessageType, QString>(MessageType::WarningMessage, messageString));
    Q_EMIT message(messageString, MessageType::WarningMessage);
}

void ConsoleModule::error(const QString &messageString)
{
    m_backlog.append(qMakePair<MessageType, QString>(MessageType::ErrorMessage, messageString));
    Q_EMIT message(messageString, MessageType::ErrorMessage);
}

#include "moc_consolemodule.cpp"
