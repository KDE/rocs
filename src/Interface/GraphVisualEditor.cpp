/* This file is part of Rocs,
	 Copyright (C) 2008 by:
	 Tomaz Canabrava <tomaz.canabrava@gmail.com>
	 Ugo Sangiori <ugorox@gmail.com>

	 Rocs is free software; you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation; either version 2 of the License, or
	 (at your option) any later version.

	 Rocs is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with Step; if not, write to the Free Software
	 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA
*/

#include "GraphVisualEditor.h"
#include "GraphScene.h"
#include "MainWindow.h"
#include "DataItem.h"
#include "PointerItem.h"

#include "Document.h"
#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"

#include "AlignAction.h"
#include "settings.h"

#include <QVBoxLayout>
#include <QGraphicsItem>
#include <KDebug>
#include <QGraphicsView>
#include <QPainter>
#include <DocumentManager.h>

GraphVisualEditor::GraphVisualEditor(MainWindow *parent)
        : QWidget(parent),
        _topNode(0),
        _bottomNode(0),
        _leftNode(0),
        _rightNode(0) {
    _scene = 0;
    _document = 0;
    _dataStructure = 0;
    _mainWindow = parent;
    setupWidgets();
}

void GraphVisualEditor::setupWidgets() {
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(0,0,0,0);
    _scene = new GraphScene(this);
    _graphicsView = new QGraphicsView();
    _graphicsView->setRenderHints(QPainter::Antialiasing);
    _graphicsView->setScene(_scene);
    vLayout -> addWidget ( _graphicsView );
    setLayout( vLayout );
}

QGraphicsView* GraphVisualEditor::view() const {
    return _graphicsView;
}

void GraphVisualEditor::setActiveDocument( ) {
    if ( _document !=  DocumentManager::self()->activeDocument()) {
        releaseDocument();
    }
    _document = DocumentManager::self()->activeDocument();
    _scene->setActiveDocument();
    
    connect(_document , SIGNAL(activeDataStructureChanged(DataStructure*)), 
            this ,       SLOT  (setActiveGraph(DataStructure*)));
}

void GraphVisualEditor::releaseDocument() {
    if (!_document){
        return;
    }
    
    _scene->clear();
    foreach(DataStructure *ds, _document->dataStructures()){
       ds->disconnect(this);
    }
}

//void GraphVisualEditor::drawGraphOnScene( DataStructure */*g*/) {}

void GraphVisualEditor::setActiveGraph( DataStructure *g) {
    _dataStructure = g;
    _scene->setActiveGraph(g);
}

GraphScene* GraphVisualEditor::scene() const {
    return _scene;
}

QList<DataItem*> GraphVisualEditor::selectedNodes() const {
    QList<DataItem*> tmpList;
    QList<QGraphicsItem*> l = _scene->selectedItems();
    foreach(QGraphicsItem *i, l) {
        if ( qgraphicsitem_cast<DataItem*>(i) ) {
            tmpList.append( qgraphicsitem_cast<DataItem*>(i) );
        }
    }
    return tmpList;
}
