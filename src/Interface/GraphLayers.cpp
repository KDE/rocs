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

GraphLayers::GraphLayers(MainWindow *parent) : QWidget(parent) {
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

void GraphLayers::populate() {
    kDebug() << "Removing old items";
    for ( int i = 1; i < layout()->count(); ++i) {
        layout()->itemAt(i)->widget()->deleteLater();
    }
    foreach(QAbstractButton *b, _buttonGroup->buttons()) {
        _buttonGroup->removeButton(b);
    }

    kDebug() << "Starting to populate";
    GraphDocument *gd = _mainWindow->activeDocument();
    connect(gd, SIGNAL(graphCreated(Graph*)), this, SLOT(addGraph(Graph*)));

    int total = gd->count();
    for (int i = 0; i < total; ++i) {
	  addGraph(gd->at(i));

    }
}

void GraphLayers::btnADDClicked() {
    QString name = _lineEdit->text();
    if (name == QString()) {
        name = "Untitled" + QString("%1").arg(_mainWindow->activeDocument()->count());
    }
    _mainWindow->activeDocument()->addGraph(name);
}

void GraphLayers::addGraph(Graph *g){
    GraphPropertiesWidget *gp = new GraphPropertiesWidget(g,_mainWindow);
    _buttonGroup->addButton(gp->radio());
    connect(gp, SIGNAL(updateNeeded()), this, SLOT(selectGraph()));
    qobject_cast<QVBoxLayout*>(layout())->insertWidget(1,gp);
}

void GraphLayers::selectGraph(){
  kDebug() << "Chamando Corretamente";
  QList<QAbstractButton *> btns = _buttonGroup->buttons();
  int s = btns.size();
  btns[0]->click();
}
