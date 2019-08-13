/*
 * Depth-First Search
 *
 * Written by ctonetti
 *
 *  Time Complexity: O(|E| + |V|)
 *
 *  This algorithm is used to traverse an graph in an
 *  tree-like structure by exploring the first branches
 *  it encounter in each node before backtracking when 
 *  this is not possible anymore. This process continues 
 *  until there is no more graph to be explored.
 *
 *  This is an useful algorithm, as it can be used to
 *  detect loops, separate connected components, detect
 *  bridges and articulation vertexes, etc.
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
