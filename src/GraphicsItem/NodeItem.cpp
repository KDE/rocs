#include "NodeItem.h"
#include <kstandarddirs.h>
#include <QDir>
#include <KGlobal>
#include <KDebug>
#include <QGraphicsColorizeEffect>
#include <QFont>
#include <QGraphicsScene>
#include <KLocale>
#include "Data.h"

QMap<QString, QSvgRenderer*> DatumItem::_renders;    

DatumItem::DatumItem(Datum* n) : QGraphicsSvgItem(0){
    _datum = n;
    _name = 0;
    _value = 0;
    _originalWidth = _datum->width();
    _iconPackage = _datum->iconPackage();
    _colorizer = new QGraphicsColorizeEffect(this);
    _font = QFont("Helvetica [Cronyx]", 18);
    
    connect(n, SIGNAL(changed()), this, SLOT(setupNode()));
    connect(n, SIGNAL(removed()), this, SLOT(deleteLater()));
    
    setupNode();
    setZValue(1);
    setFlag(ItemIsSelectable, true);
    
    kDebug() << "Visual Node Item Created";
}

void DatumItem::setupNode(){
    updateRenderer();
    updateIcon();
    updateName();
    updateValue();
    updateColor();
    updateSize();
    updatePos();
    update();
}

void DatumItem::updatePos(){
   int fixPos = boundingRect().width()/2;
   setPos(_datum->x() - fixPos, _datum->y() - fixPos);
}

void DatumItem::updateSize(){
  if (_datum->width() == _width) return;
  resetMatrix();
  _width = _datum->width();
  setScale(_datum->width());
   kDebug() << "Scale" << scale() << "Node Width" << _datum->width() << "Shét" <<  _datum->width() - 0.5;
}

void DatumItem::updateRenderer(){
  _iconPackage = _datum->iconPackage();
  if( _renders.count(_iconPackage) == 0){
        QSvgRenderer *z = new QSvgRenderer(_datum->iconPackage());
        _renders.insert(_iconPackage, z);
        setSharedRenderer(z);
  }else{
      setSharedRenderer( _renders.value(_iconPackage) );
  }
}

void DatumItem::updateIcon(){
   if ( elementId().isEmpty() ){
      _element = _datum->icon();
      setElementId(_element);
      setTransformOriginPoint(boundingRect().width()/2, boundingRect().width()/2);
   }else if( elementId() != _datum->icon()){
      _element = _datum->icon();
      setElementId(_element);
      setTransformOriginPoint(boundingRect().width()/2, boundingRect().width()/2);
   }
}

void DatumItem::updateColor(){
   _colorizer->setColor( _datum->color());
   setGraphicsEffect(_colorizer);
    if (_name && _name->isVisible()){
     _name->update();
   }
   if( _value && _value->isVisible()){
     _value->update();
   }
}

void DatumItem::updateName(){
   if ( !_name ){
    _name = new QGraphicsSimpleTextItem(i18n("Name: %1").arg(_datum->name()), this);
    _name->setFont(_font);
   }else if (_name->text() != _datum->name()){
    _name->setText(i18n("Name: %1").arg(_datum->name()));
   }
   _name->setVisible(_datum->showName());
   _name->setPos(0, 75);
   
}

void DatumItem::updateValue(){
   if ( !_value ){ 
      _value = new QGraphicsSimpleTextItem(i18n("Value: %1").arg(_datum->value().toString()), this);
      _value->setFont(_font);
   }else if (_value->text() != _datum->value().toString()){
      _value ->setText(i18n("Value: %1").arg(_datum->value().toString()));
   }
   _value->setVisible(_datum->showValue());
   _value->setPos(0, 100);
}