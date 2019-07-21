/*
 * Depth-First Search
 *
 * Written by ctonetti
 *
 */

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

function dfs(G, start) {
    
    var stk = [start]
    start.seen = true
    
    while (stk.length > 0) {
        // Get next node
        var top = stk.pop()
        top.seen = true
        
        // Mark the node
        mark(top)
        
        // Process
        top.neighbors()
        .filter(function(x) {   // Only get the unseen neighbors
            return !(x.seen)
        })
        .forEach(function(x) {  // Push the unseen nodes
            stk.push(x)
        })
        
    }
}

reset(Document)
dfs(Document, Document.nodes()[0])
