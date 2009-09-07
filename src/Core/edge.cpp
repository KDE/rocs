
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
#include <KDebug>

Edge::Edge(QObject *parent, Node *from, Node *to) :
        QObject(parent),
        _from(from),
        _to(to)
{
    _color = qobject_cast<Graph*>(parent)->edgeDefaultColor();

    if ( from == to ) {
        connect(from, SIGNAL(posChanged()), this, SIGNAL(posChanged()));
        from -> addSelfEdge(this);
    }
    else {
        connect(from, SIGNAL(posChanged()), this, SIGNAL(posChanged()));
        from -> addOutEdge(this);
        connect(to, SIGNAL(posChanged()), this, SIGNAL(posChanged()));
        to -> addInEdge(this);
    }
    connect(parent, SIGNAL(complexityChanged(bool)), this, SIGNAL(updateNeeded()));
    
    _relativeIndex = _to -> edges(_from).size();
    _showName = true;
    _showValue = true;

}

Edge::~Edge() {
    if (_from == _to) {
        _from->removeEdge(this, Node::Self);
    }
    else {
        _from->removeEdge(this, Node::Out);
        _to->removeEdge(this, Node::In);
    }
    _from = 0;
    _to = 0;
    emit removed();
}

void Edge::remove() {
    qobject_cast<Graph*>(QObject::parent())->remove(this);
}

    
bool Edge::showName(){
    return _showName;
}

bool Edge::showValue(){
  return _showValue;
}

void Edge::hideName(bool b){
  _showName = b;
  emit updateNeeded();
  kDebug() << "Hide Name: " << b;
}

void Edge::hideValue(bool b){
  _showValue = b;
  emit updateNeeded();
  kDebug() << "Hide Value: " << b;
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
    _scriptvalue = _engine->newQObject(this);
}

QScriptValue Edge::scriptValue() const {
    return  _scriptvalue;
}
void Edge::self_remove() {
    remove();
}
#endif
