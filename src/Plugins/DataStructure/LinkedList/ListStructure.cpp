/*
    Plugin that implements a list structure in Rocs
    Copyright (C) 2011 Wagner Reck

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

Rocs::ListStructure::ListStructure ( Document* parent )
      : DataStructure ( parent )
      , m_building(true) {
      m_building = false;
      init();
}

Rocs::ListStructure::ListStructure(DataStructure& other, Document * parent)
      : DataStructure(other, parent)
      , m_building(true)
{
    QHash <Data*, Data* > dataTodata;
    foreach(Data* n, other.dataList()){
        Data* newdata = addData(n->name());
        newdata->setColor(n->color());
        newdata->setValue(n->value());
        newdata->setX(n->x());
        newdata->setY(n->y());
        newdata->setWidth(n->width());
        dataTodata.insert(n, newdata);
    }
    foreach(Pointer *e, other.pointers()){
        Data* from =  dataTodata.value(e->from());
        Data* to =  dataTodata.value(e->to());

        Pointer* newPointer = addPointer(from, to);
        newPointer->setColor(e->color());
        newPointer->setValue(e->value());
    }
    m_building = false;
    init();
}


void Rocs::ListStructure::init()
{
  m_animationGroup = new QParallelAnimationGroup(this);
  if (!dataList().isEmpty()){
      m_begin = qobject_cast< ListNode* >( dataList().first());
      arrangeNodes();
  }else{
      m_begin = 0;
  }
}


Rocs::ListStructure::~ListStructure() {
  m_animationGroup->deleteLater();;
}

Pointer* Rocs::ListStructure::addPointer ( Data* from, Data* to ) {
    foreach(Pointer *e, from->out_pointers()){
        e->remove();
    }

    Pointer * e =  DataStructure::addPointer ( from, to );
    arrangeNodes();
    return e;
}

Data* Rocs::ListStructure::addData ( QString name ) {

    ListNode *n = static_cast<ListNode*>(DataStructure::addData(new ListNode(this)));
    n->setName(name);
    if (m_building)
      return n;
    if (m_begin){
      ListNode * tmp = m_begin;
      while (tmp->next() != 0)      tmp = tmp->next();

      tmp->pointTo(n);
    }else{
      m_begin = n;
    }
    arrangeNodes();
    return n;
}

void Rocs::ListStructure::remove(Data* n)
{
    if (m_begin == n){
      m_begin = dynamic_cast<ListNode*>( n)->next();
    }
    DataStructure::remove(n);
    arrangeNodes();
}


QScriptValue Rocs::ListStructure::begin() {
  return m_begin->scriptValue();
}

void Rocs::ListStructure::setBegin(ListNode * node)
{
    m_begin = node;
    arrangeNodes();
}


void Rocs::ListStructure::remove(Pointer * ptr){
    DataStructure::remove(ptr);
    arrangeNodes();
}

QScriptValue Rocs::ListStructure::createNode(const QString & name){
    ListNode *n = static_cast<ListNode*>(DataStructure::addData(new ListNode(this)));
    n->setName(name);
    n->setEngine( engine() );
    arrangeNodes();
    return n->scriptValue();
}

void Rocs::ListStructure::arrangeNodes(){
  if (m_building)
    return;
  QRectF size = document()->size();
  qreal x;
  qreal y = size.top() + 100;;
  if (m_animationGroup->state() != QAnimationGroup::Stopped){
      m_animationGroup->stop();
  }
  QScopedArrayPointer<bool>visited (new bool[dataList().count()]);
  for (int i = 0; i < dataList().count(); ++i){
      visited[i] = false;
  }

   QPropertyAnimation * anim;
   ListNode * n = m_begin;
   if (n){
    x = size.left() + 50;
    do{
      if (visited[dataList().indexOf(n)]){
        break;
      }
      visited[dataList().indexOf(n)] = true;
      if (x > size.right() - 140 + n->width()*40){
        x = size.left() + 50 + n->width() * 40;
        y += 85;
      }else{
        x  += 70 + n->width()*40;
      }
      anim = new QPropertyAnimation(n, "x");;
      anim->setDuration(500);
      anim->setStartValue(n->x());
      anim->setEndValue(x);
      m_animationGroup->addAnimation(anim);
      anim = new QPropertyAnimation(n, "y");;
      anim->setDuration(500);
      anim->setStartValue(n->y());
      anim->setEndValue(y);
      m_animationGroup->addAnimation(anim);
    }while ((n = n->next()));
  }
  x = 50 + size.left();
  y += 100;
  foreach (Data * n, dataList()){
    if (!visited[dataList().indexOf(n)]){
      anim = new QPropertyAnimation(n, "x");;
      anim->setDuration(500);
      anim->setStartValue(n->x());
      anim->setEndValue(x);
      m_animationGroup->addAnimation(anim);
      anim = new QPropertyAnimation(n, "y");;
      anim->setDuration(500);
      anim->setStartValue(n->y());
      anim->setEndValue(y);
      m_animationGroup->addAnimation(anim);
      x += 60;
      if (x > size.right() - 60){
        x = 50 + size.left();
        y += 50;
      }
    }
  }
  m_animationGroup->start();
}
