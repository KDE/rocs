/*
 * Breadth-First Search
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
