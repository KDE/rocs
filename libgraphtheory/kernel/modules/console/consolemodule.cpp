/*
 *  Copyright 2013-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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
 *  License along with this library.  If not, see <https://www.gnu.org/licenses/>.
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

QList< QPair< GraphTheory::Kernel::MessageType, QString > > ConsoleModule::backlog() const
{
    return m_backlog;
}

void ConsoleModule::clear()
{
    m_backlog.clear();
}

void ConsoleModule::log(const QString& messageString)
{
    m_backlog.append(qMakePair<Kernel::MessageType, QString>(Kernel::InfoMessage, messageString));
    emit(message(messageString, Kernel::InfoMessage));
}

void ConsoleModule::debug(const QString& messageString)
{
    m_backlog.append(qMakePair<Kernel::MessageType, QString>(Kernel::WarningMessage, messageString));
    emit(message(messageString, Kernel::WarningMessage));
}

void ConsoleModule::error(const QString& messageString)
{
    m_backlog.append(qMakePair<Kernel::MessageType, QString>(Kernel::ErrorMessage, messageString));
    emit(message(messageString, Kernel::ErrorMessage));
}
