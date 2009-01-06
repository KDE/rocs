#include "SUI_GraphScene.h"
#include "action_AbstractAction.h"

#include "node.h"
#include "edge.h"
#include <QGraphicsItem>


GraphScene::GraphScene(QObject *parent) : QGraphicsScene(parent){
	
}

void GraphScene::setAction(AbstractAction *){}

void GraphScene::createNode(Node *){}

void GraphScene::createEdge(Edge *){}
