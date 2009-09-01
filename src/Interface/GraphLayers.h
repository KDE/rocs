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
class MainWindow;

class GraphLayers : public QWidget{
  Q_OBJECT
  QButtonGroup *_buttonGroup;
  QList<GraphPropertiesWidget*> _list;
  KLineEdit *_lineEdit;
  MainWindow *_mainWindow;
  public:
    GraphLayers(MainWindow *parent = 0);
    
  public slots:
    void btnADDClicked();
    
};

#endif