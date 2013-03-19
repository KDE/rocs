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

#include "ConsoleInterface.h"

ConsoleInterface::ConsoleInterface(QObject* parent): QObject(parent)
{
}

ConsoleInterface::~ConsoleInterface()
{
}

QList< QPair< ConsoleInterface::MessageType, QString > > ConsoleInterface::backlog() const
{
    return _backlog;
}

void ConsoleInterface::clear()
{
    _backlog.clear();
}

void ConsoleInterface::log(const QString& message)
{
    _backlog.append(qMakePair<MessageType, QString>(ConsoleInterface::Log, message));
    emit(backlogChanged(ConsoleInterface::Log, message));
}

void ConsoleInterface::debug(const QString& message)
{
    _backlog.append(qMakePair<MessageType, QString>(ConsoleInterface::Debug, message));
    emit(backlogChanged(ConsoleInterface::Debug, message));
}

void ConsoleInterface::error(const QString& message)
{
    _backlog.append(qMakePair<MessageType, QString>(Error, message));
    emit(backlogChanged(ConsoleInterface::Error, message));
}
