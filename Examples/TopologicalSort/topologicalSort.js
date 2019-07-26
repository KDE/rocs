/*
 * Topological Sort
 *
 * Written by ctonetti
 * 
 * (A topological order is only valid for Directed Acyclical Graphs)
 *
 */

var order = []

function reset(G) {
    G.nodes().forEach(unmark)
    G.edges().forEach(unmark)
    
    G.nodes().forEach(function(node) {
        node.seen = false
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
    node.seen = true
    mark(node)
    
    node.successors().forEach(function(succ) {
        if (!succ.seen) {
            topologicalSort(G, succ)
        }
    })
    
    order.push(node)
}

reset(Document)
topologicalSort(Document, Document.nodes()[0])
printOrder()
