#ifndef SUI_GRAPHSCENE_H
#define SUI_GRAPHSCENE_H

#include <QGraphicsScene>

class AbstractAction;
class Node;
class Edge;
class QGraphicsItem;

class GraphView : public QGraphicsScene{
public:
	GraphView();
	void setAction(AbstractAction *action);
	void createNode(Node *n);
	void createEdge(Edge *e);

private:
};

#endif