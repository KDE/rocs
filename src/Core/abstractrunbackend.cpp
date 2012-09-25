/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  <copyright holder> <email>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "abstractrunbackend.h"
#include "Document.h"
#include <KDebug>
#include <QFile>

AbstractRunBackend::AbstractRunBackend(QObject* parent): QObject(parent)
{}

void AbstractRunBackend::debug(const QString& s)
{
    emit sendDebug(s);
}
void AbstractRunBackend::output(const QString& s)
{
    emit sendOutput(s);
}

void AbstractRunBackend::setScript(const QString& s,Document *graphs ) {
    _script = s;
    _document = graphs; 
}

void AbstractRunBackend::loadFile(const QString& file) {
    _script.clear();
    QFile f(file);
    if  (  !f.open(QIODevice::ReadOnly | QIODevice::Text ) ) {
        kDebug() << "File not found";
        return;
    }
    
    _script += f.readAll();
    _script += '\n';
}
