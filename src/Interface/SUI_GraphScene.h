#ifndef SUI_GRAPHSCENE_H
#define SUI_GRAPHSCENE_H

#include <QGraphicsScene>

class AbstractAction;
class Node;
class Edge;
class QGraphicsItem;
class GraphDocument;
class Graph;

class GraphScene : public QGraphicsScene{
public:
	GraphScene(QObject *parent);
	void setAction(AbstractAction *action);
	void createNode(Node *n);
	void createEdge(Edge *e);

private:
	GraphDocument *_graphDocument;
	Graph *_activeGraph;
	
};

#endif