/*
    This file is part of Rocs.
    Copyright 2013       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ConsoleModule.h"

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
