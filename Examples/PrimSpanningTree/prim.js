/*
 * Prim's Minimum Spanning Tree
 *
 * Written by ctonetti
 * 
 * (Remember, this algorithm only works correctly in connected graphs.)
 *
 *  Time Complexity:
 *      (Depends on the data structure used)
 *      - Our Priority Queue: O(V^2)
 *      - Binary Heap: O(|E|log|V|)
 *      - Fibonacci Heap: O(|E|+|V|log|V|)
 *
 *  This algorithm finds a minimum spanning tree of
 *  a graph. A spanning tree of a graph is a tree
 *  that connects all the nodes of an original graph
 *  by selecting edges.
 *
 *  A spanning tree is minimal when the sum of the
 *  weights of it's edges is the least among all the 
 *  possible spanning tree's of a graph.
 *
 */

function reset(G, weight) {
    G.nodes().forEach(unmark)
    G.edges().forEach(unmark)
}

function mark(node) {
    node.color = '#c00'
}

function unmark(node) {
    node.color = '#fff'
}

function getIndex(node, array) {
    for (var i = 0; i < array.length; i++) {
        if (array[i].element.id == node.id) {
            return i
        }
    }
    
    return -1
}

// It is possible to create a custom function to generate weights
function createEdgeWeight(G) {
    G.edges().forEach(function(edges) {
        edges.weight = 1
    })
}

function Element(e, p) { 
    this.element = e
    this.priority = p
} 
   
// Unoptimized data structure
function PriorityQueue() { 
    
    this.items = []

    // Time Complexity: O(N)
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

    // Time Complexity: O(1)
    this.dequeue = function() { 
	if (this.isEmpty()) 
            return "Underflow"; 
	return this.items.shift(); 
    }
    
    // Time Complexity: O(N)
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
    
    // Time Complexity: O(1)
    this.isEmpty = function() { 
        return this.items.length == 0; 
    }
    
    // Time Complexity: O(N)
    this.print = function() {
        this.items.forEach(function(item) {
            Console.log(item.element.id)
            Console.log(item.priority)
            Console.log("----")
        })
    }

} 

function prim(G, start) {
    
    var tree = []
    var pq = new PriorityQueue()
    var total_weight = 0
    
    // Initialize Weights
    G.nodes().forEach(function(node) {
        if (node.id == start.id) {
            pq.enqueue(node, 0)         // The starting node begin with priority 0
        } else {
            pq.enqueue(node, 1000000)   // All other nodes begin with priority "Infinity"
        }
    })
    
    while (!pq.isEmpty()) {
        
        // Get next element from the priority key
        var val = pq.dequeue()
        var next = val.element
        
        tree.push(next)
        total_weight += val.priority
        
        // Mark the visisted node
        mark(next)
        
        next.edges().forEach(function(edge) {
            pq.update(edge.to(), edge.weight)
            pq.update(edge.from(), edge.weight)
        })
        
    }
    
    Console.log("Weight of the MST: " + total_weight)
}

reset(Document)
createEdgeWeight(Document)
prim(Document, Document.nodes()[0])
