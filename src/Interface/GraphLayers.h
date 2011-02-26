#ifndef GRAPHLAYERS_H
#define GRAPHLAYERS_H

#include <QScrollArea>
#include <QWidget>
class DataStructure;
class QButtonGroup;
class KLineEdit;
class MainWindow;
class Document;

class GraphLayers : public QScrollArea {
    Q_OBJECT
    QButtonGroup *_buttonGroup;
    KLineEdit *_lineEdit;
    MainWindow *_mainWindow;

protected:
    void resizeEvent ( QResizeEvent * event );

public:
    GraphLayers(MainWindow *parent = 0);
    void setActiveDocument();

public slots:
    void btnADDClicked();
    void addGraph(DataStructure *g);
    void selectFirstGraph();

signals:
  void  createGraph(const QString& s);

private:
    Document *_activeDocument;
};

#endif
