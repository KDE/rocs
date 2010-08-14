#include "GraphLayers.h"
#include "GraphPropertiesWidget.h"
#include "graphDocument.h"
#include "DataStructureBase.h"
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

    KPushButton *btnADD = new KPushButton(KIcon("AddGraph"), i18n("Add"));
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
    for ( int i = 1; i < layout()->count(); ++i) {
        layout()->itemAt(i)->widget()->deleteLater();
    }
    foreach(QAbstractButton *b, _buttonGroup->buttons()) {
        _buttonGroup->removeButton(b);
    }

    GraphDocument *gd = _mainWindow->activeDocument();
    connect(gd, SIGNAL(graphCreated(DataStructureBase*)), this, SLOT(addGraph(DataStructureBase*)),Qt::UniqueConnection);

    int total = gd->count();
    for (int i = 0; i < total; ++i) {
	  addGraph(gd->at(i));
    }
}

void GraphLayers::btnADDClicked() {
    QString name = _lineEdit->text();
    if (name == QString()) {
        name = i18n("Untitled%1", _mainWindow->activeDocument()->count());
    }
    emit  createGraph(name);
}

void GraphLayers::addGraph(DataStructureBase *g){
    GraphPropertiesWidget *gp = new GraphPropertiesWidget(g,_mainWindow);
    _buttonGroup->addButton(gp->radio());
    connect(gp, SIGNAL(updateNeeded()), this, SLOT(selectFirstGraph()),Qt::UniqueConnection);
    qobject_cast<QVBoxLayout*>(layout())->insertWidget(1,gp);
}

void GraphLayers::selectFirstGraph(){
  _buttonGroup->buttons()[0]->click();
}
