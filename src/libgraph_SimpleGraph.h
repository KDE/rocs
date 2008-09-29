#ifndef SIMPLEGRAPH_H
#define SIMPLEGRAPH_H

#include <libgraph_Graph.h>

namespace libgraph
{

class Edge;
class Node;
class GraphDocument;

class SimpleGraph : public Graph
{
  public:
    /*! Default Constructor */
    SimpleGraph(GraphDocument *parent);	// TESTED - OK

    /*! Default Destructor */
    ~SimpleGraph();	// TESTED - OK

    /*! Create a new edge and connect it on both from and to nodes */
    Edge* createEdge(Node* from, Node* to); // TESTED - OK

    /*! Create a new node and places it at expected position */
    Node* createNode(QPointF position);	// TESTED - OK
};

};

#endif
