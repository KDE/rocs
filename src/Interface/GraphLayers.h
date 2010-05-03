#ifndef GRAPHLAYERS_H
#define GRAPHLAYERS_H

#include <QWidget>
class Graph;
class QButtonGroup;
class KLineEdit;
class MainWindow;

class GraphLayers : public QWidget {
    Q_OBJECT
    QButtonGroup *_buttonGroup;
    KLineEdit *_lineEdit;
    MainWindow *_mainWindow;
public:
    GraphLayers(MainWindow *parent = 0);
    void populate();
public slots:
    void btnADDClicked();
    void addGraph(Graph *g);
    void selectFirstGraph();
signals:
  void  createGraph(const QString& s);
  
};

#endif
