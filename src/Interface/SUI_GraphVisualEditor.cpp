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

#include "SUI_GraphVisualEditor.h"
#include "SUI_GraphToolBoxWidget.h"
#include "SUI_GraphScene.h"
#include "SUI_MainWindow.h"
#include "NodeItem.h"
#include "EdgeItem.h"
#include "OrientedEdgeItem.h"

#include "graphDocument.h"
#include "graph.h"
#include "node.h"
#include "edge.h"

#include "AlignAction.h"
#include "settings.h"

#include <QVBoxLayout>
#include <QPointF>
#include <QGraphicsItem>
#include <KDebug>
#include <QToolButton>
#include <KAction>
#include <KIcon>
#include <KComboBox>
#include <KColorButton>
#include <QGraphicsView>
#include <QPainter>

GraphVisualEditor::GraphVisualEditor(MainWindow *parent)
        : QWidget(parent),
        _topNode(0),
        _bottomNode(0),
        _leftNode(0),
        _rightNode(0){
   _scene = 0;
    _graphDocument = 0;
    _graph = 0;
    _mainWindow = parent;
    setupWidgets();
}

void GraphVisualEditor::setupWidgets() {
    QHBoxLayout *layout	= new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    //!################## CODE TO GENERATE THE ALIGN TOOLBOX ##########################

    _spacingPropContainer = new QWidget(parentWidget());
    layout->addWidget(setupToolButton("align-h-bottom", "Align nodes on the base", AlignAction::Bottom,this ));
    layout->addWidget(setupToolButton("align-h-middle", "Align nodes horizontally on the middle", AlignAction::MiddleHorizontal,this));
    layout->addWidget(setupToolButton("align-h-top", "Align nodes on the top",AlignAction::Top, this));
    layout->addWidget(setupToolButton("align-v-left", "Align nodes on the left", AlignAction::Left, this ));
    layout->addWidget(setupToolButton("align-v-middle", "Align nodes vertically on the middle", AlignAction::MiddleVertical,this));
    layout->addWidget(setupToolButton("align-v-right", "Align nodes on the right", AlignAction::Right,this));
    _spacingPropContainer->setLayout(layout);

    //!############################### CODE TO GENERATE THE GRAPH PROPERTIES TOOLBOX ####################################
    _graphToolBox = new GraphToolBoxWidget(_mainWindow);

    //!############################### finishes the Toolbar.
    layout = new QHBoxLayout();
    _toolbar = new QWidget(parentWidget());
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget( _graphToolBox );
    layout->addWidget( _spacingPropContainer );
    layout->addStretch();

    _toolbar->setLayout(layout);

    //!############################## Adds the GraphView on the screen.
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(0,0,0,0);
    _scene = new GraphScene(this);
    _graphicsView = new QGraphicsView();
    _graphicsView->setRenderHints(QPainter::Antialiasing);
    _graphicsView->setScene(_scene);
    vLayout -> addWidget( _toolbar );
    vLayout -> addWidget ( _graphicsView );
    setLayout( vLayout );
}

QGraphicsView* GraphVisualEditor::view() const {
    return _graphicsView;
}

QToolButton* GraphVisualEditor::setupToolButton(const QString& actionName, const QString& tooltip, AlignAction::Orientation o, QWidget *parent) {
    AlignAction*  tmpAction = new AlignAction(actionName, tooltip, o, parent);
    QToolButton*  tmpButton = new QToolButton(_spacingPropContainer);
    tmpButton -> setDefaultAction ( tmpAction );
    tmpButton -> setAutoRaise( true );
    return tmpButton;
}

void GraphVisualEditor::setActiveGraphDocument( GraphDocument *gd) {
    if ( _graphDocument != 0 ) { releaseGraphDocument(); }

    _graphDocument = gd;
    _graphToolBox->setActiveGraphDocument( gd );
    _scene->setActiveGraphDocument( gd );
}

void GraphVisualEditor::releaseGraphDocument() {
    _scene->clearGraph();
    int size = _graphDocument->size();
    for (int i = 0; i < size; i++) {
        _graphDocument->at(i)->disconnect(this);
    }
    _scene->setActiveGraphDocument(0);
    _graphDocument = 0;
}

void GraphVisualEditor::drawGraphOnScene( Graph */*g*/) {}

void GraphVisualEditor::setActiveGraph( Graph *g) {
    _graph = g;
    _scene->setActiveGraph(g);
}

GraphScene* GraphVisualEditor::scene() const {
    return _scene;
}

QList<NodeItem*> GraphVisualEditor::selectedNodes() const {
    QList<NodeItem*> tmpList;
    QList<QGraphicsItem*> l = _scene->selectedItems();
    foreach(QGraphicsItem *i, l) {
        if ( qgraphicsitem_cast<NodeItem*>(i) ) {
            tmpList.append( qgraphicsitem_cast<NodeItem*>(i) );
        }
    }
    return tmpList;
}
