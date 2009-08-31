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
#include <KLineEdit>

GraphLayers::GraphLayers(QWidget *parent) : QWidget(parent){
 // KAction* action = new KAction(,i18n("New Graph"),  this);
  KPushButton *btnADD = new KPushButton(KIcon("AddGraph"), "Add");
  _lineEdit = new KLineEdit(this);
  connect(btnADD, SIGNAL(clicked()), this, SLOT(btnADDClicked()));
  QHBoxLayout *lineLayout = new QHBoxLayout(this); 
  lineLayout->addWidget(btnADD);
  lineLayout->addWidget(lineEdit);
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addLayout(lineLayout);
  setLayout(layout);
}

void GraphLayers::btnADDClicked(){
  MainWindow *m = qobject_cast<MainWindow*>(parent());

  Graph *g = m->activeDocument()->addGraph(_lineEdit->text());
  GraphPropertiesWidget *_new = new GraphPropertiesWidget(g,m);
  _list.append(_new);
  layout()->addWidget(_new);
}