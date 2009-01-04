#ifndef SUI_GRAPHSCENE_H
#define SUI_GRAPHSCENE_H

#include <QGraphicsScene>

class AbstractAction;
class Node;
class Edge;
class QGraphicsItem;

class GraphScene : public QGraphicsScene{
public:
	GraphScene();
	void setAction(AbstractAction *action);
	void createNode(Node *n);
	void createEdge(Edge *e);

private:
};

#endif