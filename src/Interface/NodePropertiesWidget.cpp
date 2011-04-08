#include "NodePropertiesWidget.h"
#include "Data.h"
#include "MainWindow.h"
#include <KDebug>
#include "DataItem.h"
#include "DataStructure.h"
#include "model_GraphProperties.h"
#include <DataStructurePluginManager.h>

DataPropertiesWidget::DataPropertiesWidget (MainWindow* /*parent*/  ): QWidget(0) {
    setupUi(this);
    _item = 0;
    _data = 0;
}

void DataPropertiesWidget::setData(DataItem *n, QPointF pos) {
    if (_data == n->data())
      return;

    if (_data){
        disconnect(_data, SIGNAL(changed()), this, SLOT(reflectAttributes()));
    }

    _data = n->data();
    if (! _item ){
      _svgFile = _data->iconPackage();
    }

    _item = n;
    move(pos.x()+ 10,  pos.y() + 10);

    show(); activateWindow(); raise();

    delete extraItens->layout();
    extraItens->setLayout(DataStructurePluginManager::self()->dataExtraProperties(_data, this));
    reflectAttributes();
    
    connect(_data, SIGNAL(changed()), this, SLOT(reflectAttributes()));
   
    connect( _showName,     SIGNAL(toggled(bool)),          this, SLOT( applyChanges() ));
    connect( _showValue,    SIGNAL(toggled(bool)),          this, SLOT( applyChanges() ));
    connect( _name,         SIGNAL(textEdited(QString)),   this, SLOT( applyChanges() ));
    connect( _value,        SIGNAL(textEdited(QString)),   this, SLOT( applyChanges() ));

    GraphPropertiesModel *model = new GraphPropertiesModel();
    model->setDataSource(_data);

    _propertiesTable->setModel(model);
}

void DataPropertiesWidget::applyChanges(){
    if (_data->name() != _name->text())
        _data->setName(_name->text());
    
    if (_data->value() != _value->text())
        _data->setValue(_value->text());
    
    if(_data->showName() != _showName->isChecked())
        _data->setShowName(_showName->isChecked());
    
    if (_data->showValue() != _showValue->isChecked())
        _data->setShowValue(_showValue->isChecked());
}

void DataPropertiesWidget::reflectAttributes(){
    if (!extraItens->layout()){
        _oldDataStructurePlugin = DataStructurePluginManager::self()->actualPlugin();
    }
    
    if (_oldDataStructurePlugin != DataStructurePluginManager::self()->actualPlugin()){
        extraItens->layout()->deleteLater();
    }
    
    if (!extraItens->layout()){
        extraItens->setLayout(DataStructurePluginManager::self()->dataExtraProperties(_data, this));
    }
    
    _color->setColor(_data->color().value<QColor>());
    _name->setText(_data->name());
    _value->setText(_data->value().toString());
    _showName->setChecked(_data->showName());
    _showValue->setChecked(_data->showValue());
    _propertyName->setText("");
    _propertyValue->setText("");
    _isPropertyGlobal->setCheckState(Qt::Unchecked);
    
    if (( _svgFile == _data->iconPackage()) && (_images->count() != 0)){
       return;
    }
    _images->clear();
    QFile svgFile(_item->data()->iconPackage());
    if (!svgFile.open(QIODevice::ReadOnly | QIODevice::Text)){
      kDebug() << "could not open file for reading";
      return;
    }

    QXmlStreamReader reader(&svgFile);
    while(!reader.atEnd()){
        reader.readNext();
        if (!reader.attributes().hasAttribute("id")){
            continue;
        }
        QString attribute = reader.attributes().value("id").toString();
        if (attribute.startsWith("rocs_")){
            attribute.remove("rocs_");
            _images->addItem(attribute);
        }
    }
}

void DataPropertiesWidget::on__images_activated(const QString& s)
{
  _data->setIcon("rocs_"+s);
}

void DataPropertiesWidget::on__color_activated(const QColor& c) {
  _data->setColor(c.name());
}

// void DataPropertiesWidget::updateAutomateAttributes(bool b){
// //     if (b) {
// //         _begin->setChecked(_data->begin());
// //         _end->setChecked(_data->end());
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

void DataPropertiesWidget::on__addProperty_clicked(){

    GraphPropertiesModel *model =  qobject_cast< GraphPropertiesModel*>(_propertiesTable->model());
    model->addDynamicProperty(_propertyName->text(),
                            QVariant(_propertyValue->text()),
                            _data,
                            (_isPropertyGlobal->checkState() == Qt::Checked));

}
