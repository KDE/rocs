#include "NodePropertiesWidget.h"
#include "node.h"
#include "MainWindow.h"
#include <KDebug>

NodePropertiesWidget::NodePropertiesWidget (MainWindow* /*parent*/  ): QWidget(0){
  setupUi(this);
}

void NodePropertiesWidget::setNode(Node *n){
  _node = n;
  _color->setColor(n->color());
  _x->setValue(n->x());
  _y->setValue(n->y());
  _name->setText(n->name());
  _value->setText(n->value());
  _width->setValue(n->width()*4);
  move(n->x() + 10, n->y() + 10);
  show();
  activateWindow();
  raise();
}

void NodePropertiesWidget::on__color_activated(const QColor& c){
  if (! _node ) return;
  _node->setColor(c.name());
}
void NodePropertiesWidget::on__name_textChanged(const QString& s){
  if (! _node ) return;
  _node->setName(s);
}

void NodePropertiesWidget::on__value_textChanged(const QString& s){
  if (! _node ) return;
  _node->setValue(s);
}

void NodePropertiesWidget::on__x_valueChanged(int i){
  if (! _node ) return;
  _node->setX(i);
}

void NodePropertiesWidget::on__y_valueChanged(int i){
  if (! _node ) return ;
  _node->setY(i);
}

void NodePropertiesWidget::on__width_valueChanged(int i){
  kDebug() << "Entrando no changed";
  if (! _node ){
    kDebug() << "Weeee";
    return;
  }
  qreal width = ( (qreal)i / 4);
  if (width < 0.5) width = 0.5;
  _node->setWidth(width);
}
