/* 
 * Kruskal`s Minimum Spanning Tree
 *
 * Written by ctonetti
 * 
 * (Remember, this algorithm only works correctly in connected graphs.)
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
