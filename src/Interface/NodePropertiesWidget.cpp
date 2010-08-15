#include "NodePropertiesWidget.h"
#include "Data.h"
#include "MainWindow.h"
#include <KDebug>
#include "NodeItem.h"
#include "DataType.h"
#include "model_GraphProperties.h"
#include <DSPluginManager.h>

DatumPropertiesWidget::DatumPropertiesWidget (MainWindow* /*parent*/  ): QWidget(0) {
    setupUi(this);
    _item = 0;
    _datum = 0;
}

void DatumPropertiesWidget::setDatum(DatumItem *n, QPointF pos) {
    if (_datum == n->datum())
      return;

    if (_datum){
      disconnectDatum(_datum);
    }

    _datum = n->datum();
    if (! _item ){
      _svgFile = _datum->iconPackage();
    }

    _item = n;
    move(pos.x()+ 10,  pos.y() + 10);

    show(); activateWindow(); raise();

    reflectAttributes();


    connect(_datum, SIGNAL(changed()), this, SLOT(reflectAttributes()));
//     connect(_datum->parent(), SIGNAL(automateChanged(bool)), this, SLOT(updateAutomateAttributes(bool)));

    connect( _showName,     SIGNAL(toggled(bool)),          _datum, SLOT(hideName(bool)));
    connect( _showValue,    SIGNAL(toggled(bool)),          _datum, SLOT( hideValue(bool)));
//     connect( _begin,        SIGNAL(toggled(bool)),          _datum, SLOT(setBegin(bool)));
//     connect( _end,          SIGNAL(toggled(bool)),          _datum, SLOT(setEnd(bool)));
    connect( _name,         SIGNAL(textChanged(QString)),   _datum, SLOT(setName(QString)));
    connect( _value,        SIGNAL(textChanged(QString)),   _datum, SLOT(setValue(QString)));
//     connect( _x,            SIGNAL(valueChanged(int)),      _datum, SLOT(setX(int)));
//     connect( _y,            SIGNAL(valueChanged(int)),      _datum, SLOT(setY(int)));
//     connect( _width,        SIGNAL(valueChanged(double)),   _datum, SLOT(setWidth(double)));

    GraphPropertiesModel *model = new GraphPropertiesModel();
    model->setDataSource(_datum);

    _propertiesTable->setModel(model);

}

void DatumPropertiesWidget::reflectAttributes(){
    if (extraItens->layout()){
      delete extraItens->layout();
    }
    if(QLayout *lay = Rocs::DSPluginManager::instance()->datumExtraProperties(_datum, this)){
      extraItens->setLayout(lay);
    }
   _color->setColor(_datum->color());
//    _x->setValue(_datum->x());
//    _y->setValue(_datum->y());
   _name->setText(_datum->name());
   _value->setText(_datum->value().toString());
//    _width->setValue(_datum->width());
   _showName->setChecked(_datum->showName());
   _showValue->setChecked(_datum->showValue());
//    updateAutomateAttributes(qobject_cast< Graph* >(_datum->parent())->automate());
   _propertyName->setText("");
   _propertyValue->setText("");
   _isPropertyGlobal->setCheckState(Qt::Unchecked);
   if (( _svgFile == _datum->iconPackage()) && (_images->count() != 0)){
      kDebug() << _svgFile << "already set, and images combo box is not empty";
      return;
   }
    _images->clear();
    QFile svgFile(_item->datum()->iconPackage());
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

void DatumPropertiesWidget::on__images_activated(const QString& s)
{
  _datum->setIcon("rocs_"+s);
}

void DatumPropertiesWidget::on__color_activated(const QColor& c) {
  _datum->setColor(c.name());
}

// void DatumPropertiesWidget::updateAutomateAttributes(bool b){
// //     if (b) {
// //         _begin->setChecked(_datum->begin());
// //         _end->setChecked(_datum->end());
// //         _begin->show();
// //         _end->show();
// //     }
// //     else {
// //         _begin->setChecked(false);
// //         _end->setChecked(false);
// //         _begin->hide();
// //         _end->hide();
// //     }
// }

void DatumPropertiesWidget::on__addProperty_clicked(){

    GraphPropertiesModel *model =  qobject_cast< GraphPropertiesModel*>(_propertiesTable->model());
    model->addDynamicProperty(_propertyName->text(),
                            QVariant(_propertyValue->text()),
                            _datum,
                            (_isPropertyGlobal->checkState() == Qt::Checked));

}

void DatumPropertiesWidget::disconnectDatum(Datum *n){

    disconnect(n, SIGNAL(changed()), this, SLOT(reflectAttributes()));
    disconnect(n->parent(), SIGNAL(automateChanged(bool)), this, SLOT(updateAutomateAttributes(bool)));

    disconnect( _showName,     SIGNAL(toggled(bool)),          n, SLOT(hideName(bool)));
    disconnect( _showValue,    SIGNAL(toggled(bool)),          n, SLOT( hideValue(bool)));
//     disconnect( _begin,        SIGNAL(toggled(bool)),          n, SLOT(setBegin(bool)));
//     disconnect( _end,          SIGNAL(toggled(bool)),          n, SLOT(setEnd(bool)));
    disconnect( _name,         SIGNAL(textChanged(QString)),   n, SLOT(setName(QString)));
    disconnect( _value,        SIGNAL(textChanged(QString)),   n, SLOT(setValue(QString)));
//     disconnect( _x,            SIGNAL(valueChanged(int)),      n, SLOT(setX(int)));
//     disconnect( _y,            SIGNAL(valueChanged(int)),      n, SLOT(setY(int)));
//     disconnect( _width,        SIGNAL(valueChanged(double)),   n, SLOT(setWidth(double)));

}