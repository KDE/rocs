#ifndef SIMPLENODE_H
#define SIMPLENODE_H

#include "Node.h"

class Edge;
class QPointF;

class SimpleNode : public Node
{
  public: 
    /*! Default constructor */
    SimpleNode(QPointF position, Graph *parent); // TESTED - OK

    /*! Default Destructor */
    ~SimpleNode();	// TESTED - OK

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

    /*! Checks to see if this node is connected with another */
    bool isConnected(Node* to);	// TESTED
				// WARNING: Probably I will need to change the behavior of this method.

    /*! Add a edge on the edges list */
    void addEdge(Edge *e);	// WARNING: DO NOT CALL IT DIRECTLY. 
};

#endif
