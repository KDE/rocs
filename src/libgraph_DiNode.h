#ifndef DINODE_H
#define DINODE_H

#include "libgraph_Node.h"
namespace libgraph
{

class Edge;
class Graph;

class DiNode : public Node 
{
public:
  /*! Default constructor */
  DiNode(QPointF position, Graph *parent);
  
  /*! Default Destructor */
  ~DiNode();

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
  
  /*! Verify if this node is connected with the To node */
  bool isConnected(Node *to);

  /*! add a new edge to this node */
  void addEdge(Edge *e);	//WARNING: CANNOT CALL IT DIRECLY.
};

};

#endif