#ifndef SIMPLEGRAPH_H
#define SIMPLEGRAPH_H

#include <Graph.h>

class Edge;
class Node;

class SimpleGraph : public Graph
{
  public:
    /*! Default Constructor */
    SimpleGraph();	// TESTED - OK

    /*! Default Destructor */
    ~SimpleGraph();	// TESTED - OK

    /*! Create a new edge and connect it on both from and to nodes */
    Edge* createEdge(Node* from, Node* to); // TESTED - OK

    /*! Create a new node and places it at expected position */
    Node* createNode(QPointF position);	// TESTED - OK
};

#endif
