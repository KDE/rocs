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
    Data* data() const { return _data; }
    static QMap<QString, QSvgRenderer*> _renders;
    QRectF  boundingRect () const;
    QGraphicsSimpleTextItem *name() const;
    QGraphicsSimpleTextItem *value() const;
  private slots:
    void setupNode();
    void updateRenderer();
    void updateIcon();
    void updateName();
    void updateValue();
    void updateColor();
    void updatePos();
    void updateSize();

private:
    Data *_data;
    QString _iconPackage;
    QGraphicsSimpleTextItem *_name;
    QGraphicsSimpleTextItem *_value;
    QGraphicsColorizeEffect *_colorizer;
    QGraphicsRectItem *_boundingRect;
    QFont _font;
    int _oldStyle;

    qreal _originalWidth;
    qreal _width;
};

#endif
