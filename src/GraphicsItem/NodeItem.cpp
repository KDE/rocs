#include "NodeItem.h"
#include <kstandarddirs.h>
#include <QDir>
#include <KGlobal>
#include <KDebug>
#include <QGraphicsColorizeEffect>

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
    kDebug() << "Node Item Created";

}

NodeItem::~NodeItem(){
}

void NodeItem::setupNode(){
    updateRenderer();
    updateIcon();
    updateName();
    updateValue();
    updateColor();
    updateSize();
    updatePos();
}

void NodeItem::updatePos(){
   // setting the positions
   int fixPos = - boundingRect().width()/2;
   setPos(_node->x() + fixPos, _node->y() + fixPos);
   kDebug() << "setting position to" << _node->x() << _node->y();
}

void NodeItem::updateSize(){
  if (_node->width() == _width) return;
  resetMatrix();
  _width = _node->width();
  scale(_node->width(),_node->width());
  kDebug() << "Scaling in a factor of" << _node->width();
}

void NodeItem::updateRenderer(){
   if ( ! _renderer ){
      _renderer = new QSvgRenderer(_node->iconPackage());
      setSharedRenderer(_renderer);
      kDebug() << "New render created";
   }else if (_iconPackage != _node->iconPackage()){
     _renderer -> load(_node->iconPackage());
     setSharedRenderer(_renderer);
     kDebug() << "Using existing renderer";
   }
}

void NodeItem::updateIcon(){
   //creating or refreshing the icon
   if ( elementId().isEmpty() ){
      _element = _node->icon();
      setElementId(_element);
      kDebug() << "Creating initial icon";
   }else if( elementId() != _node->icon()){
      _element = _node->icon();
      setElementId(_element);
      kDebug() << "Updating existing icon";
   }
}

void NodeItem::updateColor(){
   // coloring
   _colorizer->setColor( _node->color());
   setGraphicsEffect(_colorizer);
   kDebug() << "setting the color to " << _node->color();
}

void NodeItem::updateName(){
   // setting the name
   if ( !_name ){
    _name = new QGraphicsTextItem(_node->name(), this);
    kDebug() << "Setting the name to" << _node->name();
   }else if (_name->toPlainText() != _node->name()){
    _name->setPlainText(_node->name());
    kDebug() << "Updating the name to" << _node->name();
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
    _value = new QGraphicsTextItem(_node->value().toString(), this);
    kDebug() << "Setting the value to" << _node->value().toString();
   }else if (_value->toPlainText() != _node->value().toString()){
    _value ->setPlainText(_node->value().toString());
    kDebug() << "updating the value to" << _node -> value().toString();
   }
   if (! _node->showValue()){
    _value->hide();
   }else{
    _value->show();
   }
   _value->setPos(100, 100);
}