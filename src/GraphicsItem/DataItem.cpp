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
,_oldStyle(GraphicsLayout::self()->viewStyleDataNode())
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
    setZValue(1);
    setFlag(ItemIsSelectable, true);
    setupNode();
}

DataItem::~DataItem()
{
    delete _name;
    delete _value;
  //  delete _colorizer;
}

void DataItem::setupNode(){
    updateName();
    updateValue();  
    updateRenderer();
    updateIcon();
    updateColor();
    updateSize(); 
    updatePos(); 
    update();
}

void DataItem::updatePos(){
    int fixPos = boundingRect().width()/2;
    setPos(_data->x() - fixPos, _data->y() - fixPos);
    updateName();
    updateValue();
}

void DataItem::updateSize(){
    if (_data->width() == _width) return;
    resetTransform();
    _width = _data->width();
    setScale(_data->width());
    updateName();
    updateValue();
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
        _name->setBrush(QBrush(Qt::black));
        _value->setBrush(QBrush(Qt::black));
        _colorizer = 0;
        return;
    }
    
   delete _colorizer;
    _colorizer = new QGraphicsColorizeEffect();
    _colorizer->setColor( c );
    setGraphicsEffect(_colorizer);
    _name->setBrush(QBrush(c));
    _value->setBrush(QBrush(c));
}
    
void DataItem::updateName(){
    if ( !_name ){
        _name = new QGraphicsSimpleTextItem(i18n("%1").arg(_data->name()));
        _name->setFlags(ItemIgnoresTransformations);
        _name->setFont(_font);
        _name->setZValue(zValue()+1);
    }else if (_name->text() != _data->name()){
        _name->setText(i18n("%1").arg(_data->name()));
    }
   
    int style = GraphicsLayout::self()->viewStyleDataNode();
    
    qreal dataWidth = boundingRect().width() * scale();
    
    qreal x =  pos().x();
    if ( style == ConfigureDefaultProperties::CENTER ){
        x  += ((dataWidth > _name->boundingRect().width()+10)  
            ? ((dataWidth - _name->boundingRect().width())/4)
            : dataWidth + 30);
    }
    
    qreal y =  pos().y() + (( style == ConfigureDefaultProperties::ABOVE ) ? - (dataWidth/2) 
                          : ( style == ConfigureDefaultProperties::BELOW ) ? 50 + (dataWidth/2) 
                          : 25 );

    _name->setVisible(_data->showName());
    
    if (_value && _value->isVisible()){
        y +=  (( style == ConfigureDefaultProperties::ABOVE ) ? -20 :  20 );
    }
    _name->setPos(x,y);    
}

QGraphicsSimpleTextItem* DataItem::name() const
{
    return _name;
}

QGraphicsSimpleTextItem* DataItem::value() const
{
    return _value;
}

void DataItem::updateValue(){
    if ( !_value ){ 
        _value = new QGraphicsSimpleTextItem(i18n("v=%1").arg(_data->value().toString()));
        _value->setFlags(ItemIgnoresTransformations);
        _value->setFont(_font);
        _value->setZValue(zValue()+2);
    } else if (QVariant(_value->text()) != _data->value().toString()){
        _value ->setText(i18n("v=%1").arg(_data->value().toString()));
    }
    
   int style = GraphicsLayout::self()->viewStyleDataNode();
    
    qreal dataWidth = boundingRect().width() * scale();
    
    qreal x =  pos().x();
    if ( style == ConfigureDefaultProperties::CENTER ){
        x  += ((dataWidth > _value->boundingRect().width()+10)  
            ? ((dataWidth - _value->boundingRect().width())/4)
            : dataWidth + 30);
    }
    
    qreal y =  pos().y() + (( style == ConfigureDefaultProperties::ABOVE ) ? - (dataWidth/2) 
                          : ( style == ConfigureDefaultProperties::BELOW ) ? 50 + (dataWidth/2) 
                          : 25 );

    _value->setPos(x,y);
    _value->setVisible(_data->showValue());
}
