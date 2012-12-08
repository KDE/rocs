/**
 * Breadth-first search
 *
 *  __(')<  written by lambda fairy
 * \___)
 */

function reset_graph(g) {
  function reset(thing) {
    thing.set_type(0)
    thing.color = '#999'
  }
  g.list_nodes().forEach(reset)
  g.list_edges().forEach(reset)
}

function add_mine(g) {
  var mine = g.list_nodes()[2]  // Completely arbitrary
  mine.set_type(1)
}

function bfs(start, mark, is_match) {
  var queue = [start]
  start.seen = true; mark(start); interrupt()
  var cursor
  while (queue.length > 0) {
    cursor = queue.shift()
    if (is_match(cursor)) {
      interrupt()
      return true
    }
    cursor.adj_nodes().forEach(function(node) {
      if (!node.seen) {
	node.seen = true; mark(node)
	queue.push(node)
      }
    }); interrupt()
  }
  return false
}

function set_type_to(value) {
  return function(thing) { thing.set_type(value) }
}

reset_graph(lake)
add_mine(lake)

bfs(lake.list_nodes()[0],
    function(node) {
      output('Found ' + node.name)
      node.color = '#f90'
    },
    function(node) {
      var match = false
      if (node.type() == 1) {
        output('-- Deactivated mine at ' + node.name)
	node.color = '#c00'
	return true
      } else {
	output('-- Cleared ' + node.name)
        node.color = '#0a0'
	return false
      }
    })