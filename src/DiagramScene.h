#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H
#include <QGraphicsScene>
#include <QObject>
#include <QScriptValue>

#include "Group.h"
#include "DiagramNode.h"

class QString;
class QGraphicsSceneMouseEvent;
class Edge;
class MainWindow;
class QScriptEngine;
class GraphEngine;


class DiagramScene : public QGraphicsScene {
  Q_OBJECT
  
  public:
    enum Mode {  
      MoveItem,      // Moves The current Item Under Mouse
      SelectItem,     // Select the item at the current mouse position
      SquareSelect,     // Starts an Square-type Select
      InsertNode,     // Insert a new node at the current mouse position
      InsertEdge,     // Insert a new Edge between two nodes
      InsertEdgeArrow1,  // Insert a New Edge between two nodes, with an fluffy arrow.
      InsertEdgeArrow2  // Insert a new Edge between two nodes, witn a fluffy double-arrow.      
    };
    
    DiagramScene(QObject *parent = 0);
    ~DiagramScene();
    
    int action;
    GraphEngine *engine;
    void allignVLeft();
    void allignVRight();
    void allignVCenter();
    void allignHTop();
    void allignHBottom();
    void allignHCenter();
    void group();
    void ungroup();
    void removeFromGroup();
    void setStartNode();
    void setEndNode();
    void changeSelectedNodeColor(QColor c);
    void removeSelection();
    void removeEdge(Edge *e);
    void removeNode(Node *n);
    void removeGroup(Group *g);
    void toogleIndexVisibility();
    
    QList<Node*> getSelectedNodes() const;
    QList<Edge*> getSelectedEdges() const;
    QList<Node*> getNodes() const;
    QList<Edge*> getEdges() const;
    QList<Group*> getGroups() const;
    
  signals:
    void message(QString s);
    
  private:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    
    QGraphicsItem *tmpItem; //! ponteiro pra a linha que está sendo desenhada no momento.

    int arrowType; //! tipo de linha que está sendo desenhada no momento.
    
    QList<Node*> selectedNodes;
    QList<Edge*> selectedEdges;
    QList<Node*> nodes;
    QList<Edge*> edges;
    QList<Group*> groups;

    void beginInsertEdge(QPointF pos, int arrowType = 0);
    void selectItem(QPointF pos);
    void insertEdgeWithLine();
    void beginSquareSelect(QPointF pos);
    void endSquareSelect();
    void toogleWindowButtons();
    MainWindow *mainWindow;
    Node *startNode;
    Node *endNode;
    
    public slots:
    /* ######################
       #   Script Control
       ###################### */
    
    void createScriptFunctions(const QString& program);
    void insertEdge(Node *begin, Node *end, int arrowType = 0);
    void insertNode(QPointF pos);
    QScriptValue insertNode(double x1, double y1);
    void debug(const QString& s);
    
};

#endif
