#include "GraphLayers.h"
#include "GraphPropertiesWidget.h"
#include "Document.h"
#include "DataStructure.h"
#include "MainWindow.h"
#include <KPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <KIcon>
#include <QHBoxLayout>
#include <KDebug>
#include <KLineEdit>
#include <QWidget>
#include <QSizePolicy>
#include "DocumentManager.h"
#include <qscrollbar.h>

GraphLayers::GraphLayers(MainWindow *parent) : QScrollArea(parent) {
    _mainWindow = parent;
    
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    QVBoxLayout *vBoxLayout = new QVBoxLayout();
    
    QWidget *contents = new QWidget();
    KPushButton *btnADD = new KPushButton(KIcon("AddGraph"), i18n("Add"));
    
    _buttonGroup = new QButtonGroup();
    _lineEdit = new KLineEdit();
    
    connect(btnADD, SIGNAL(clicked()), this, SLOT(btnADDClicked()));
    hBoxLayout->addWidget(btnADD);
    hBoxLayout->addWidget(_lineEdit);
    
    vBoxLayout->addLayout(hBoxLayout);
    vBoxLayout->addStretch();
    vBoxLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    contents->setLayout(vBoxLayout);
    
    hBoxLayout->setSpacing(0);
    hBoxLayout->setContentsMargins(0,0,0,0);
    vBoxLayout->setSpacing(0);
    vBoxLayout->setContentsMargins(0,0,0,0);
    setWidget(contents);
    setFrameShape(NoFrame);
}

void GraphLayers::resizeEvent(QResizeEvent* event)
{
    QScrollArea::resizeEvent(event);
    widget()->setFixedWidth(contentsRect().width());
}

void GraphLayers::populate() {
    kDebug() << "Starting to populate the layers";
    for ( int i = 1; i < widget()->layout()->count(); ++i) {
        widget()->layout()->itemAt(i)->widget()->deleteLater();
    }
    foreach(QAbstractButton *b, _buttonGroup->buttons()) {
        _buttonGroup->removeButton(b);
    }

    Document *gd = DocumentManager::self()->activeDocument();
    connect(gd, SIGNAL(dataStructureCreated(DataStructure*)), 
            this, SLOT(addGraph(DataStructure*)),Qt::UniqueConnection);
    
    connect(this, SIGNAL(createGraph(QString)), 
            gd, SLOT(addDataStructure(QString)), Qt::UniqueConnection);

    foreach(DataStructure *s, gd->dataStructures()){
	  addGraph(s);
    }
    kDebug() << "Finished Populating";
}

void GraphLayers::btnADDClicked() {
    QString name = _lineEdit->text();
    if (name.isEmpty()) {
        name = i18n("Untitled%1", DocumentManager::self()->activeDocument()->dataStructures().count());
    }
    emit  createGraph(name);
}

void GraphLayers::addGraph(DataStructure *g){
    GraphPropertiesWidget *gp = new GraphPropertiesWidget(g,_mainWindow);
    _buttonGroup->addButton(gp->radio());
    connect(gp, SIGNAL(updateNeeded()), this, SLOT(selectFirstGraph()));
    qobject_cast<QVBoxLayout*>(widget()->layout())->insertWidget(1,gp);
}

void GraphLayers::selectFirstGraph(){
  _buttonGroup->buttons()[0]->click();
}
