/*
 * Bellman-Ford Single Source Shortest Path Algorithm
 *
 * Written by ctonetti
 * 
 * (Can detect negative cycles in graph)
 *
 */

var INF = 1000000
var dist = {}

function reset(G) {
    G.nodes().forEach(unmark)
    G.edges().forEach(unmark)
}

function printDistances(G, start) {
    
    G.nodes().forEach(function(node) {
        if (node.id != start.id) {
            if (dist[node.id] != INF) {
                Console.log("Distance: " + start.id + " -> " + node.id + ": " + dist[node.id])
            } else {
                Console.log("Distance: " + start.id + " -> " + node.id + ": INF")
            }
        }
    })
}

// It is possible to create a custom function to generate weights
function createEdgeWeight(G) {
    G.edges().forEach(function(edges) {
        edges.weight = 1
    })
}

function mark(node) {
    node.color = '#c00'
}

function unmark(node) {
    node.color = '#fff'
}

function bellmanFord(G, start) {
    // Initialize node values with infinity
    G.nodes().forEach(function(node) {
        dist[node.id] = INF
    })
    
    dist[start.id] = 0
    mark(start)
    
    for (var i = 0; i < G.nodes().length; i++) {
        G.edges().forEach(function(edge) {
            if (dist[edge.from().id] + edge.weight < dist[edge.to().id]) {
                dist[edge.to().id] = dist[edge.from().id] + edge.weight
            }
        })
    }
    
    // Check for negative cycles
    var negative_cycle = false
    G.edges().forEach(function(edge) {
        if (dist[edge.from().id] + edge.weight < dist[edge.to().id]) {
            Console.log("Graph contains negative cycle!")
            negative_cycle = true
        }
    })
    
    // print the results in the output window
    if (!negative_cycle) {
        printDistances(G, start)
    }
}

reset(Document)
createEdgeWeight(Document)
bellmanFord(Document, Document.nodes()[0])
