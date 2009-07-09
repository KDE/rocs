#ifndef ALIGNACTION_H
#define ALIGNACTION_H

#include <KAction>
#include "graphicsitem_Node.h"
#include "node.h"

class AlignAction : public KAction{
  Q_OBJECT
public:
  enum Orientation{Left, Right, Top, Bottom, MiddleHorizontal, MiddleVertical};
  AlignAction(const QString& actionName, const QString& tooltip,AlignAction::Orientation o, QWidget *parent);

private slots:
  void align();

private:
  void allignY(QList<NodeItem*>& l);
  void allignX(QList<NodeItem*>& l);

  Orientation m_orientation;
  
};

#endif