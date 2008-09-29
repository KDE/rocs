#ifndef GRAPH_H
#define GRAPH_H

#include <QList>
#include <QObject>
#include <QColor>
#include <QString>

class Node;
class Edge;
class QPointF;
class GraphCollection;

class Graph : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName)
  Q_PROPERTY(QColor color READ color WRITE setColor)

public:
  /*! Default Constructor */
  Graph(GraphCollection *parent);						// TESTED - OK

  /*! Default Destructor */
  virtual ~Graph(); 					// TESTED - OK
    
  /*! Changes the Graph's Name */
  void setName(const QString& name); 			// TESTED - OK

  /*! gets the name of the Graph */
  QString name() const; 				// TESTED - OK

  /*! Changes the Graph's Color */
  void setColor(QColor color); 				// TESTED - OK

  /*! Gets the color of the Graph */
  QColor color() const; 				// TESTED - OK

  /*! Creates a new Node */
  virtual Node* createNode(QPointF Position) = 0;	// PURE VIRTUAL, TEST EACH OF INHERETED ONES.

  /*! Creates a new Edge between 2 nodes */
  virtual Edge* createEdge(Node* from, Node* to) = 0;	// PURE VIRTUAL, TEST EACH OF INHERETED ONES.

  /*! Removes a node */
  void removeNode(int index);				// TESTED - OK

  /*! removes a node */
  void removeNode(Node *node);				// TESTED - OK

  /*! Removes an Edge */
  void removeEdge(int index);				// TESTED - OK

  /*! removes an edge that doesn't have any more links */
  void removeEdge(Edge *e);				// TESTED - OK

  /*! destroys an edge from the list */
  void destroyEdge(Edge *e); // WARNING: DO NOT CALL IT DIRECTLY! - TESTED - OK

  /*! Returns all nodes from the graph */		// TESTED - OK
  QList<Node*> nodes() const;

  /*! Returns all edges from the graph */
  QList<Edge*> edges() const;				// TESTED - OK

  /*! transform this graph into a KGraph */
  void transformIntoKGraph();				// TESTED - OK. ( But should move to respective files, maybe? )

  /*! make this graph a KGraph with nodes Nodes and a radius = radius at the specified pos.. erase everything inside. */
  void makeKGraph(int nodes, qreal radius, QPointF pos);

  /*! make this graph a CGraph with nodes Nodes and a radius = radius at the specified pos. erase everything inside */
  void makeCGraph(int nodes, qreal radius, QPointF pos);

  /*! Create nodes Nodes in a circular form with radius radius at position pos. */
  int makeConcentricNodes(int nodes, qreal radius, QPointF pos);
  
signals:
  /*! emited when the name changes */
  void nameChanged(const QString& name);

  /*! emited when the color chnges */
  void colorChanged(const QColor& color);

  /*! emited when a new node is created */
  void nodeCreated(Node* node);

  /*! emited when a new edge is created */
  void edgeCreated(Edge* edge);

  /*! emited when a node is destroyed */
  void nodeRemoved(int i);

  /*! emited when a edge is destroyed */
  void edgeRemoved(int i);

protected:
  QList<Node*> _nodes;
  QList<Edge*> _edges;
  QString _name;
  QColor _color;

};

#endif
