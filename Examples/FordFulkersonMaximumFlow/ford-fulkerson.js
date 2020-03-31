/* Ford-Fulkerson maximum flow algorithm
 * 
 * Writen by dilsonguim
 * 
 * Time complexity: O((|V| + |E|) * flow),
 * 
 * This algorithm computes the maximum flow in
 * a flow network. The capacity of each edge should be
 * specified in the property 'capacity'.
 * Source and sink nodes are specified directly in the script.
 *  
 */
function reset(G) {
    G.nodes().forEach(function (node) {
        node.seen = false
        node.leading_edge = ""
    })
    
    G.edges().forEach(function (edge) {
        edge.residual = false
        edge.flow = 0
        edge.capacity = parseInt(edge.capacity)
    })
}

function markNode(node) {
    node.color = "#c00"
}

function unmarkNode(node) {
    node.color = "#fff"
}

function applyToNodesInPath(path, func) {
    path.forEach(function (edge){
        func(edge.from())
    })
    func(path[0].to())
}

/* Removes residual edges from the residual network,
 * turning it back into the original graph.
 * 
 */
function removeResidualEdges(G) {
    edges = G.edges()
    edges.forEach(function (edge) {
        if (edge.residual) {
            G.remove(edge)
        }
    })
}

/* Adds residual edges to the graph, turning it into a
 * residual network.
 */
function createResidualEdges(G) {
    edges = G.edges()
    edges.forEach(function (edge) {
        residual_edge = G.createEdge(edge.to(), edge.from())
        residual_edge.type = 2
        residual_edge.residual = true
        edge.reverse = residual_edge
        residual_edge.reverse = edge
    })
}


/* Finds an augmenting path in the residual network if one
 * exists.
 */
function findAugmentingPath(G, source, sink) {
    G.nodes().forEach(function (node) {
        node.seen = false
    })
    
    var stack = [source]
    
    while (stack.length > 0) {
        current = stack.pop()
        
        if (current.seen)
            continue
        
        current.seen = true
        
        current.outEdges()
        .filter(function (edge) {
            return edge.flow < edge.capacity && !(edge.to().seen)
        })
        .forEach(function (edge) {
            
            stack.push(edge.to())
            edge.to().leading_edge = edge
        })
    }
    
    //Build array of edges in the augmenting path
    path = []
    if (sink.seen) {
        
        end_node = sink
        while (end_node != source) {
            edge = end_node.leading_edge
            path.push(edge)
            end_node = edge.from()
        }
        
    }
    return path
}

/* Increases the flow along an augmenting path.
 */
function increaseFlow(path) {
    flow_increase = path.map(function (edge) {
      return edge.capacity - edge.flow  
    }).reduce(function (flow_increase, residual_capacity) {
        return Math.min(flow_increase, residual_capacity)
    }, Infinity)
    
    path.forEach(function (edge) {
        edge.flow += flow_increase
        
        if (edge.residual) {
            edge.capacity -= edge.flow
            edge.reverse.flow -= edge.flow
            edge.flow = 0
        } else {
            edge.reverse.capacity += edge.flow
        }
    })
    
    Console.log("Flow incresed by " + flow_increase)
    
    return flow_increase
       
}

/* This algorithm works in graphs with directed edges.
 * 
 * Each edge must contain the property capacity.
 * 
 * The property flow can be used to recover the solution
 * to the maximum flow problem.
 */
function maximumFlow(G, source, sink) {
    //Transform the graph into the residual network
    createResidualEdges(Document)
    
    flow = 0
    while (true) {
        path = findAugmentingPath(Document, source, sink)
        if (path.length == 0) {
            break
        }
        
        //Mark all nodes in the augmenting path
        applyToNodesInPath(path, markNode)
        
        //Increase the flow along the augmenting path
        flow += increaseFlow(path)
        
        //Unmark nodes in the augment path
        applyToNodesInPath(path, unmarkNode)
        
    }
   
    Console.log("The maximum flow is " + flow)
    
    //Mark nodes in the same side as the sink in the min-cut
    G.nodes()
    .filter(function (node) {
        return node.seen
    })
    .forEach(markNode)
    
    //Transform the residual network back to the original graph
    removeResidualEdges(Document)    
    
    return flow
}

reset(Document)
source = Document.nodes()[0]
sink = Document.nodes()[Document.nodes().length - 1]
maximumFlow(Document, source, sink)
