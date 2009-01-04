#include "SUI_GraphView.h"
#include "action_AbstractAction.h"

#include "node.h"
#include "edge.h"
#include <QGraphicsItem>


GraphView::GraphView() : QGraphicsScene(0){}

void GraphView::setAction(AbstractAction *){}

void GraphView::createNode(Node *){}

void GraphView::createEdge(Edge *){}
