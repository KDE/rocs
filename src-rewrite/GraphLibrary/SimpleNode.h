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

    /*! Checks to see if this node is connected with another */
    bool isConnected(Node* to);	// TESTED
				// WARNING: Probably I will need to change the behavior of this method.

    /*! Add a edge on the edges list */
    void addEdge(Edge *e);	// WARNING: DO NOT CALL IT DIRECTLY. 
};

#endif
