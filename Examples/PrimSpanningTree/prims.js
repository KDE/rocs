/**
 * Prim's algorithm
 *
 *  __(')<  written by lambda fairy
 * \___)
 */

function reset_graph(g) {
  function reset(thing) {
    thing.set_type(0)
    thing.color = '#000'
  }
  function reset_node(node) {
    reset(node)
    node.width = 0.3
  }
  function reset_edge(edge) {
    reset(edge)
    edge.width = 1
  }
  g.nodes().forEach(reset_node)
  g.edges().forEach(reset_edge)
}

function add_weights(g) {
  g.edges().forEach(function(edge) {
    edge.weight = randint(1, 10)
  })
}

function randint(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min
}

function reset_visited(nodes) {
  nodes.forEach(function(node) {
    node.visited = false
  })
}

function get_unvisited(edge) {
  var s = edge.from.visited, e = edge.to.visited
  if (s && !e) {
    return edge.to
  } else if (!s && e) {
    return edge.from
  } else {
    return null
  }
}

function prim(g, start, mark_node, mark_edge) {
  // Mark the starting point
  reset_visited(g.nodes())
  start.visited = true
  var tree = [start]
  mark_node(start); interrupt()
  // Loop until all nodes are included in the tree
  while (tree.length < g.nodes().length) {
    // Go through all the edges connected to the tree ...
    var best_weight = Infinity, best_target = null, best_edge = null
    tree.forEach(function(node) {
      // ... and choose the one with the lowest weight
      node.adj_edges().forEach(function(edge) {
        var outside = get_unvisited(edge)
	if (outside != null && edge.weight < best_weight) {
	  best_weight = edge.weight
	  best_target = outside
	  best_edge = edge
	}
      })
    })
    // Mark the closest node, and add it to the tree
    tree.push(best_target)
    best_target.visited = true
    mark_node(best_target); mark_edge(best_edge); interrupt()
  }
}

reset_graph(network)
add_weights(network)
var server = network.nodes()[0]
server.set_type(1)
prim(network, server,
     function(node) {
       if (node.type() == 1) {
	 Console.log('Starting server ' + node.name)
	 node.color = '#00f'
	 node.width = 0.6
       } else {
         Console.log('Connecting ' + node.name)
         node.color = '#0a0'
       }
     },
     function(edge) {
       edge.set_type(1)
       edge.color = '#0a0'
       edge.width = 2
       Console.log(' -- latency: ' + edge.weight)
     })

Console.log('== All clients connected ==')
