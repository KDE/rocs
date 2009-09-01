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
  KPushButton *btnADD = new KPushButton(KIcon("AddGraph"), "Add");
  _lineEdit = new KLineEdit(this);
  connect(btnADD, SIGNAL(clicked()), this, SLOT(btnADDClicked()));
  QHBoxLayout *lineLayout = new QHBoxLayout(this); 
  lineLayout->addWidget(btnADD);
  lineLayout->addWidget(_lineEdit);
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addLayout(lineLayout);
  setLayout(layout);
}

void GraphLayers::btnADDClicked(){
  Graph *g = _mainWindow->activeDocument()->addGraph(_lineEdit->text());
  GraphPropertiesWidget *_new = new GraphPropertiesWidget(g,_mainWindow);
  _list.append(_new);
  layout()->addWidget(_new);
  kDebug() << "New widget created! yey!";
}