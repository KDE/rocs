#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QString>
#include <QPointF>
#include <QColor>

class Edge;
class Graph;

class Node : public QObject
{
Q_OBJECT

Q_PROPERTY(QPointF position READ position WRITE setPosition)
Q_PROPERTY(QColor color READ color WRITE setColor)
Q_PROPERTY(QString name READ name WRITE setName)
Q_PROPERTY(int index READ index WRITE setIndex)
Q_PROPERTY(bool visited READ visited WRITE setVisited RESET unsetVisited)

public:
  /*! Default Constructor */
  Node(QPointF position, Graph *parent);	// TESTED - OK
  
  /*! Default Destructor */
  virtual ~Node();// TESTED - OK

  /*! gets the name of the Node */
  QString name() const;// TESTED - OK

  /*! sets the name of the node */
  void setName(QString name);// TESTED - OK

  /*! gets the position of the node */
  QPointF position() const;// TESTED - OK

  /*! sets the position of the node */
  void setPosition(QPointF position);// TESTED - OK

  /*! gets the node's color */
  QColor color() const;// TESTED - OK
  
  /*! sets the Node's color */
  void setColor(QColor color);// TESTED - OK

  /*! gets the node Index */
  int index() const;// TESTED - OK

  /*!sets the node Index */
  void setIndex(int index); // TESTED - OK
			    //WARNING: This function CANNOT be called direcly. ONLY the graph can call it.

  /*! checks if the node is or isn't visited */
  bool visited(); // TODO: Test this

  /*! set the visited to true */
  void setVisited(bool visited); // TODO: Test this.

  /*! unset the visited node */
  void unsetVisited();	// TODO: Test this.

  /*! Add a Edge to the list */
  virtual void addEdge(Edge *e) = 0;	// PURE VIRTUAL, TEST ON THE INHERETED CLASSES

  /*! check to see if there's any Node equals to this one on the Connected List */
  virtual bool isConnected(Node *n) = 0; // PURE VIRTUAL, TEST ON THE INHERETED CLASSES

  /*! removes one edge from the list, if the edge exists inside of the node */
  void destroyEdge(Edge *e); // TESTED - OK
			      //WARNING: This function CANNOT be called directly. ONLY the edge can call it.

  /*! Removes the node at specified index from the list */
  void removeEdge(int index); // TESTED - OK

  /*! remove the specified edge from the list */
  void removeEdge(Edge *e); // TESTED - OK

  /*! Return all edges connected with this node */
  QList<Edge*> edges() const; // TESTED - OK

  /*! Return the first node that this node is connected to. */
  virtual Node *getAdjacentNode() = 0; 

  /*! return the node connected with this using edge e */
  virtual Node *getAdjacentNode(Edge *e) = 0; 

  /*! Return the first node that has not been visited and this node is connected to */
  virtual Node *getFirstUnvisitedNode() = 0; 

  /*! Return the first edge on the list, does not care if it's visited or not */
  virtual Edge *getFirstEdge() = 0; 

  /*! Return the first unvisited edge on the list */
  virtual Edge *getFirstUnvisitedEdge() = 0; 


signals:
  /*! emited when the name changes */
  void nameChanged(QString name);
  
  /*! emited when the position changes */
  void positionChanged(QPointF position);
  
  /*! emited wwhen the color changes */
  void colorChanged(QColor color);

  /*! emited when the index changes */
  void indexChanged(int index);

  /*! emited when visited status changes */
  void visitedChanged(bool b);

protected:
  QList<Edge*> _edges;
  QColor _color;
  QPointF _position;
  QString _name;
  int _index;
  bool _visited;
  
};

#endif
