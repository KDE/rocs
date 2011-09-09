/* 
    This file is part of Rocs.
    Copyright 2009-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>

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

#include "NodePropertiesWidget.h"
#include "Data.h"
#include "MainWindow.h"
#include <KDebug>
#include "DataItem.h"
#include "DataStructure.h"
#include "model_GraphProperties.h"
#include <DataStructurePluginManager.h>
#include <QMap>
#include <QPainter>
#include <QImage>

DataPropertiesWidget::DataPropertiesWidget (MainWindow* /*parent*/  ): 
    QWidget(0),
    _data(),
    _item(0){
    setupUi(this);
    connect(_btnClose, SIGNAL(clicked()), this, SLOT(hide()));
}

void DataPropertiesWidget::setData(DataItem *n, QPointF pos) {
    if (_data == n->data()){
      show(); activateWindow(); raise();
      return;
    }

    if (_data){
        _data->disconnect(this);
        _showName->disconnect(_data.get());
        _showValue->disconnect(_data.get());
        _disableColor->disconnect(_data.get());
        _name->disconnect(_data.get());
        _value->disconnect(_data.get());
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
   
    connect( _showName,     SIGNAL(toggled(bool)),         _data.get(), SLOT(setShowName(bool)));
    connect( _showValue,    SIGNAL(toggled(bool)),         _data.get(), SLOT(setShowValue(bool)));
    connect( _disableColor, SIGNAL(toggled(bool)),         this, SLOT(setUseColor(bool)));
    connect( _name,         SIGNAL(textEdited(QString)),   _data.get(), SLOT(setName(QString)));
    connect( _value,        SIGNAL(textEdited(QString)),   _data.get(), SLOT(setValue(QString)));

    GraphPropertiesModel *model = new GraphPropertiesModel();
    model->setDataSource(_data.get());

    _propertiesTable->setModel(model);
}

void DataPropertiesWidget::setUseColor(bool b){
    _data->setUseColor(!b);
}

void DataPropertiesWidget::reflectAttributes(){
    if (!extraItens->layout()){
        _oldDataStructurePlugin = DataStructurePluginManager::self()->actualPluginName();
    }
    
    if (_oldDataStructurePlugin != DataStructurePluginManager::self()->actualPluginName()){
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
    _disableColor->setChecked(!_data->useColor());
    _propertyName->setText("");
    _propertyValue->setText("");
    _isPropertyGlobal->setCheckState(Qt::Unchecked);
    
    if (( _svgFile == _data->iconPackage()) && (_images->count() != 0)){
        QString icon = _data->icon();
        icon.remove("rocs_");
       _images->setCurrentItem(icon);
       return;
    }
    _images->clear();
    QFile svgFile(_item->data()->iconPackage());
    svgFile.open(QIODevice::ReadOnly | QIODevice::Text);

    QXmlStreamReader reader(&svgFile);
    QSvgRenderer *renderer = _item->_renders.value(svgFile.fileName());
    
    while(!reader.atEnd()){
        reader.readNext();
        if (!reader.attributes().hasAttribute("id")){
            continue;
        }
        QString attribute = reader.attributes().value("id").toString();
        if (attribute.startsWith("rocs_")){
            QImage iconImage = QImage(80, 80, QImage::Format_ARGB32);                                                                                                                                                                                 

            QPainter painter;
            painter.begin(&iconImage);
            renderer->render(&painter, attribute);
            painter.end();

            attribute.remove("rocs_");
            _images->addItem(KIcon(QPixmap::fromImage(iconImage)), attribute);
        }
    }
    QString icon = _data->icon();
    icon.remove("rocs_");
    _images->setCurrentItem(icon);
}

void DataPropertiesWidget::on__images_activated(const QString& s){
  _data->setIcon("rocs_"+s);
}

void DataPropertiesWidget::on__color_activated(const QColor& c){
  _data->setColor(QColor(c));
}

void DataPropertiesWidget::on__addProperty_clicked(){
    GraphPropertiesModel *model =  qobject_cast< GraphPropertiesModel*>(_propertiesTable->model());
    model->addDynamicProperty(_propertyName->text(), QVariant(_propertyValue->text()),
                            _data.get(), (_isPropertyGlobal->checkState() == Qt::Checked));

}
