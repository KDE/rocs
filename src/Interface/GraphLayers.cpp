#include "GraphLayers.h"
#include "GraphPropertiesWidget.h"
#include "graphDocument.h"
#include "graph.h"
#include "MainWindow.h"
#include <KPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <KIcon>
#include <QHBoxLayout>
#include <KDebug>
#include <KLineEdit>

GraphLayers::GraphLayers(MainWindow *parent) : QWidget(parent){
 _mainWindow = parent;
 QHBoxLayout *hBoxLayout = new QHBoxLayout();
 
 KPushButton *btnADD = new KPushButton(KIcon("AddGraph"), "Add");
 _lineEdit = new KLineEdit(this);
 connect(btnADD, SIGNAL(clicked()), this, SLOT(btnADDClicked()));
  hBoxLayout->addWidget(btnADD);
  hBoxLayout->addWidget(_lineEdit);
  QVBoxLayout *l = new QVBoxLayout();
  l->addLayout(hBoxLayout);
  l->addStretch();
  setLayout(l);
  _buttonGroup = new QButtonGroup();
}

void GraphLayers::populate(){
  kDebug() << "Removing old items";
  for( int i = 1; i < layout()->count(); ++i){
    layout()->itemAt(i)->widget()->deleteLater();  
  }
  foreach(QAbstractButton *b, _buttonGroup->buttons()){
    _buttonGroup->removeButton(b);
  }
  
  kDebug() << "Starting to populate";
  GraphDocument *gd = _mainWindow->activeDocument();
  int total = gd->count();
  for(int i = 0; i < total; ++i){
    Graph *g = gd->at(i);
    GraphPropertiesWidget *gp = new GraphPropertiesWidget(g, _mainWindow);
    _buttonGroup->addButton(gp->radio());
    qobject_cast<QVBoxLayout*>(layout())->insertWidget(1,gp);
  }
}

void GraphLayers::btnADDClicked(){
  QString name = _lineEdit->text();
  if (name == QString()){
    name = "Untitled" + QString("%1").arg(_mainWindow->activeDocument()->count());
  
  }
  Graph *g = _mainWindow->activeDocument()->addGraph(name);
  GraphPropertiesWidget *gp = new GraphPropertiesWidget(g,_mainWindow);
  _buttonGroup->addButton(gp->radio());
  qobject_cast<QVBoxLayout*>(layout())->insertWidget(1,gp);
}
