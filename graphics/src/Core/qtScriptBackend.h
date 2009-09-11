/***************************************************************************
 * main.cpp
 * This file is part of the KDE project
 * copyright (C)2004-2007 by Tomaz Canabrava (tomaz.canabrava@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/
#ifndef QTSCRIPTBACKEND_H
#define QTSCRIPTBACKEND_H

#include <QList>
#include <QtScript>
#include <QScriptValue>
#include <QScriptString>
#include <QScriptContext>

class Graph;
class GraphDocument;
class KTextBrowser;

class QtScriptBackend : public QScriptEngine {
public:
    QtScriptBackend(GraphDocument& graphs,  KTextBrowser *debugArea);
    void setScript(const QString& s);
    void loadFile(const QString& file);
    void setProperty ( QScriptValue & object, const QScriptString & name, uint id, const QScriptValue & value );
    QScriptValue property ( const QScriptValue & object, const QScriptString & name, uint id );
private:
    QString _script;
    GraphDocument& _graphs;
    KTextBrowser  *_debugArea;
    void createGraphList();
};

#endif
