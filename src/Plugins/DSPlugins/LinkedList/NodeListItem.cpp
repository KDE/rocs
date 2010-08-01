#include "NodeListItem.h"
#include <kstandarddirs.h>
#include <QDir>
#include <KGlobal>
#include <KDebug>
#include <QGraphicsColorizeEffect>
#include <QFont>
#include <QGraphicsScene>
#include <KLocale>

QMap<QString, QSvgRenderer*> NodeListItem::_renders;

NodeListItem::NodeListItem(Node* n) : QGraphicsSvgItem(0){
    _node = n;
    _name = 0;
    _value = 0;
    _originalWidth = _node->width();
    _iconPackage = _node->iconPackage();
    _colorizer = new QGraphicsColorizeEffect(this);
    _font = QFont("Helvetica [Cronyx]", 18);

    connect(n, SIGNAL(changed()), this, SLOT(setupNode()));
    connect(n, SIGNAL(removed()), this, SLOT(deleteLater()));

    setupNode();
    setZValue(1);
    setFlag(ItemIsSelectable, true);

    kDebug() << "Visual Node Item Created";
}

void NodeListItem::setupNode(){
    updateRenderer();
    updateIcon();
    updateName();
    updateValue();
    updateColor();
    updateSize();
    updatePos();
    update();
}

void NodeListItem::updatePos(){
   int fixPos = boundingRect().width()/2;
   setPos(_node->x() - fixPos, _node->y() - fixPos);
}

void NodeListItem::updateSize(){
  if (_node->width() == _width) return;
  resetMatrix();
  _width = _node->width();
  setScale(_node->width());
   kDebug() << "Scale" << scale() << "Node Width" << _node->width() << "Shét" <<  _node->width() - 0.5;
}

void NodeListItem::updateRenderer(){
  _iconPackage = _node->iconPackage();
  if( _renders.count(_iconPackage) == 0){
        QSvgRenderer *z = new QSvgRenderer(_node->iconPackage());
        _renders.insert(_iconPackage, z);
        setSharedRenderer(z);
  }else{
      setSharedRenderer( _renders.value(_iconPackage) );
  }
}

void NodeListItem::updateIcon(){
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

void NodeListItem::updateColor(){
   _colorizer->setColor( _node->color());
   setGraphicsEffect(_colorizer);
    if (_name && _name->isVisible()){
     _name->update();
   }
   if( _value && _value->isVisible()){
     _value->update();
   }
}

void NodeListItem::updateName(){
   if ( !_name ){
    _name = new QGraphicsSimpleTextItem(i18n("Name: %1").arg(_node->name()), this);
    _name->setFont(_font);
   }else if (_name->text() != _node->name()){
    _name->setText(i18n("Name: %1").arg(_node->name()));
   }
   _name->setVisible(_node->showName());
   _name->setPos(0, 75);

}

void NodeListItem::updateValue(){
   if ( !_value ){
      _value = new QGraphicsSimpleTextItem(i18n("Value: %1").arg(_node->value().toString()), this);
      _value->setFont(_font);
   }else if (_value->text() != _node->value().toString()){
      _value ->setText(i18n("Value: %1").arg(_node->value().toString()));
   }
   _value->setVisible(_node->showValue());
   _value->setPos(0, 100);
}