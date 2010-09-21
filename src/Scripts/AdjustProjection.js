function degToRad(num){
  return (num/180)*Math.PI
}

var EarthRadius = 6378.1;

function projectionGallPeters(node){
  node.x = EarthRadius * degToRad(node.Longitude) * 1000
  node.y = 600 - 2 * EarthRadius * degToRad(nodes[i].Latitude) * 1000
}

//From http://www.movable-type.co.uk/scripts/latlong.html
function distance(node1, node2){
  var dLat = degToRad(node2.Latitude-node1.Latitude);
  var dLon = degToRad(node2.Longitude-node1.Longitude);
  var a = Math.sin(dLat/2) * Math.sin(dLat/2) +
	  Math.cos(degToRad(node1.Latitude)) * Math.cos(degToRad(node2.Latitude)) *
	  Math.sin(dLon/2) * Math.sin(dLon/2);
  var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));
  var d = EarthRadius * c;
  debug(d)
}

function fitGraphInDoc(graph){
  nodes = graph.list_nodes();

  ratioX = 1;
  ratioY = 1;
  shiftX = 20;
  shiftY = 20;
  height = 600 - (shiftY*2);
  width  = 800 - (shiftX*2);

  if (nodes.length == 0)
    return;
  maxX = minX = nodes[0].x;
  maxY = minY = nodes[0].y;
  for (i = 0; i < nodes.length; ++i){
    maxX = Math.max(maxX, nodes[i].x);
    maxY = Math.max(maxY, nodes[i].y);
    minX = Math.min(minX, nodes[i].x);
    minY = Math.min(minY, nodes[i].y);
  }

  factorX = width/(maxX - minX);
  factorY = height/(maxY - minY);


  if ((maxY == minY) || (maxX == minX)){
    for (i = 0 ; i < nodes.length; ++i){
          nodes[i].x = width/2;
          nodes[i].y = height/2;
    }
    return
  }

  if ((maxY - minY) > (maxX - minX)){
      ratioX = (maxX - minX) / (maxY - minY);
      shiftX = ((width + 2* shiftX) - ((maxX - minX)* factorX * ratioX)) /2;
  }else {
      ratioY = (maxY - minY) / (maxX - minX);
      shiftX = (( height + 2* shiftY)- ((maxY - minY)* factorY * ratioY)) /2;
  }



  for (i = 0 ; i < nodes.length; ++i){
          nodes[i].x = (nodes[i].x - minX) * factorX * ratioX + shiftX;
          nodes[i].y = (nodes[i].y - minY) * factorY * ratioY + shiftY;
	  debug(nodes[i].x + ", " + nodes[i].y);
  }
}

nodes = graphs[0].list_nodes();
for (i = 0; i < nodes.length; i++){
  projectionGallPeters(nodes[i])
  if (i != 0)
     distance(nodes[i-1], nodes[i]);
}

fitGraphInDoc(graphs[0]);
