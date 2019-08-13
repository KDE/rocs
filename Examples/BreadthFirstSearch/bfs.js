/*
 * Breadth-First Search
 *
 * Written by ctonetti
 *
 *  Time Complexity: O(|E| + |V|)
 *
 *  This algorithm is used to traverse an graph in an
 *  tree-like structure by exploring the nodes in
 *  "layers", prioritizing the exploration of the
 *  nearest neighbor of a node.
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

function bfs(G, start) {
    
    var queue = [start]
    start.seen = true

    while (queue.length > 0) {
        
        var top = queue.shift();
        top.seen = true

        mark(top)

        top.neighbors()
        .filter(function(x) {
            return !(x.seen)
        })
        .forEach(function(x) {
            queue.push(x)
        })
    }
}

reset(Document)
bfs(Document, Document.nodes()[0])
