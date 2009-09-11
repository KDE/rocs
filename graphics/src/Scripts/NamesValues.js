var Nomes = new Array("A", "B", "C1", "D1");
var Valores = new Array(1,22,3,44);
var Nodes = g.list_nodes();

for(var i = 0; i < Nodes.length; i++){
  Nodes[i].name = Nomes[i];
  Nodes[i].value = Valores[i];
}