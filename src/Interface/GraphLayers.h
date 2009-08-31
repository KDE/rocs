#ifndef GRAPHLAYERS_H
#define GRAPHLAYERS_H

#include <QWidget>
#include <QList>
class GraphPropertiesWidget;
class graphDocument;
class graph;
class KPushButton;
class QButtonGroup;
class KLineEdit;

class GraphLayers : public QWidget{

  QButtonGroup *_buttonGroup;
  QList<GraphPropertiesWidget*> _list;
  KLineEdit *_lineEdit;
  
  public:
    GraphLayers(QWidget *parent = 0);
    
  public slots:
    void btnADDClicked();
    
};

#endif