#include "DataItem.h"
#include <kstandarddirs.h>
#include <QDir>
#include <KGlobal>
#include <KDebug>
#include <QGraphicsColorizeEffect>
#include <QFont>
#include <QGraphicsScene>
#include <KLocale>
#include "Data.h"
#include "Interface/ConfigureDefaultProperties.h"
#include <DocumentManager.h>
#include <DataStructure.h>
#include "GraphicsLayout.h"

QMap<QString, QSvgRenderer*> DataItem::_renders;    

DataItem::DataItem(Data* n) 
: QGraphicsSvgItem(0)
,_data(n)
,_iconPackage(n->iconPackage())
,_name(0)
,_value(0)
,_colorizer(0)
,_font(QFont("Helvetica [Cronyx]", 12))
,_originalWidth(n->width())
{
    connect(n, SIGNAL(removed()), this, SLOT(deleteLater()));
    connect(n, SIGNAL(iconChanged(QString)), this, SLOT(updateIcon()));
    connect(n, SIGNAL(nameChanged(QString)), this, SLOT(updateName()));
    connect(n, SIGNAL(valueChanged(QVariant)), this, SLOT(updateValue()));
    connect(n, SIGNAL(colorChanged(QColor)), this, SLOT(updateColor()));
    connect(n, SIGNAL(posChanged(QPointF)), this, SLOT(updatePos()));
    connect(n, SIGNAL(nameVisibilityChanged(bool)), this, SLOT(updateName()));
    connect(n, SIGNAL(valueVisibilityChanged(bool)), this, SLOT(updateValue()));
    connect(n, SIGNAL(useColorChanged(bool)), this, SLOT(updateColor()));
    connect(n, SIGNAL(widthChanged(double)), this, SLOT(updateSize()));
    
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setupNode();
    setZValue(1);
    setFlag(ItemIsSelectable, true);
}

DataItem::~DataItem()
{

}

void DataItem::setupNode(){
    updateRenderer();
    updateIcon();
    updateName();
    updateValue();
    updateColor();
    updateSize();
    updatePos();
    update();
}

void DataItem::updatePos(){
    int fixPos = boundingRect().width()/2;
    setPos(_data->x() - fixPos, _data->y() - fixPos);
}

void DataItem::updateSize(){
    if (_data->width() == _width) return;
    resetTransform();
    _width = _data->width();
    setScale(_data->width());
}

void DataItem::updateRenderer(){
    _iconPackage = _data->iconPackage();
    if( _renders.count(_iconPackage) == 0){
        QSvgRenderer *z = new QSvgRenderer(_data->iconPackage());
        _renders.insert(_iconPackage, z);
        setSharedRenderer(z);
    }else{
        setSharedRenderer( _renders.value(_iconPackage) );
    }
}

void DataItem::updateIcon(){
    if ( elementId().isEmpty() ||  elementId() != _data->icon() ){
        setElementId(_data->icon());
        setTransformOriginPoint(boundingRect().width()/2, boundingRect().width()/2);
    }
}

void DataItem::updateColor(){
    QColor c(_data->color().value<QColor>());
    if (!_data->useColor()){
        delete _colorizer;
        setGraphicsEffect(0);
        _colorizer = 0;
        return;
    }
   
    delete _colorizer;
    _colorizer = new QGraphicsColorizeEffect();
    _colorizer->setColor( c );

    setGraphicsEffect(_colorizer);
    if (_name && _name->isVisible()){
        _name->update();
    }
    if( _value && _value->isVisible()){
        _value->update();
    }
}

void DataItem::updateName(){
    if ( !_name ){
        _name = new QGraphicsSimpleTextItem(i18n("%1").arg(_data->name()), this);
        _name->setFlags(ItemIgnoresTransformations);
        _name->setFont(_font);
    }else if (_name->text() != _data->name()){
        _name->setText(i18n("%1").arg(_data->name()));
    }

    switch(GraphicsLayout::self()->viewStyleDataNode()) {
        case ConfigureDefaultProperties::ABOVE: {
            if (_data->dataStructure()->dataValueVisibility()) {
                _name->setPos(0, -100);
            } else {
                _name->setPos(0, -55);
            }
            break;
        }
        case ConfigureDefaultProperties::CENTER: {
            if (_data->dataStructure()->dataValueVisibility()) {
                _name->setPos(0, -20);
            } else {
                _name->setPos(0, 0);
            }
            break;
        }
        case ConfigureDefaultProperties::BELOW: {
            _name->setPos(0, 70);
            break;
        }
        default:
            _name->setPos(0, 0);
    }
    _name->setVisible(_data->showName());
}

void DataItem::updateValue(){
    if ( !_value ){ 
        _value = new QGraphicsSimpleTextItem(i18n("v=%1").arg(_data->value().toString()), this);
        _value->setFlags(ItemIgnoresTransformations);
        _value->setFont(_font);
    } else if (QVariant(_value->text()) != _data->value().toString()){
        _value ->setText(i18n("v=%1").arg(_data->value().toString()));
    }
    
    switch(GraphicsLayout::self()->viewStyleDataNode()) {
        case ConfigureDefaultProperties::ABOVE: {
            _value->setPos(0, -55);
            break;
        }
        case ConfigureDefaultProperties::CENTER: {
            if (_data->dataStructure()->dataNameVisibility()) {
                _value->setPos(0, 30);
            } else {
                _value->setPos(0, 0);
            }
            break;
        }
        case ConfigureDefaultProperties::BELOW: {
            if (_data->dataStructure()->dataNameVisibility()) {
                _value->setPos(0, 115);
            } else {
                _value->setPos(0, 70);
            }   
            break;
        }
        default:
            _value->setPos(0, 25);
    }
   
    _value->setVisible(_data->showValue());
}
