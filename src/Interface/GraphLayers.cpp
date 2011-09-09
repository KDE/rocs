/* 
    This file is part of Rocs.
    Copyright 2009-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of 
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

GraphLayers::GraphLayers(MainWindow *parent) : 
    QScrollArea(parent)
    , _mainWindow(parent)
    , _activeDocument(0)
{    
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
    widget()->setFixedWidth(contentsRect().width()); //! do not remove the duplicated line below. 
    widget()->setFixedWidth(contentsRect().width()); //! it looks it just works like this. -.-'
}

void GraphLayers::setActiveDocument() {
    
    if (_activeDocument && _activeDocument != DocumentManager::self()->activeDocument()){
        _activeDocument->disconnect(this);
        disconnect(_activeDocument);
    }

    _activeDocument = DocumentManager::self()->activeDocument();
    for ( int i = 1; i < widget()->layout()->count(); ++i) {
        widget()->layout()->itemAt(i)->widget()->deleteLater();
    }
    foreach(QAbstractButton *b, _buttonGroup->buttons()) {
        _buttonGroup->removeButton(b);
    }

    connect(_activeDocument, SIGNAL(dataStructureCreated(DataStructurePtr)), 
            this, SLOT(addGraph(DataStructurePtr)),Qt::UniqueConnection);
    
    connect(this, SIGNAL(createGraph(QString)), 
            _activeDocument, SLOT(addDataStructure(QString)), Qt::UniqueConnection);

    foreach(DataStructurePtr s, _activeDocument->dataStructures()){
        addGraph(s);
    }
}

void GraphLayers::btnADDClicked() {
    QString name = _lineEdit->text();
    if (name.isEmpty()) {
        name = i18n("Untitled%1", DocumentManager::self()->activeDocument()->dataStructures().count());
    }
    emit  createGraph(name);
}

void GraphLayers::addGraph(DataStructurePtr g){
    GraphPropertiesWidget *gp = new GraphPropertiesWidget(g,_mainWindow);
    _buttonGroup->addButton(gp->radio());
    connect(gp, SIGNAL(updateNeeded()), this, SLOT(selectFirstGraph()));
    qobject_cast<QVBoxLayout*>(widget()->layout())->insertWidget(1,gp);
}

void GraphLayers::selectFirstGraph(){
  _buttonGroup->buttons()[0]->click();
}
