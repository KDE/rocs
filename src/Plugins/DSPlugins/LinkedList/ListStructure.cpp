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
#include "Pointer.h"
#include <QDebug>

Rocs::ListStructure::ListStructure ( DataTypeDocument* parent ) : DataType ( parent ) {
  qDebug() << "Creating  a list structure";
  setDirected(true);
  Datum * p = addNode("P");
  p->hideName(false);
  p->hideValue(true);
  setBegin(p);
  _animationGroup = new QParallelAnimationGroup(this);
  arrangeNodes();
}

Rocs::ListStructure::ListStructure(DataType& other): DataType(other)
{
  setDirected(true);
  qDebug() << "Criando pelo construtor de cópia";
  _animationGroup = new QParallelAnimationGroup(this);
  setBegin(addNode("P"));
  _begin->hideName(true);
  _begin->hideValue(false);
  
  arrangeNodes();
  
}

Rocs::ListStructure::~ListStructure() {

}

Pointer* Rocs::ListStructure::addEdge ( Datum* from, Datum* to ) {
    foreach(Pointer *e, from->adjacent_pointers()){
      e->self_remove();
    }

    Pointer * e =  DataType::addPointer ( from, to );
    arrangeNodes();
    return e;
}

Datum* Rocs::ListStructure::addNode ( QString name ) {
    
    Datum *n = DataType::addDatum(new ListNode(this));
    n->setName(name);
    arrangeNodes();
    return n;
}

QScriptValue Rocs::ListStructure::front() {
  return begin()->scriptValue();
}

QScriptValue Rocs::ListStructure::createNode(const QString & name){
    Datum * n = addNode(name);
    n->setEngine(_engine);
    return n->scriptValue();
}

void Rocs::ListStructure::arrangeNodes(){
  qreal x;
  qreal y;
  if (_animationGroup->state() != QAnimationGroup::Stopped){
      _animationGroup->stop();
  }
  QScopedArrayPointer<bool>visited (new bool[_data.count()]);
  for (int i = 0; i < _data.count(); ++i){
      visited[i] = false;
  }
  if (_begin == 0){
    return;
  }else{
    qDebug() << _begin;
  }
  qDebug() << "Indo animar os bagaços.";
  
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

  visited[_data.indexOf(_begin)] = true;
  ListNode * n = qobject_cast<ListNode*>(_begin);
  if (n == 0){
    qDebug() << _begin->metaObject()->className();
    return;
  }
  x = n->width() * 40;
  y = 250;
  while ((n = n->next())){
    if (visited[_data.indexOf(n)] || n == _begin){
      break;
    }
    visited[_data.indexOf(n)] = true;
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
  foreach (Datum * n, _data){
    if (!visited[_data.indexOf(n)]){
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

