#include "NodeItem.h"
#include <kstandarddirs.h>
#include <QDir>
#include <KGlobal>
#include <KDebug>
#include <QGraphicsColorizeEffect>
#include <QFont>
#include <QGraphicsScene>

NodeItem::NodeItem(Node* n) : QGraphicsSvgItem(0){
    _node = n;
    _renderer = 0;
    _iconPackage = _node->iconPackage();
    _name = 0;
    _value = 0;
    _originalWidth = _node->width();
    _colorizer = new QGraphicsColorizeEffect(this);
    connect(n, SIGNAL(changed()), this, SLOT(setupNode()));
    connect(n, SIGNAL(removed()), this, SLOT(deleteLater()));
    setupNode();
    setZValue(1);
    setFlag(ItemIsSelectable, true);
    kDebug() << "Node Item Created";
}

void NodeItem::setupNode(){
    
    updateRenderer();
    updateIcon();
    updateName();
    updateValue();
    updateColor();
    updateSize();
    updatePos();
    update();
}

void NodeItem::updatePos(){
   // setting the positions
   
   int fixPos = boundingRect().width()/2;
   setPos(_node->x() - fixPos, _node->y() - fixPos);
}

void NodeItem::updateSize(){
  if (_node->width() == _width) return;
  resetMatrix();
  _width = _node->width();
  setScale(_node->width());
   kDebug() << "Scale" << scale() << "Node Width" << _node->width() << "Shét" <<  _node->width() - 0.5;
}

void NodeItem::updateRenderer(){
   if ( ! _renderer ){
      _renderer = new QSvgRenderer(_node->iconPackage());
      setSharedRenderer(_renderer);
   }else if (_iconPackage != _node->iconPackage()){
     _renderer -> load(_node->iconPackage());
     setSharedRenderer(_renderer);
   }
}

void NodeItem::updateIcon(){
   //creating or refreshing the icon
   if ( elementId().isEmpty() ){
      _element = _node->icon();
      setElementId(_element);
      setTransformOriginPoint(boundingRect().width()/2, boundingRect().width()/2);
   }else if( elementId() != _node->icon()){
      _element = _node->icon();
      setElementId(_element);
      setTransformOriginPoint(boundingRect().width()/2, boundingRect().width()/2);
   }
}

void NodeItem::updateColor(){
   _colorizer->setColor( _node->color());
   setGraphicsEffect(_colorizer);
}

void NodeItem::updateName(){
   if ( !_name ){
    _name = new QGraphicsSimpleTextItem(_node->name(), this);
    _name->setFont(QFont("Helvetica [Cronyx]", 18));
   }else if (_name->text() != _node->name()){
    _name->setText(_node->name());
   }
   if ( ! _node->showName() ){
    _name->hide();
   }else{
    _name->show();
   }
   _name->setPos(boundingRect().width() - _name->boundingRect().width(), 75);
}

void NodeItem::updateValue(){
   // setting the value
   if ( !_value ){ 
      _value = new QGraphicsSimpleTextItem(_node->value().toString(), this);
      _value->setFont(QFont("Helvetica [Cronyx]", 18));
   }else if (_value->text() != _node->value().toString()){
      _value ->setText(_node->value().toString());
   }
   if (! _node->showValue()){
      _value->hide();
   }else{
      _value->show();
   }
   _value->setPos(20, 20);
}