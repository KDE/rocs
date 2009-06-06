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
#include "graphicsitem_Node.h"
#include "graphicsitem_Edge.h"
#include "graphicsitem_OrientedEdge.h"

#include "graphDocument.h"
#include "graph.h"
#include "node.h"
#include "edge.h"
#include "graphicsitem_Node.h"


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
#include "settings.h"


GraphVisualEditor::GraphVisualEditor(MainWindow *parent)
        : QWidget(parent),
        _topNode(0),
        _bottomNode(0),
        _leftNode(0),
        _rightNode(0),
        _scene(0) {
    _graphDocument = 0;
    _graph = 0;
    _mainWindow = parent;
    setupWidgets();
}

void GraphVisualEditor::setupWidgets() {

    QHBoxLayout *layout = 0;

    layout	= new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    //!################## CODE TO GENERATE THE ALLIGN TOOLBOX ##########################

    _spacingPropContainer = new QWidget(parentWidget());

    layout->addWidget(setupToolButton("allign-h-bottom", "Allign nodes on the base", SLOT(alignHBottom()),_spacingPropContainer ));
    layout->addWidget(setupToolButton("allign-h-middle", "Allign nodes horizontally on the middle", SLOT(alignHMiddle()),_spacingPropContainer));
    layout->addWidget(setupToolButton("allign-h-top", "Allign nodes on the top", SLOT(alignHTop()),_spacingPropContainer));
    layout->addWidget(setupToolButton("allign-v-left", "Allign nodes on the left", SLOT(alignVLeft()),_spacingPropContainer ));
    layout->addWidget(setupToolButton("allign-v-middle", "Allign nodes vertically on the middle", SLOT(alignVMiddle()),_spacingPropContainer));
    layout->addWidget(setupToolButton("allign-v-right", "Allign nodes on the right", SLOT(alignVRight()),_spacingPropContainer));

    _spacingPropContainer->setLayout(layout);

    //!############################### CODE TO GENERATE THE GRAPH PROPERTIES TOOLBOX ####################################

    _graphToolBox = new GraphToolBoxWidget(_mainWindow);

    //!############################### CODE TO GENERATE THE EDGE PROPERTIES TOOLBOX #####################################
    layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    _edgePropContainer = new QWidget(parentWidget());
    _edgePropContainer -> setLayout(layout);

    //!################################ CODE TO GENERATE THE NODE PROPERTIES TOOLBOX ####################################
    layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    _nodePropContainer = new QWidget(parentWidget());

    layout->addWidget(setupToolButton("set-start", "Set this node as 'start_node' on the engine.", SLOT(setStartNode()),_nodePropContainer));
    layout->addWidget(setupToolButton("set-end","Set this node as 'end_node' on the engine. if there's more than one end node, the 'end_node' is an array",SLOT(setEndNode()),_nodePropContainer));

    _nodePropContainer -> setLayout(layout);

    //!############################### finishes the Toolbar.
    layout = new QHBoxLayout();
    _toolbar = new QWidget(parentWidget());
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget( _graphToolBox );
    layout->addWidget( _nodePropContainer );
    layout->addWidget( _edgePropContainer );
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
QToolButton* GraphVisualEditor::setupToolButton(const QString& actionName, const QString& tooltip, const char* slot, QWidget *parent) {
    QToolButton *tmpButton = 0;
    KAction *tmpAction = 0;

    tmpAction = new KAction(KIcon(actionName), tooltip, parent);
    connect( tmpAction, SIGNAL(triggered()), this, slot);
    tmpButton = new QToolButton(_spacingPropContainer);
    tmpButton -> setDefaultAction ( tmpAction );
    tmpButton -> setAutoRaise( true );
    return tmpButton;

}

void GraphVisualEditor::setActiveGraphDocument( GraphDocument *gd) {
    if ( _graphDocument != 0 ) {
        kDebug() << "Releasing Graph Document";
        releaseGraphDocument();
        kDebug() << "Graph Document Released.";
    }

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

void GraphVisualEditor::drawGraphOnScene( Graph *g) {

}

void GraphVisualEditor::setActiveGraph( Graph *g) {
    _graph = g;
    _scene->setActiveGraph(g);
}

GraphScene* GraphVisualEditor::scene() const {
    return _scene;
}

void GraphVisualEditor::alignHBottom() {
    QList<NodeItem*> l = selectedNodes();

    if ( l.size()	== 0) {
        return;
    }

    qreal bottom = l[0]->scenePos().y();
    foreach(NodeItem *i, l) {
        if ( i->scenePos().y() < bottom ) {
            bottom = i -> scenePos().y();
        }
    }
    foreach(NodeItem *i, l) {
      Node *n = i->node();
        n -> setY(bottom);
    }

}

void GraphVisualEditor::alignHMiddle() {
    QList<NodeItem*> l = selectedNodes();

    if ( l.size()	== 0) {
        return;
    }

    qreal bottom = l[0]->pos().y();
    qreal top = bottom;

    foreach(NodeItem *i, l) {
        if ( i->pos().y() < bottom ) {
            bottom = i -> scenePos().y();
        }
        else if ( i->scenePos().y() > top ) {
            top = i -> scenePos().y();
        }
    }
    qreal middle = (top + bottom) / 2;
    foreach(NodeItem *i, l) {
	i->node()->setY(middle);
    }
}
void GraphVisualEditor::alignHTop() {
    QList<NodeItem*> l = selectedNodes();

    if ( l.size()	== 0) {
        return;
    }

    qreal top = l[0]->scenePos().y();
    foreach(NodeItem *i, l) {
        if ( i->scenePos().y() > top ) {
            top = i -> scenePos().y();
        }
    }
    foreach(NodeItem *i, l) {
        i -> node() -> setY(top);
    }

}

void GraphVisualEditor::alignVLeft() {
    QList<NodeItem*> l = selectedNodes();
    if ( l.size()	== 0) {
        return;
    }

    qreal left = l[0]->scenePos().x();
    foreach(NodeItem *i, l) {
        if ( i->scenePos().x() < left ) {
            left = i -> scenePos().x();
        }
    }
    foreach(NodeItem *i, l) {
        i -> node() -> setX(left);
    }
}

void GraphVisualEditor::alignVMiddle() {
    QList<NodeItem*> l = selectedNodes();
    if ( l.size()	== 0) {
        return;
    }
    qreal left = l[0]->scenePos().x();
    qreal right = left;

    foreach(NodeItem *i, l) {
        if ( i->scenePos().x() < left ) {
            left = i -> scenePos().x();
        }
        else if ( i->scenePos().x() > right) {
            right= i->scenePos().x();
        }
    }
    qreal middle = ( left +	right) / 2;
    foreach(NodeItem *i, l) {
        i -> node() -> setX(middle);
    }
}

void GraphVisualEditor::alignVRight() {
    QList<NodeItem*> l = selectedNodes();
    if ( l.size()	== 0) {
        return;
    }
    qreal right = l[0]->scenePos().x();
    foreach(NodeItem *i, l) {
        if ( i->scenePos().x() > right ) {
            right = i -> scenePos().x();
        }
    }
    foreach(NodeItem *i, l) {
        i -> node() -> setX(right);
    }

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
