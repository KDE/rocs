
//Simple scrip to show use of adjacency list of nodes
//and make a Broad Walk in the graph

nodes = graphs[0].list_nodes()

//Initialize nodes.
for (var i = 0; i < nodes.length; i++){
    nodes[i].value = 0;
}

queue = [nodes[0]]
notQuit = true
adjust = 0;
//make a broadwalk in graph
while (notQuit){
    var i = 0;
    //'POG' to make a array act like a queue :)
    while (i < queue.length && queue[i] == -1){
	i++
    }
    //empty queue. Quit
    if (i == queue.length){ 
	notQuit = false 
    }else{
	//is item not was preocessed, process it
	if (queue[i].value == 0){
	    queue[i].value = i+1-adjust
	    adj = queue[i].adj_nodes()
	    //Queue adjacency nodes.
	    for (var count = 0; count < adj.length; count++){
		if (adj[count].value == 0){
		  queue.push(adj[count])
		}
	    }
	}else{
	    //node was in the queue, but was already processed.
	    adjust += 1;
	}
	//dequeue node.
	queue[i] = -1	
    }
}
