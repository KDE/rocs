/* 
 * Kruskal`s Minimum Spanning Tree
 *
 * Written by ctonetti
 * 
 * (Remember, this algorithm only works correctly in connected graphs.)
 *
 *  Time Complexity: O(V^2)
 *      (Depends on the data structure used and the type of 
 *      sorting algorithm. Optimally is O(E*ack(V), where
 *      ack() is the ackermann function, if there is guarantee
 *      that the edges can be sorted in linear time (counting
 *      sort or radix sort))
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
        if (array[i].property("element").id == node.id) {
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
            if (this.items[i].property("priority") > e.property("priority")) {
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
        if (idx != -1 && weight < this.items[idx].property("priority")) {
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
            Console.log(item.property("element").from().id + " - " + item.property("element").to().id)
            Console.log(item.property("priority"))
            Console.log("----")
        })
    }

} 

function UnionFind() {
    
    this.parents = {}
    
    this.count = 0
    
    this.init = function(items) {
        // Construct forest of disjoint elements
        // Each element is a node in the original
        // graph
        for (var i=0; i < items.length; i++) {
            this.parents[items[i].id] = items[i]
        }
    
        this.count = items.length
    }
    
    // Unite two disjoint sets
    this.union = function(a, b) {
        var root_a = this.find(a)
        var root_b = this.find(b)
        
        // Check if the two are already in the same tree
        if (root_a.id == root_b.id) {
            return
        }
        
        if (root_a.id < root_b.id) {
            if (this.parents[b.id] != b) {
                this.union(this.parents[b.id], a)
            }
            
            this.parents[b.id] = this.parents[a.id]
        } else {
            if (this.parents[a.id] != a) {
                this.union(this.parents[a.id], b)
            }
            
            this.parents[a.id] = this.parents[b.id]
        }
    }
    
    this.find = function(a) {
        while (this.parents[a.id] != a) {
            a = this.parents[a.id]
        }
        
        return a
    }
    
    this.connected = function(a, b) {
        return this.find(a).id == this.find(b).id
    }
}

function kruskal(G) {
    
    var pq = new PriorityQueue()
    var uf = new UnionFind()
    
    var tree = []
    
    uf.init(G.nodes())
    
    G.edges().forEach(function(e) {
        pq.enqueue(e, e.weight)
    })
    
    while (!pq.isEmpty()) {
        
        var edge = pq.dequeue().element
        
        if (uf.connected(edge.from(), edge.to())) {
            continue
        }
        
        Console.log("Edge: " + edge.from().id + " - " + edge.to().id + " Added!")
        
        uf.union(edge.from(), edge.to())
        tree.push(edge)
    }
    
    return tree
}

reset(Document)
createEdgeWeight(Document)
Console.log(kruskal(Document).length)
