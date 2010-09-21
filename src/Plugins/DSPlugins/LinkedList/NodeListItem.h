#ifndef NODELISTITEM_H
#define NODELISTITEM_H

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
#include "Data.h"

class NodeListItem : public QGraphicsSvgItem{
  Q_OBJECT
    qreal _originalWidth;
    qreal _width;

  public:
    NodeListItem(Datum *n);

    Datum* node() const { return _node; }
    static QMap<QString, QSvgRenderer*> _renders;

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
    Datum *_node;
    QString _iconPackage;
    QString _element;
    QGraphicsSimpleTextItem *_name;
    QGraphicsSimpleTextItem *_value;
    QGraphicsColorizeEffect *_colorizer;
    QFont _font;
};

// #include "node.h"

#endif
