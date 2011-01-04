#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsSvgItem>
#include <QStateMachine>
#include <QState>
#include <QPropertyAnimation>
#include <QSvgRenderer>
#include <QGraphicsTextItem>
#include <QGraphicsColorizeEffect>
#include <QMap>
#include <QString>
#include <QFont>

class Data;

class DataItem : public QGraphicsSvgItem{
  Q_OBJECT

  public:
    DataItem(Data *n);

    Data* datum() const { return _datum; }
    static QMap<QString, QSvgRenderer*> _renders;
    
  private:
    void setupNode();
    void updateRenderer();
    void updateIcon();
    void updateName();
    void updateValue();
    void updateColor();
    void updatePos();
    void updateSize();
  

private:
    Data *_datum;
    QString _iconPackage;
    QString _element;
    QGraphicsSimpleTextItem *_name;
    QGraphicsSimpleTextItem *_value;
    QGraphicsColorizeEffect *_colorizer;
    QFont _font;

    qreal _originalWidth;
    qreal _width;
    
};

#endif
