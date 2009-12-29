// Coloring
// Select a color to nodes in a way that same colors never will be adjacent
// In case of graphs with nodes with great number of adjacency nodes, should be necessary add more colors to array 'colors'


//Some helpful funcitions
//return true if item is in array
function inArray(item, array){
  for (var j = 0; j < array.length; j++){
      if (item == array[j]){
	return true;
      }
  }
  return false;
}

//return the next color avoiding the color in array 'avoidColors'
function color(avoidColors){
   for (var k=0; k<colors.length; k++){
	if (!inArray(colors[k], avoidColors)){
	      debug(colors[k]);
	      return colors[k];
	}
   }
   return "gray";
}

//Return color of all(unic) the neibours.
function neibourColors(node){
  adjColors = [];
  adj = node.adj_nodes();
  for (var k=0; k < adj.length; k++){
      if (!inArray(adj[k].color, adjColors)){
	  adjColors.push(adj[k].color);
      }
  }
  return adjColors;
}



//******Begin Here******

nodes = graphs[0].list_nodes();
colors = ["red", "blue", "green", "yellow", "cyan", "purple", "white"];

if (nodes.length != 0){
  


//Initialize nodes.
for (var i = 0; i < nodes.length; i++){
    nodes[i].color = "black";
    nodes[i].value = 0;
}

queue = [nodes[0]]
notQuit = true;
adjust = 0;
//make a broadsearch in graph
while (notQuit){
    var i = 0;
    //POD to make a array act like a queue :)
    while (i < queue.length && queue[i] == -1){
	i++;
    }
    //empty queue. Quit
    if (i == queue.length){ 
	notQuit = false;
    }else{ 
	//is item not was preocessed, process it
	if (queue[i].value == 0){
	    queue[i].value = i+1-adjust;
	    queue[i].color = color(neibourColors(queue[i]));
	    adj = queue[i].adj_nodes();
	    //Queue adjacency nodes.
	    for (var count = 0; count < adj.length; count++){
		if (adj[count].value == 0){
		  queue.push(adj[count]);
		}
	    }
	}else{
	    //node was in the queue, but was already processed.
	    adjust += 1;
	}
	//pop queue.
	queue[i] = -1;
    }
}
}