/*
 * Floyd-Warshall Multiple Sources Shortest Paths Algorithm
 *
 * Written by ctonetti
 * 
 *
 */

var INF = 1000000
var dist = {}

function reset(G) {
    G.nodes().forEach(unmark)
    G.edges().forEach(unmark)
    
    var V = G.nodes().length
    var N = G.nodes()
    
    // Create raw weight matrix
    for (var i = 0; i < V; i++) {
        dist[N[i].id] = {}
        for (var j = 0; j < V; j++) {
            if (N[i].id == N[j].id) {
                dist[N[i].id][N[j].id] = 0
            } else {
                dist[N[i].id][N[j].id] = INF
            }
        }
    }
    
    createEdgeWeight(Document)
    
    G.edges().forEach(function(edge) {
        dist[edge.from().id][edge.to().id] = edge.weight
        dist[edge.to().id][edge.from().id] = edge.weight
    })
}

function printDistances(G) {
    var V = G.nodes().length
    var N = G.nodes()
    
    Console.log("Distance Matrix:")
    
    line = "id "
    for (var i = 0; i < V; i++) {
        line += N[i].id + " "
    }
    Console.log(line)
    
    for (var i = 0; i < V; i++) {
        var line = N[i].id + "  "
        for (var j = 0; j < V; j++) {
            if (dist[N[i].id][N[j].id] == INF) {
                line += "INF  "
            } else {
                line += dist[N[i].id][N[j].id] + "  "
            }
        }
        Console.log(line)
    }
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

function floydWarshall(G) {
    
    var V = G.nodes().length
    var N = G.nodes()

    for (var k = 0; k < V; k++) {
        for (var i = 0; i < V; i++) {
            for (var j = 0; j < V; j++) {
                if (dist[N[i].id][N[j].id] > dist[N[i].id][N[k].id] + dist[N[k].id][N[j].id]) {
                    dist[N[i].id][N[j].id] = dist[N[i].id][N[k].id] + dist[N[k].id][N[j].id]
                }
            }
        }
    }
    
    printDistances(G)
}

reset(Document)
floydWarshall(Document)
