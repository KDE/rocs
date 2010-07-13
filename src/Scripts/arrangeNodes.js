function maxAdjInArray(array){
  c = -1;
  for (var i = 0; i < array.length; i++){
    if (c < array[i])
      c = array[i];
  }
  return c;
}

function layingOutNodes(arrayNode, graph){
    if (arrayNode.length == 0){
      return;
    }
    rInc = 300/arrayNode.length;
    debug(rInc);
    r = 0;
    if (arrayNode[0].length > 1){
      r = rInc;
    }
    for (i = 0; i < arrayNode.length; i++){
      pos = 300/2 + rInc
      nodes = arrayNode[i]
      for (j = 0; j < nodes.length; j++){
	grad = 360/nodes.length * j
	nodes[j].x = Math.cos(grad*Math.PI/180) * r + 400;
	nodes[j].y = Math.sin(grad*Math.PI/180) * r + 300;
	
      }
      r += rInc;
    }
  
}

for (gCount = 0; gCount < graphs.length; gCount++){
  graph = graphs[gCount];
  list = [];
  var nodes = graph.list_nodes();
  for(var nCount = 0; nCount < nodes.length; nCount++){
    node = nodes[nCount];
    list[nCount] = node.adj_nodes().length;
    debug(list[nCount])
  }
    c = maxAdjInArray(list);
    circles = 0;
    arrayNode = new Array;
    while (c != -1){
      
      debug(c);
      i = 0;
      node = new Array;
      for (nTmp = 0; nTmp < nodes.length; nTmp++){
	  if (list[nTmp] == c){
	    node[i++] = nodes[nTmp];
	    list[nTmp] = -1
	    
	  }
      }
      c = maxAdjInArray(list);
      arrayNode[circles++] = node;
    }
  debug (circles);
  debug(arrayNode[circles-1].length)
  layingOutNodes(arrayNode, graph);
}