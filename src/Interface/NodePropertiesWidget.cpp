#include "NodePropertiesWidget.h"
#include "node.h"
#include "MainWindow.h"
#include <KDebug>
#include "NodeItem.h"
#include "graph.h"
#include "model_GraphProperties.h"

NodePropertiesWidget::NodePropertiesWidget (MainWindow* /*parent*/  ): QWidget(0) {
    setupUi(this);
    _item = 0;
}

void NodePropertiesWidget::setNode(NodeItem *n, QPointF pos) {
  
    _node = n->node();
    if (! _item ){
      _svgFile = _node->iconPackage();
    }
    
    _item = n;
    move(pos.x()+ 10,  pos.y() + 10);
    show();
    activateWindow();
    raise();
    disconnect();
    
    reflectAttributes();
    
    connect(_node, SIGNAL(changed()), this, SLOT(reflectAttributes()));
    connect(_node->parent(), SIGNAL(automateChanged(bool)), this, SLOT(updateAutomateAttributes(bool)));
    
    connect( _showName,     SIGNAL(toggled(bool)),          _node, SLOT(hideName(bool)));
    connect( _showValue,    SIGNAL(toggled(bool)),          _node, SLOT( hideValue(bool)));
    connect( _begin,        SIGNAL(toggled(bool)),          _node, SLOT(setBegin(bool)));
    connect( _end,          SIGNAL(toggled(bool)),          _node, SLOT(setEnd(bool)));
    connect( _name,         SIGNAL(textChanged(QString)),   _node, SLOT(setName(QString)));
    connect( _value,        SIGNAL(textChanged(QString)),   _node, SLOT(setValue(QString)));
    connect( _x,            SIGNAL(valueChanged(int)),      _node, SLOT(setX(int)));
    connect( _y,            SIGNAL(valueChanged(int)),      _node, SLOT(setY(int)));
    connect( _width,        SIGNAL(valueChanged(double)),   _node, SLOT(setWidth(double)));
    
    GraphPropertiesModel *model = new GraphPropertiesModel();
    model->setDataSource(_node);
    
    _propertiesTable->setModel(model);
    
}

void NodePropertiesWidget::reflectAttributes(){
   _color->setColor(_node->color());
   _x->setValue(_node->x());
   _y->setValue(_node->y());
   _name->setText(_node->name());
   _value->setText(_node->value().toString());
   _width->setValue(_node->width());
   _showName->setChecked(!_node->showName());
   _showValue->setChecked(!_node->showValue());
   updateAutomateAttributes(qobject_cast< Graph* >(_node->parent())->automate());
   _propertyName->setText("");
   _propertyValue->setText("");
   _isPropertyGlobal->setCheckState(Qt::Unchecked);
   if (( _svgFile == _node->iconPackage()) && (_images->count() != 0)){
      kDebug() << _svgFile << "already set, and images combo box is not empty";
      return;
   }
    _images->clear();
    QFile svgFile(_item->node()->iconPackage());
    if (!svgFile.open(QIODevice::ReadOnly | QIODevice::Text)){
      kDebug() << "could not open file for reading";
      return;
    }
    
    QXmlStreamReader reader(&svgFile);
    while(!reader.atEnd()){
      reader.readNext();
      if ( reader.attributes().hasAttribute("id")){
          QString attribute = reader.attributes().value("id").toString();
          if (attribute.startsWith("rocs_")){
              attribute.remove("rocs_");
              _images->addItem(attribute);
          }
      }
    }
}

void NodePropertiesWidget::on__images_activated(const QString& s)
{
  _node->setIcon("rocs_"+s);
}

void NodePropertiesWidget::on__color_activated(const QColor& c) { 
  _node->setColor(c.name()); 
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

void NodePropertiesWidget::on__addProperty_clicked(){
//     if (_isPropertyGlobal->checkState() == Qt::Checked) {
//       _node->graph()->addNodesDinamicProperty(_propertyName->text(),
// 					      QVariant(_propertyValue->text()));
//     }else{
//       _node->addDinamicProperty(_propertyName->text(),
// 				QVariant(_propertyValue->text()));
//     }
    GraphPropertiesModel *model =  qobject_cast< GraphPropertiesModel*>(_propertiesTable->model());
    model->addDinamicProperty(_propertyName->text(),
					    QVariant(_propertyValue->text()),
					    _node,
					    (_isPropertyGlobal->checkState() == Qt::Checked));
}
