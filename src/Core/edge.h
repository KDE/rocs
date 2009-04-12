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

#ifndef EDGE_H
#define EDGE_H

#ifndef USING_QTSCRIPT
#define USING_QTSCRIPT 1
#endif

#include <QObject>
#include <QVariant>
#include <QtScript>

#ifdef USING_QTSCRIPT
#include <QScriptValue>
#include "qtScriptBackend.h"
#endif

class Node;

class Edge : public QObject {
    Q_OBJECT

public:
    Edge(QObject *parent, Node *from, Node *to);
    ~Edge();
    int relativeIndex();

#ifdef USING_QTSCRIPT
    QScriptValue scriptValue() const;
    void setEngine(	QtScriptBackend *engine );
#endif

public  slots:
    Node* from() const;
    Node* to() const;

#ifdef USING_QTSCRIPT
    QScriptValue start();
    QScriptValue end();
#endif

private:
    Node *_from;
    Node *_to;


#ifdef USING_QTSCRIPT
    QScriptValue _value;
    QtScriptBackend *_engine;
#endif

signals:
    void removed();
};

#endif
