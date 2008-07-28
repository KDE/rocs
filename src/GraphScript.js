/***************************************************************************
 *   Copyright (C) 2005-2006 by Anne-Marie Mahfouf   *
 *   tomaz.canabrava@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

/* GRAPH CLASS */

var graph = new Object();

graph.isRegular = function(){
   if (nodes.length == 0) return false;
   var size = nodes[0].edges().length;
   for(var i = 0; i < nodes.length; i++){
      if (size != nodes[i].edges().length) return false;
   }
   return true;
}

graph.isNull = function(){
  if (edges.length == 0) return true;
  return true;
}

graph.isComplete = function(){
  //! need to check if every node has exacly NumberOfNodes-1 edges.

  if (edges.length == 0){
    if (nodes.length == 1) return true;
    return false;
  }
  
  for(var i = 0; i < nodes.length; i++ ){
    if (nodes[i].edges().length != (nodes.length - 1)) return false;
  }
  return true;
}


graph.isConnected = function(Node1, Node2){
  var e = Node1.edges();
  for(var i = 0; i < e.length; i++){
    if ((e[i].getFrom() == Node2)  || (e[i].getTo() == Node2)) return true;
  }
  return false;
}

graph.insertNode = function(x, y){
  return scene.insertNode(x,y);
}

graph.makeCopy = function(){
  if (nodes.length == 0) return;

  var copy = new Array();
  var length = nodes.length;

  for(var i = 0; i < length; i++)
  {
    var n = this.insertNode(0,0);
    copy.push(n);
  }

  var topRight = nodes[0].getX();
  var topLeft = nodes[0].getX();
  for(var i = 0; i < length; i++){
    if (topRight < nodes[i].getX()) topRight = nodes[i].getX();
    if (topLeft > nodes[i].getX())  topLeft = nodes[i].getX();
  }
  
  for(var i = 0; i < length; i++){
     var Node = copy[i];
     var posX = nodes[i].getX() + topRight - topLeft + 50;
     Node.setXY(posX, nodes[i].getY());  
  }

  return copy;  
}

graph.makeCopyReflected = function(){
  if (nodes.length == 0) return;

  var copy = new Array();
  var length = nodes.length;

  for(var i = 0; i < length; i++)
  {
    var n = this.insertNode(0,0);
    copy.push(n);
  }

  var topRight = nodes[0].getX();
  for(var i = 0; i < length; i++){
    if (topRight < nodes[i].getX()){
         topRight = nodes[i].getX();
    }
  }
  
  for(var i = 0; i < length; i++){
     var Node = copy[i];
     Node.setXY(topRight + topRight - nodes[i].getX() + 30, nodes[i].getY());  
  }

  return copy;  
}

graph.insertEdge = function(Node1, Node2, Type) {
  return scene.insertEdge(Node1, Node2, Type);
}

graph.numberOfEdges = function(node1, node2){

    if (!graph.isConnected(node1, node2)){
       return 0;
    }

    var n1_edges = node1.edges();
    var edges_size = n1_edges.length;
    var amount = 0;
    var num_loops = 0;

    for (var i = 0; i < edges_size; i++){

        if (node1 == node2){
             if (n1_edges[i].getFrom() == n1_edges[i].getTo()){
                 amount++;
                num_loops++;
             }
        }

       else if ((n1_edges[i].getFrom() == node2) || (n1_edges[i].getTo() == node2)){
          amount++;
         }
    }

    if (num_loops > 0){
        amount -= num_loops/2;
    }

   return amount;
}

graph.adjacencyMatrix = function(nullvalue){
  //! somebody has a better way for matrix creation on javascript than this?
  var adjMatrix = new Array();
  for(var i = 0; i < nodes.length; i++){
    adjMatrix[i] = new Array();
  }
  
  for(var i = 0; i < nodes.length; i++){
    for(var e = 0; e < nodes.length; e++){
      adjMatrix[i][e] = graph.numberOfEdges(nodes[i], nodes[e]);
      if (!nullValue) continue;
      if (adjMatrix[i][e] == 0) adjMatrix[i][e] = nullvalue;
    }
  }
  return adjMatrix;
}

graph.distance = function(node1, node2){
    if (!this.isConnected(node1, node2)){
        return -1;
    }

    var e = 0;
    for(var i = 0; i < node1.edges().length; i++) {
       e = node1.edges()[i];
       if  (e.getFrom() == node1 && e.getTo() == node2)
      {
          return e.getLength();
      }
     if (e.getTo() == node1 && e.getFrom() == node2){
           return e.getLength();
     }
   }
   return -1;
}


graph.incidencyMatrix = function(){

}

graph.makeIndexArray = function(from, to){
  if (to >= nodes.length) return 0;
  var tmp = new Array();
  for(var i =from; i <= to; i++){
    tmp.push(i);
  }
  return tmp;
}

graph.makeArray = function(from, to){
  if (to >= nodes.length) return 0;
  var tmp = new Array();
  for(var i = from; i <= to; i++){
    tmp.push(nodes[i]);
  }
  return tmp;
}

graph.length_table = function(nullvalue){
  if (nullvalue == undefined) nullvalue = -1;
  var m = matrix(nodes.length);
  for(var i = 0; i < nodes.length; i++){
    for(var e = 0; e < nodes.length; e++){
      var length =  graph.distance(nodes[i], nodes[e]);
      if (length == -1) m[i][e] = nullvalue;
       else m[i][e] = length;
    }
  }
  return m;
}

function matrix(size){
  var m = new Array();
  for(var i = 0; i < size; i++){
    m[i] = new Array();
  }
  return m;
}

function arrayRemoveAt( x, pos ){
    if ( pos >= this.length ) return;
    var y = new Array();
   for(var i = 0; i < x.length; i++){
       if (i != pos){
           y.push( x[i] );
      }
   }
   return y;
}
