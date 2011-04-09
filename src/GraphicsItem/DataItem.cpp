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

QMap<QString, QSvgRenderer*> DataItem::_renders;    

DataItem::DataItem(Data* n) 
: QGraphicsSvgItem(0)
,_datum(n)
,_iconPackage(n->iconPackage())
,_name(0)
,_value(0)
,_colorizer(0)
,_font(QFont("Helvetica [Cronyx]", 18))
,_originalWidth(n->width())
{
    connect(n, SIGNAL(removed()), this, SLOT(deleteLater()));
    connect(n, SIGNAL(changed()), this, SLOT(setupNode()));
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
   setPos(_datum->x() - fixPos, _datum->y() - fixPos);
}

void DataItem::updateSize(){
  if (_datum->width() == _width) return;
  resetMatrix();
  _width = _datum->width();
  setScale(_datum->width());
}

void DataItem::updateRenderer(){
  _iconPackage = _datum->iconPackage();
  if( _renders.count(_iconPackage) == 0){
        QSvgRenderer *z = new QSvgRenderer(_datum->iconPackage());
        _renders.insert(_iconPackage, z);
        setSharedRenderer(z);
  }else{
      setSharedRenderer( _renders.value(_iconPackage) );
  }
}

void DataItem::updateIcon(){
   if ( elementId().isEmpty() ||  elementId() != _datum->icon() ){
      setElementId(_datum->icon());
      setTransformOriginPoint(boundingRect().width()/2, boundingRect().width()/2);
   }
}

void DataItem::updateColor(){
   QColor c(_datum->color().value<QColor>());
   if (!_datum->useColor()){
       delete _colorizer;
       setGraphicsEffect(0);
       _colorizer = 0;
       return;
   }
   
   qDebug() << "color on the colorizer" << c;
    delete _colorizer;
    _colorizer = new QGraphicsColorizeEffect();
    _colorizer->setColor( c );

   qDebug() << "Meh";
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
    _name = new QGraphicsSimpleTextItem(i18n("Name: %1").arg(_datum->name()), this);
    _name->setFont(_font);
   }else if (_name->text() != _datum->name()){
    _name->setText(i18n("Name: %1").arg(_datum->name()));
   }
   _name->setVisible(_datum->showName());
   _name->setPos(0, 75);
   
}

void DataItem::updateValue(){
   if ( !_value ){ 
      _value = new QGraphicsSimpleTextItem(i18n("Value: %1").arg(_datum->value().toString()), this);
      _value->setFont(_font);
   }else if (_value->text() != _datum->value().toString()){
      _value ->setText(i18n("Value: %1").arg(_datum->value().toString()));
   }
   _value->setVisible(_datum->showValue());
   _value->setPos(0, 100);
}
