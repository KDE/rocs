#include "AlignAction.h"
#include <KIcon>
#include "SUI_GraphVisualEditor.h"
#include "generics.h"
#include <KDebug>

AlignAction::AlignAction(const QString& actionName, const QString& tooltip,AlignAction::Orientation o, QWidget *parent)
  : KAction(KIcon(actionName), tooltip, parent){
  m_orientation = o;
  connect(this, SIGNAL(triggered()), this, SLOT(align()));
    kDebug() << " Allign Action Created!";
}

void AlignAction::align(){
  QList<NodeItem*> l = qobject_cast<GraphVisualEditor*>(parent())->selectedNodes();
  if (l.size() < 1) return;
  switch(m_orientation){
    case Left : 
    case MiddleVertical : 
	  qSort(l.begin(), l.end(),  leftLessThan); 
	  allignX(l); 
    break;
    case Bottom : 
    case MiddleHorizontal : 
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
}

void AlignAction::allignY(QList<NodeItem*>& l){
  qreal final = l[0]->pos().y();
  if (m_orientation == MiddleHorizontal || m_orientation == MiddleVertical){
    qreal otherSide = l[l.size()-1]->pos().y();
    qreal final = (final + otherSide) / 2;
  }
  foreach(NodeItem *i, l) {
    i->node()->setY(final);
  }
}

void AlignAction::allignX(QList<NodeItem*>& l){
  qreal final = l[0]->pos().x();
  if (m_orientation == MiddleHorizontal || m_orientation == MiddleVertical){
    qreal otherSide = l[l.size()-1]->pos().x();
    qreal final = (final + otherSide) / 2;
  }
  foreach(NodeItem *i, l) {
    i->node()->setX(final);
  }
}
