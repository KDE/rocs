#ifndef DATAITEM_H
#define DATAITEM_H

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

#include "rocslib_export.h"

class Data;

class ROCSLIB_EXPORT DataItem : public QGraphicsSvgItem{
  Q_OBJECT

  public:
    DataItem(Data *n);
    virtual ~DataItem();
    Data* data() const { return _datum; }
    static QMap<QString, QSvgRenderer*> _renders;
    
  private slots:
    void setupNode();
  
private:
    void updateRenderer();
    void updateIcon();
    void updateName();
    void updateValue();
    void updateColor();
    void updatePos();
    void updateSize();

    Data *_datum;
    QString _iconPackage;
    QGraphicsSimpleTextItem *_name;
    QGraphicsSimpleTextItem *_value;
    QGraphicsColorizeEffect *_colorizer;
    QFont _font;

    qreal _originalWidth;
    qreal _width;
};

#endif
