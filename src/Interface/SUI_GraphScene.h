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


class GraphScene : public QGraphicsScene {
public:
    GraphScene(QObject *parent);
    void setAction(AbstractAction *action);
    QGraphicsItem* createNode(Graph *g, Node *n);
    QGraphicsItem* createEdge(Graph *g, Edge *e);
    void updateGraph(Graph *g);
    void updateDocument();
    void setActiveGraph(Graph *g);
    void setActiveGraphDocument(GraphDocument *gd);
    void insertGraphItem(Graph *g, QGraphicsItem *item);
    void clearGraph();
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
