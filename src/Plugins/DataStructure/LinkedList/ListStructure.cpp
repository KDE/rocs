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

Rocs::ListStructure::ListStructure ( Document* parent ) : DataStructure ( parent ) {
  _animationGroup = new QParallelAnimationGroup(this);
  createFront();
  arrangeNodes();
}

Rocs::ListStructure::ListStructure(DataStructure& other, Document * parent): DataStructure(other, parent)
{
  _animationGroup = new QParallelAnimationGroup(this);
  createFront();
  arrangeNodes();
}

Rocs::ListStructure::~ListStructure() {
  _animationGroup->deleteLater();;
}

void Rocs::ListStructure::createFront()
{
  _front = static_cast<ListNode*>(addData("P"));
  _front->setShowName(true);
  _front->setShowValue(false);
}

Pointer* Rocs::ListStructure::addPointer ( Data* from, Data* to ) {
    foreach(Pointer *e, from->adjacent_pointers()){
        e->remove();
    }

    Pointer * e =  DataStructure::addPointer ( from, to );
    arrangeNodes();
    return e;
}

Data* Rocs::ListStructure::addData ( QString name ) {

    ListNode *n = static_cast<ListNode*>(DataStructure::addData(new ListNode(this)));
    n->setName(name);
    return n;
}

QScriptValue Rocs::ListStructure::front() {
  return _front->scriptValue();
}

QScriptValue Rocs::ListStructure::createNode(const QString & name){
    Data * n = addData(name);
    n->setEngine( engine() );
    return n->scriptValue();
}

void Rocs::ListStructure::arrangeNodes(){
  qreal x;
  qreal y;
  if (_animationGroup->state() != QAnimationGroup::Stopped){
      _animationGroup->stop();
  }
  QScopedArrayPointer<bool>visited (new bool[dataList().count()]);
  for (int i = 0; i < dataList().count(); ++i){
      visited[i] = false;
  }

  QPropertyAnimation * anim = new QPropertyAnimation(_front, "x");;
  anim->setDuration(500);
  anim->setStartValue(_front->x());
  anim->setEndValue(40);
  _animationGroup->addAnimation(anim);
  anim = new QPropertyAnimation(_front, "y");;
  anim->setDuration(500);
  anim->setStartValue(_front->y());
  anim->setEndValue(120);
  _animationGroup->addAnimation(anim);

  visited[dataList().indexOf(_front)] = true;
  ListNode * n = _front;

  x = n->width() * 40;
  y = 250;
  while ((n = n->next())){
    if (visited[dataList().indexOf(n)] || n == _front){
      break;
    }
    visited[dataList().indexOf(n)] = true;
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
  foreach (Data * n, dataList()){
    if (!visited[dataList().indexOf(n)]){
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
