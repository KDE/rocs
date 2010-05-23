/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "AlignAction.h"
#include <KIcon>
#include "GraphVisualEditor.h"
#include "GraphScene.h"
#include "generics.h"
#include <KDebug>

AlignAction::AlignAction(const QString& tooltip,AlignAction::Orientation o, QWidget *parent)
        : KAction(KIcon(), tooltip, parent) {
    m_orientation = o;
    connect(this, SIGNAL(triggered()), this, SLOT(align()));
    kDebug() << " Allign Action Created!";
    switch (o) {
      case Left :    setIcon(KIcon("rocsallignleft"));   break;
      case  Right :  setIcon(KIcon("rocsallignright"));  break; 
      case Top :     setIcon(KIcon("rocsalligntop"));    break;
      case Bottom :  setIcon(KIcon("rocsallignbottom")); break;
      case HCenter : setIcon(KIcon("rocsallignhmiddle"));break;
      case VCenter : setIcon(KIcon("rocsallignvmiddle"));break;
    }
}

void AlignAction::align() {
    GraphVisualEditor *gEditor = qobject_cast<GraphVisualEditor*>(parent());
    QList<NodeItem*> l = gEditor->selectedNodes();

    if (l.size() < 1) return;
    gEditor->scene()->setHideEdges(true);
    switch (m_orientation) {
    case Left :
    case VCenter :
        qSort(l.begin(), l.end(),  leftLessThan);
        allignX(l);
        break;
    case Bottom :
    case HCenter :
        qSort(l.begin(), l.end(), bottomLessThan);
        allignY(l);
        break;
    case Right :
        qSort(l.begin(), l.end(), rightLessThan);
        allignX(l);
        break;
    case Top :
        qSort( l.begin(), l.end(), topLessThan);
        allignY(l);
        break;
    }

    gEditor->scene()->setHideEdges(false);
}

void AlignAction::allignY(QList<NodeItem*>& l) {
    qreal final = l[0]->node()->y();
    
    kDebug() << "final sould be:" << final;
    if (m_orientation == VCenter || m_orientation == HCenter) {
        qreal otherSide = l[l.size()-1]->node()->y();
        final = (final + otherSide) / 2;
    }
    foreach(NodeItem *i, l) {
        i->node()->setY(final);
    }
    kDebug() << "but it is:" << l[0]->node()->y();
}

void AlignAction::allignX(QList<NodeItem*>& l) {
    qreal final = l[0]->node()->x();
    kDebug() << "final should be:" << final;
    if (m_orientation == VCenter || m_orientation == HCenter) {
        qreal otherSide = l[l.size()-1]->node()->x();
        final = (final + otherSide) / 2;
    }
    foreach(NodeItem *i, l) {
        i->node()->setX(final);
    }
    kDebug() << "but it is:" << l[0]->node()->x();
}
