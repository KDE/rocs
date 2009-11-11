#include "NodePropertiesWidget.h"
#include "node.h"
#include "MainWindow.h"
#include <KDebug>
#include "NodeItem.h"
#include "graph.h"

NodePropertiesWidget::NodePropertiesWidget (MainWindow* /*parent*/  ): QWidget(0) {
    setupUi(this);
}

void NodePropertiesWidget::setNode(NodeItem *n, QPointF pos) {
    _item = n;
    _node = n->node();
    move(pos.x()+ 10,  pos.y() + 10);
    show();
    activateWindow();
    raise();
    disconnect();
    connect(_node, SIGNAL(updateNeeded()), this, SLOT(reflectAttributes()));
    connect(_node, SIGNAL(posChanged()), this, SLOT(reflectAttributes()));
    connect(_node->parent(), SIGNAL(automateChanged(bool)), this, SLOT(updateAutomateAttributes(bool)));
    reflectAttributes();

}

void NodePropertiesWidget::reflectAttributes(){
    _color->setColor(_node->color());
    _x->setValue(_node->x());
    _y->setValue(_node->y());
    _name->setText(_node->name());
    _value->setText(_node->value());
    _width->setValue(_node->width());
    _showName->setChecked(!_node->showName());
    _showValue->setChecked(!_node->showValue());
    updateAutomateAttributes(qobject_cast< Graph* >(_node->parent())->automate());
}

void NodePropertiesWidget::updateAutomateAttributes(bool b){
    if (b) {
        _begin->setChecked(_node->begin());
        _end->setChecked(_node->end());
        _begin->show();
        _end->show();
    }
    else {
        _begin->setChecked(false);
        _end->setChecked(false);
        _begin->hide();
        _end->hide();
    }
}

void NodePropertiesWidget::on__showName_toggled(bool b) {
    _node->hideName(!b);
}

void NodePropertiesWidget::on__showValue_toggled(bool b) {
    _node->hideValue(!b);
}

void NodePropertiesWidget::on__begin_toggled(bool b) {
    _node->setBegin(b);
    kDebug() << "begin: " << b;
}

void NodePropertiesWidget::on__end_toggled(bool b) {
    _node->setEnd(b);
    kDebug() << "end: " << b;
}

void NodePropertiesWidget::on__color_activated(const QColor& c) {
    if (! _node ) return;
    _node->setColor(c.name());
}
void NodePropertiesWidget::on__name_textChanged(const QString& s) {
    if (! _node ) return;
    _node->setName(s);
}

void NodePropertiesWidget::on__value_textChanged(const QString& s) {
    if (! _node ) return;
    _node->setValue(s);
}

void NodePropertiesWidget::on__x_valueChanged(int i) {
    if (! _node ) return;
    _node->setX(i);
}

void NodePropertiesWidget::on__y_valueChanged(int i) {
    if (! _node ) return ;
    _node->setY(i);
}

void NodePropertiesWidget::on__width_valueChanged(double i) {
    if (! _node ) {
        return;
    }

    qreal oldW = _node->width();
    if (oldW < i) {
        _item->startUpSizing();
    }
    else {
        _item->startDownSizing();
    }
    _node->setWidth(i);
    if (oldW < i) {
        _item->endUpSizing();
    }
    else {
        _item->endDownSizing();
    }
}

