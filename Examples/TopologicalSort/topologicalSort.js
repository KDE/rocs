/*
 * Topological Sort
 *
 * Written by ctonetti
 * 
 * (A topological order is only valid for Directed Acyclical Graphs)
 *
 *  Time Complexity: O(|E| + |V|)
 *
 *  This algorithm finds a topological order of a given graph.
 *  A topological order is an order in which each node appear
 *  before any of the nodes that can be reached by it's out
 *  edges.
 *
 */

var order = []

function reset(G) {
    G.nodes().forEach(unmark)
    G.edges().forEach(unmark)
    
    G.nodes().forEach(function(node) {
        node.setProperty("seen", false)
    })
}

function mark(node) {
    node.color = '#c00'
}

function unmark(node) {
    node.color = '#fff'
}

function printOrder() {
    order.reverse()
    
    Console.log("Topological Order:")
    order.forEach(function(node) {
        Console.log(node.id)
    })
}

// Recursive version of the DFS, modified
function topologicalSort(G, node) {
    node.setProperty("seen", true)
    mark(node)
    
    node.successors().forEach(function(succ) {
        if (!succ.property("seen")) {
            topologicalSort(G, succ)
        }
    })
    
    order.push(node)
}

reset(Document)
topologicalSort(Document, Document.nodes()[0])
printOrder()
