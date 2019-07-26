/*
 * Dijkstra Single Source Shortest Path Algorithm
 *
 * Written by ctonetti
 * 
 * (Don't accept negative edges in the graph)
 *
 */

var INF = 1000000
var dist = {}

function reset(G) {
    G.nodes().forEach(unmark)
    G.edges().forEach(unmark)
}

function getIndex(node, array) {
    for (var i = 0; i < array.length; i++) {
        if (array[i].element.id == node.id) {
            return i
        }
    }
    
    return -1
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

function Element(e, p) { 
    this.element = e
    this.priority = p
}

function PriorityQueue() { 
    
    this.items = []
    
    this.enqueue = function(node, weight) { 
	var e = new Element(node, weight); 
	var contain = false; 

	for (var i = 0; i < this.items.length; i++) { 
            if (this.items[i].priority > e.priority) { 
                this.items.splice(i, 0, e); 
                contain = true; 
                break; 
            } 
	} 

	if (!contain) { 
            this.items.push(e); 
	} 
    }

    this.dequeue = function() { 
	if (this.isEmpty()) 
            return "Underflow"; 
	return this.items.shift(); 
    }
    
    this.update = function(node, weight) {
        var idx = getIndex(node, this.items)
        
        // In this case, we only update the PQ if the new priority
        // is lower
        if (idx != -1 && weight < this.items[idx].priority) {
            this.items.splice(idx, 1)
            this.enqueue(node, weight)
            return true
        }
        return false
    }
    
    this.isEmpty = function() { 
        return this.items.length == 0; 
    }
    
    this.print = function() {
        this.items.forEach(function(item) {
            Console.log(item.element.from().id + " - " + item.element.to().id)
            Console.log(item.priority)
            Console.log("----")
        })
    }

} 

function dijkstra(G, start) {
    
    var pq = new PriorityQueue()
    
    // Initialize node values with infinity
    G.nodes().forEach(function(node) {
        pq.enqueue(node, INF)
    })
    
    pq.update(start, 0)
    mark(start) // Initial is marked
    
    while (!pq.isEmpty()) {
        
        var e = pq.dequeue()
        
        // Update distances
        dist[e.element.id] = e.priority
        
        // Relax each neighbor
        e.element.outEdges().forEach(function(edge) {
            pq.update(edge.to(), edge.weight + e.priority)
        })
    }
    
    // print the results in the output window
    printDistances(G, start)
}

reset(Document)
createEdgeWeight(Document)
dijkstra(Document, Document.nodes()[0])
