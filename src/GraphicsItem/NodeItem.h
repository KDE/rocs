#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsSvgItem>
#include <QStateMachine>
#include <QState>
#include <QPropertyAnimation>
#include <QSvgRenderer>
#include <QGraphicsTextItem>
#include <QGraphicsColorizeEffect>

#include "node.h"

class NodeItem : public QGraphicsSvgItem{
  Q_OBJECT
    qreal _originalWidth;
    qreal _width;

  public:
    NodeItem(Node *n);
    ~NodeItem();
    Node* node() const { return _node; }
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
    Node *_node;
    QSvgRenderer *_renderer;
    
    QString _iconPackage;
    QString _element;
    QGraphicsSimpleTextItem *_name;
    QGraphicsSimpleTextItem *_value;
    QGraphicsColorizeEffect *_colorizer;
};

#include "node.h"

#endif