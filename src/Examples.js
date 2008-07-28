
function complement(){
  var size = nodes.length;
  var copy = graph.makeCopy(); 

  for(var i = 0; i < size - 1; i++){
    for (var e = i+ 1; e < size; e++){
      if (e == i) continue;
      if ( graph.isConnected(nodes[i], nodes[e])) continue;
      if ( graph.isConnected(copy[i], copy[e])) continue;
      graph.insertEdge(copy[i], copy[e], 0);
    }
  }
}

