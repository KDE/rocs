#ifndef MULTINODE_H
#define MULTINODE_H

#include "Node.h"

class MultiNode : public Node
{
public:
  /*! Default Constructor */
  MultiNode(QPointF position, Graph *parent);

  /*! Default Destructor */
  ~MultiNode();

  /*! Return the first node that this node is connected to. */
  Node *getAdjacentNode(); // TODO: Need to test

  /*! return the node connected with this using edge e */
  Node *getAdjacentNode(Edge *e); // TODO: Need to test

  /*! Return the first node that has not been visited and this node is connected to */
  Node *getFirstUnvisitedNode(); // TODO: need to test

  /*! Return the first edge on the list, does not care if it's visited or not */
  Edge *getFirstEdge(); // TODO: need to test

  /*! Return the first unvisited edge on the list */
  Edge *getFirstUnvisitedEdge(); // TODO: need to test

  /*! Check if this node is connected with the To Node */
  bool isConnected(Node *to);

  /*! Add a new edge on this node. */
  void addEdge(Edge *e);	// WARNING: CANNOT CALL IT DIRECTLY
};

#endif
