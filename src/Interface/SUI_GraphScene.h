#ifndef SUI_GRAPHSCENE_H
#define SUI_GRAPHSCENE_H

#include <QGraphicsScene>
#include <QMultiHash>

class AbstractAction;
class Node;
class Edge;
class QGraphicsItem;
class GraphDocument;
class Graph;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
class QKeyEvent;
class NodeItem;
class EdgeItem;


class GraphScene : public QGraphicsScene{
public:
	GraphScene(QObject *parent);
	void setAction(AbstractAction *action);
	NodeItem* createNode(Node *n);
	EdgeItem* createEdge(Edge *e);
	void updateGraph(Graph *g);
	void setGraph(Graph *g);
	void insertGraphItem(QGraphicsItem *item);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent);
	void keyPressEvent(QKeyEvent *keyEvent);

private:
	GraphDocument *_graphDocument;
	Graph *_graph;
	AbstractAction *_action;
	QMultiHash<Graph*, QGraphicsItem* > _hashGraphs;
};

#endif