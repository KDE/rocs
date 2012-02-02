/*
    This file is part of Rocs.
    Copyright 2011  Wagner Reck <wagner.reck@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of 
    the License, or (at your option) any later version.

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
#include <boost/shared_ptr.hpp>


namespace boost { void throw_exception( std::exception const & ) {} }

DataStructurePtr Rocs::ListStructure::create(Document *parent) {
    return DataStructure::create<ListStructure>(parent);
}

DataStructurePtr Rocs::ListStructure::create(DataStructurePtr other, Document *parent) {
    boost::shared_ptr<ListStructure> ds = boost::static_pointer_cast<ListStructure>(DataStructure::create<ListStructure>(parent));
    ds->importStructure(other);
    return ds;
}


Rocs::ListStructure::ListStructure ( Document* parent )
      : DataStructure ( parent )
      , m_building(true) {
      m_building = false;
      init();
}

void Rocs::ListStructure::importStructure(DataStructurePtr other) {
    m_building = true;
    QHash < Data*, DataPtr > dataTodata;
    foreach(DataPtr n, other->dataList()){
        DataPtr newdata = addData(n->name());
        newdata->setColor(n->color());
        newdata->setValue(n->value());
        newdata->setX(n->x());
        newdata->setY(n->y());
        newdata->setWidth(n->width());
        dataTodata.insert(n.get(), newdata);
    }
    foreach(PointerPtr e, other->pointers()){
        DataPtr from =  dataTodata.value(e->from().get());
        DataPtr to =  dataTodata.value(e->to().get());

        PointerPtr newPointer = addPointer(from, to);
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
      m_begin = boost::static_pointer_cast<ListNode>( dataList().first());
      arrangeNodes();
  }else{
      m_begin = boost::shared_ptr<ListNode>();
  }
}


Rocs::ListStructure::~ListStructure() {
  m_animationGroup->deleteLater();;
}

PointerPtr Rocs::ListStructure::addPointer ( DataPtr from, DataPtr to ) {
    foreach(PointerPtr e, from->out_pointers() + from->self_pointers()){
        e->remove();
    }

    PointerPtr e =  DataStructure::addPointer ( from, to );
    arrangeNodes();
    return e;
}

DataPtr Rocs::ListStructure::addData ( QString name ) {
    boost::shared_ptr<ListNode> n = boost::static_pointer_cast<ListNode>( ListNode::create(getDataStructure()) );
    n->setName(name);

    if (m_building) {
        return addData(n);;
    }
        
    if (m_begin){
      boost::shared_ptr<ListNode> tmp = m_begin;
      while (tmp->next() != 0)      tmp = tmp->next();

      tmp->pointTo(n);
    }else{
      m_begin = n;
    }
    addData(n);
    arrangeNodes();
    return n;
}

void Rocs::ListStructure::remove(DataPtr n)
{
    if (m_begin == n){
        m_begin = boost::static_pointer_cast<ListNode>(n)->next();
    }
    DataStructure::remove(n);
    arrangeNodes();
}


QScriptValue Rocs::ListStructure::begin() {
  return m_begin->scriptValue();
}

void Rocs::ListStructure::setBegin(boost::shared_ptr< ListNode > node)
{
    m_begin = node;
    arrangeNodes();
}


void Rocs::ListStructure::remove(PointerPtr ptr){
    DataStructure::remove(ptr);
    arrangeNodes();
}

QScriptValue Rocs::ListStructure::createNode(const QString & name){
    boost::shared_ptr<ListNode> n = boost::static_pointer_cast<ListNode>(DataStructure::addData(ListNode::create(getDataStructure())));
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
   boost::shared_ptr<ListNode> n = m_begin;
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
      anim = new QPropertyAnimation(n.get(), "x");;
      anim->setDuration(500);
      anim->setStartValue(n->x());
      anim->setEndValue(x);
      m_animationGroup->addAnimation(anim);
      anim = new QPropertyAnimation(n.get(), "y");;
      anim->setDuration(500);
      anim->setStartValue(n->y());
      anim->setEndValue(y);
      m_animationGroup->addAnimation(anim);
    }while ((n = n->next()));
  }
  x = 50 + size.left();
  y += 100;
  foreach (DataPtr n, dataList()){
    if (!visited[dataList().indexOf(n)]){
      anim = new QPropertyAnimation(n.get(), "x");;
      anim->setDuration(500);
      anim->setStartValue(n->x());
      anim->setEndValue(x);
      m_animationGroup->addAnimation(anim);
      anim = new QPropertyAnimation(n.get(), "y");;
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
