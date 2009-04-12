
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

#include "edge.h"
#include "node.h"
#include "graph.h"

Edge::Edge(QObject *parent, Node *from, Node *to) :
        QObject(parent),
        _from(from),
        _to(to)
{
    setProperty("color", "#FF00FF");
}

Edge::~Edge()
{
    emit removed();
    if (_from == _to) {
        _from->removeEdge(this, Node::Self);
    }
    else {
        _from->removeEdge(this, Node::Out);
        _to->removeEdge(this, Node::In);
    }
    _from = 0;
    _to = 0;
}

Node* Edge::from() const {
    return _from;
}

Node* Edge::to() const {
    return _to;
}

int Edge::relativeIndex() {
    QList<Edge*> list = _to -> edges(_from);
    return list.indexOf(this);
}
#ifdef USING_QTSCRIPT

QScriptValue Edge::start() {
    return _from->scriptValue();
}

QScriptValue  Edge::end() {
    return _to->scriptValue();
}

void Edge::setEngine(	QtScriptBackend *engine ) {
    _engine = engine;
    _value = _engine->newQObject(this);
}

QScriptValue Edge::scriptValue() const {
    return  _value;
}
#endif
