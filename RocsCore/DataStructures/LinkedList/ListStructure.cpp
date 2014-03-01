/*
    This file is part of Rocs.
    Copyright 2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2013  Andreas Cord-Landwehr <cordlandwehr@kde.org>

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
#include "ListNode.h"
#include "Pointer.h"
#include <KLocale>
#include <QDebug>
#include <boost/shared_ptr.hpp>


DataStructurePtr Rocs::ListStructure::create(Document *parent)
{
    return DataStructure::create<ListStructure>(parent);
}

DataStructurePtr Rocs::ListStructure::create(DataStructurePtr other, Document *parent)
{
    boost::shared_ptr<ListStructure> ds = boost::static_pointer_cast<ListStructure>(create(parent));
    ds->importStructure(other);
    return ds;
}

Rocs::ListStructure::ListStructure(Document* parent)
    : DataStructure(parent)
    , m_building(true)
{
    m_building = false;
    init();
}

void Rocs::ListStructure::importStructure(DataStructurePtr other)
{
    m_building = true;
    QHash < Data*, DataPtr > dataTodata;
    foreach (int type, other->document()->dataTypeList()) {
        foreach (DataPtr n, other->dataList(type)) {
            DataPtr newdata = createData("", type);
            newdata->setColor(n->color());
            newdata->setProperty("value", n->property("value").toString());
            newdata->setX(n->x());
            newdata->setY(n->y());
            newdata->setWidth(n->width());
            dataTodata.insert(n.get(), newdata);
            foreach (const QString &property, other->document()->dataType(type)->properties()) {
                newdata->setProperty(property.toLatin1(), n->property(property.toLatin1()));
            }
        }
    }
    foreach (int type, other->document()->pointerTypeList()) {
        foreach (PointerPtr e, other->pointers(type)) {
            DataPtr from =  dataTodata.value(e->from().get());
            DataPtr to =  dataTodata.value(e->to().get());

            PointerPtr newPointer = createPointer(from, to, type);
            newPointer->setColor(e->color());
            newPointer->setProperty("value", e->property("value").toString());
            foreach (const QString &property, other->document()->pointerType(type)->properties()) {
                newPointer->setProperty(property.toLatin1(), e->property(property.toLatin1()));
            }
        }
    }
    m_building = false;
    init();
}


void Rocs::ListStructure::init()
{
    connect(this, SIGNAL(changed()), this, SLOT(arrangeNodes()));

    m_animationGroup = new QParallelAnimationGroup(parent());
    if (!dataList(0).isEmpty()) {
        m_begin = boost::static_pointer_cast<ListNode>(dataList(0).first());
    }
}

Rocs::ListStructure::~ListStructure()
{
    m_building = true;
    m_animationGroup->stop();
// FIXME this is a memory leak, but fix this later
// since delete later guides to crash on deletion
//     m_animationGroup->deleteLater();
}

PointerPtr Rocs::ListStructure::createPointer(DataPtr from, DataPtr to, int pointerType)
{
    foreach(PointerPtr e, from->outPointerList()) {
        e->remove();
    }

    PointerPtr e = DataStructure::createPointer(from, to, pointerType);
    return e;
}

DataPtr Rocs::ListStructure::createData(const QString& name, int dataType)
{
    boost::shared_ptr<ListNode> n = boost::static_pointer_cast<ListNode>(
                                        ListNode::create(getDataStructure(), generateUniqueIdentifier(), dataType)
                                    );
    n->setProperty("name", name);
    addData(n);

    // insert node as new head
    n->pointTo(m_begin);
    m_begin = n;
    return n;
}

void Rocs::ListStructure::remove(DataPtr n)
{
    if (m_begin == n) {
        m_begin = boost::static_pointer_cast<ListNode>(n)->next();
    }
    DataStructure::remove(n);
}

QScriptValue Rocs::ListStructure::head()
{
    return m_begin->scriptValue();
}

void Rocs::ListStructure::setHead(Data *headNode)
{
    Q_ASSERT(headNode);
    if (!headNode) {
        return;
    }
    //FIXME: we need resorting!
    m_begin = boost::static_pointer_cast<ListNode>(headNode->getData());
}

QScriptValue Rocs::ListStructure::begin()
{
    emit scriptError(i18n("The method \"%1\" is deprecated, please use \"%2\" instead.",
        QString("begin()"),
        QString("head()")));
    return head();
}

void Rocs::ListStructure::setBegin(Data* node)
{
    emit scriptError(i18n("The method \"%1\" is deprecated, please use \"%2\" instead.",
        QString("setBegin(node)"),
        QString("setHead(node)")));
    setHead(node);
}

void Rocs::ListStructure::remove(PointerPtr ptr)
{
    DataStructure::remove(ptr);
}

QScriptValue Rocs::ListStructure::createNode()
{
    return createNode(0);
}

QScriptValue Rocs::ListStructure::createNode(int type)
{
    boost::shared_ptr<ListNode> n = boost::static_pointer_cast<ListNode>(
                                        DataStructure::addData(ListNode::create(getDataStructure(), generateUniqueIdentifier(), type))
                                    );
    n->setEngine(engine());
    return n->scriptValue();
}

void Rocs::ListStructure::arrangeNodes()
{
    if (m_building) {
        return;
    }

    QRectF size = document()->sceneRect();
    qreal x;
    qreal y = size.top() + 100;;
    if (m_animationGroup->state() != QAnimationGroup::Stopped) {
        m_animationGroup->stop();
    }
    //FIXME only default data type considered
    QScopedArrayPointer<bool>visited(new bool[dataList(0).count()]);
    for (int i = 0; i < dataList(0).count(); ++i) {
        visited[i] = false;
    }

    QPropertyAnimation * anim;
    boost::shared_ptr<ListNode> n = m_begin;
    if (n) {
        x = size.left() + 50;
        do {
            if (visited[dataList(0).indexOf(n)]) {
                break;
            }
            visited[dataList(0).indexOf(n)] = true;
            if (x > size.right() - 140 + n->width() * 40) {
                x = size.left() + 50 + n->width() * 40;
                y += 85;
            } else {
                x  += 70 + n->width() * 40;
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
        } while ((n = n->next()));
    }
    x = 50 + size.left();
    y += 100;
    //FIXME only default data type considered
    foreach(DataPtr n, dataList(0)) {
        if (!visited[dataList(0).indexOf(n)]) {
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
            if (x > size.right() - 60) {
                x = 50 + size.left();
                y += 50;
            }
        }
    }
    m_animationGroup->start();
}
