/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "ListStructure.h"
#include "KDebug"
#include "ListNode.h"

Rocs::ListStructure::ListStructure ( GraphDocument* parent ) : Graph ( parent ) {
  setDirected(true);
  Node * p = addNode("P");
  p->hideName(false);
  p->hideValue(true);
  setBegin(p);
  _animationGroup = new QParallelAnimationGroup(this);
  arrangeNodes();

}

Rocs::ListStructure::ListStructure(Graph& other): Graph(other)
{
  setDirected(true);
  _animationGroup = new QParallelAnimationGroup(this);
  if (_begin == 0){
    Node * p = addNode("P");
    p->hideName(true);
    p->hideValue(false);
    setBegin(p);

    arrangeNodes();
  }

}


Rocs::ListStructure::~ListStructure() {

}


Edge* Rocs::ListStructure::addEdge ( Node* from, Node* to ) {
    foreach(Edge *e, from->adjacent_edges()){
      e->self_remove();
//       remove(e);
    }

    Edge * e =  Graph::addEdge ( from, to );
    arrangeNodes();
    return e;
}

void Rocs::ListStructure::setEngine ( QScriptEngine* engine )
{
    _engine = engine;

    _value = _engine->newQObject(this);

    if (! name().isEmpty() ) {
        _engine->globalObject().setProperty(name(), _value);
    }

    foreach(Node *n, nodes()) {
        n->setEngine(engine);
    }
    foreach(Edge *e, edges()) {
        e->setEngine(engine);
    }
}


Node* Rocs::ListStructure::addNode ( QString name ) {
      if (_readOnly) return 0;

    Node  *n = new ListNode(this);
    n->setName(name);
    _nodes.append( n );
    emit nodeCreated(n);
    connect (n, SIGNAL(changed()), this, SIGNAL(changed()));
    arrangeNodes();
    return n;
//     return Graph::addNode ( name );
}


QScriptValue Rocs::ListStructure::front() {
  return begin()->scriptValue();
}

QScriptValue Rocs::ListStructure::createNode(const QString & name){
    Node * n = addNode(name);
    n->setEngine(_engine);
    return n->scriptValue();
}


void Rocs::ListStructure::arrangeNodes(){
  qreal x;
  qreal y;
  if (_animationGroup->state() != QAnimationGroup::Stopped){
    _animationGroup->stop();
  }
  QScopedArrayPointer<bool>visited (new bool[_nodes.count()]);
  for (int i = 0; i < _nodes.count(); ++i){
    visited[i] = false;
  }
  if (_begin == 0){
    return;
  }
  QPropertyAnimation * anim = new QPropertyAnimation(_begin, "x");;
  anim->setDuration(500);
  anim->setStartValue(_begin->x());
  anim->setEndValue(40);
  _animationGroup->addAnimation(anim);
  anim = new QPropertyAnimation(_begin, "y");;
  anim->setDuration(500);
  anim->setStartValue(_begin->y());
  anim->setEndValue(120);
  _animationGroup->addAnimation(anim);

  visited[_nodes.indexOf(_begin)] = true;
  ListNode * n = qobject_cast<ListNode*>(_begin);
  x = n->width() * 40;
  y = 250;
  while ((n = n->next())){
    if (visited[_nodes.indexOf(n)] || n == _begin){
      break;
    }
    visited[_nodes.indexOf(n)] = true;
    x = x + 70 + n->width()*40;
    anim = new QPropertyAnimation(n, "x");;
    anim->setDuration(500);
    anim->setStartValue(n->x());
    anim->setEndValue(x);
    _animationGroup->addAnimation(anim);
    anim = new QPropertyAnimation(n, "y");;
    anim->setDuration(500);
    anim->setStartValue(n->y());
    anim->setEndValue(y);
    _animationGroup->addAnimation(anim);
  }
  x = y = 30;
  foreach (Node * n, _nodes){
    if (!visited[_nodes.indexOf(n)]){
      anim = new QPropertyAnimation(n, "x");;
      anim->setDuration(500);
      anim->setStartValue(n->x());
      anim->setEndValue(x);
      _animationGroup->addAnimation(anim);
      anim = new QPropertyAnimation(n, "y");;
      anim->setDuration(500);
      anim->setStartValue(n->y());
      anim->setEndValue(y);
      _animationGroup->addAnimation(anim);
//       n->setX(x);
//       n->setY(y);
      x += 60;
    }
  }
  _animationGroup->start();
}

