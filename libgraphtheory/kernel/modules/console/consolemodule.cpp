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
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
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

QList< QPair< ConsoleModule::MessageType, QString > > ConsoleModule::backlog() const
{
    return _backlog;
}

void ConsoleModule::clear()
{
    _backlog.clear();
}

void ConsoleModule::log(const QString& message)
{
    _backlog.append(qMakePair<MessageType, QString>(ConsoleModule::Log, message));
    emit(backlogChanged(ConsoleModule::Log, message));
}

void ConsoleModule::debug(const QString& message)
{
    _backlog.append(qMakePair<MessageType, QString>(ConsoleModule::Debug, message));
    emit(backlogChanged(ConsoleModule::Debug, message));
}

void ConsoleModule::error(const QString& message)
{
    _backlog.append(qMakePair<MessageType, QString>(Error, message));
    emit(backlogChanged(ConsoleModule::Error, message));
}
