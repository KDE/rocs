/*
 * Hopcroft-Karp Maximum Bipartite Matching
 *
 * Written by ctonetti
 *
 *  Time Complexity: O(|E|sqrt(|V|))
 *
 *  A matching is a set of edges chosen in such a way that no
 *  two edges share an endpoint. A matching is of maximum size 
 *  when it has the maximum number of edges possible.
 *
 *  This algorithm starts with an empty matching and builds a
 *  maximum matching by finding augmenting paths iteratively. 
 *  The algorithm ends when there is no augmenting paths left.
 *
 */

var L = [] // Left side of the bipartite graph
var R = [] // Right side of the bipartite graph

var M = {} // Matching
var D = {} // Distances

var INF = 10000000
var NIL = -1

function reset(G) {
    G.nodes().forEach(unmark)
    G.edges().forEach(unmark)
    
    G.nodes().forEach(function(node) {
        node.setProperty("seen", false)
        node.setProperty("bip", 0)
    })
}

function mark(node) {
    node.color = '#c00'
}

function unmark(node) {
    node.color = '#fff'
}

function printMatchings() {
    
    Console.log("Matchings:")
    
    L.forEach(function(node) {
        if (M[node.id] != NIL) {
            Console.log("    " + node.id + " --- " + M[node.id].id)
        }
    })
}

// Modified BFS to divide the graph in two
// disjoint sets of nodes. This function also 
// checks if the graph is really bipartite
function divideBipartite(G) {
    
    if (G.nodes().length == 0) {
        Console.log("Empty Graph!")
        return false
    }
    
    var N = G.nodes() 
    var Q = [N[0]]
    
    N[0].bip = 1
    
    while (Q.length > 0) {
        
        var top = Q.shift()
        
        top.setProperty("seen", true)
        mark(top)
        
        neigh = top.neighbors()
        
        // Color the neighbors of top
        for (var i = 0; i < neigh.length; i++) {
            if (neigh[i].property("bip") == 0) {                    // Not assigned a set yet
                neigh[i].property("bip") = (top.property("bip") == 1 ? 2 : 1)
            } else {                                    // Assigned, check if correct
                if (neigh[i].property("bip") == top.property("bip") && neigh[i].id != top.id) {
                    return false
                }
            }
            
            if (!neigh[i].property("seen")) {
                Q.push(neigh[i])
            }
        }
    }
    
    G.nodes().forEach(function(node) {
        if (node.property("bip") == 1) {
            L.push(node)
        }
        
        if (node.property("bip") == 2) {
            R.push(node)
        }
        
        if (node.property("bip") == 0) {
            Console.log("Unconnected nodes!")
        }
    })
    
    return true
}

// Modified BFS to check if an augmenting path exist
// BFS treat the graph as layers, helping in identifying
// alternating paths. Only consider one side of the 
// bipartite graph.
function hasAugmentingPath(G) {
    
    Q = [] // Queue
    
    for (var i = 0; i < L.length; i++) {
        if (M[L[i].id] == NIL) {
            D[L[i].id] = 0
            Q.push(L[i])     // Add free vertex to queue
        } else {
            D[L[i].id] = INF
        }
    }
    
    D[NIL] = INF
    
    while (Q.length > 0) {
        
        var t = Q.pop()
        
        if (t != NIL) {
            t.neighbors().forEach(function(neigh) {
                if (D[M[neigh.id]] == INF) {
                    D[M[neigh.id]] = D[t.id] + 1
                    Q.push(M[neigh.id])
                }
            })
        }
    }
    
    return (D[NIL] != INF)
}

// Get the augmenting path that begins in node
// "node" and apply to the matching. If no such
// path exist, the function returns false, true
// otherwise.
function applyAugmentingPath(G, node) {
    
    if (node != NIL) {
        
        var neighs = node.neighbors()
        
        for (var i = 0; i < neighs.length; i++) {
            
            var neigh = neighs[i]
            
            if (D[M[neigh.id]] == D[node.id] + 1) {
                M[neigh.id] = node
                M[node.id] = neigh
                return true;
            }
        }
        
        D[node.id] = INF
        return false
    }
    
    return true
}

// Main function
function hopcroftKarp(G) {

    // Initialize all nodes as not matched
    G.nodes().forEach(function(node) {
        M[node.id] = NIL
    })
    
    var matches = 0
    
    // While a augmenting path exist,
    // update matching
    while(hasAugmentingPath(G)) {
        for (var i = 0; i < L.length; i++) {
            if (M[L[i].id] == NIL && applyAugmentingPath(G, L[i])) {
                matches++ // Count number of matches
            }
        }
    }
    
    return matches
}

reset(Document)
divideBipartite(Document)
Console.log("Total Matchings: " + hopcroftKarp(Document))
printMatchings()
