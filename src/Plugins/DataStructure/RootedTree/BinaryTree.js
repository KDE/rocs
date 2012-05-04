function binarySearch(root, v){
    if (root == undefined)
      return root
    if (root.name == v)
      return root
    if (v < root.name)
        return binarySearch(root.leftChild, v);
    else
        return binarySearch(root.rightChild, v);
}
function binaryTreeInsert(root, v){
    y = undefined
    while (root != undefined){
        y = root
        if (v < root.name)
            root = root.leftChild
        else
            root = root.rightChild
    }
    newNode = graphs[0].add_data(v)
    if (y == undefined)
      graphs[0].set_root_node(newNode)
    else {
        newNode.add_node_parent(y)
        if (newNode.name < y.name)
          y.add_left_child(newNode)
        else
          y.add_right_child(newNode)
    }
    newNode.adjustPosition()
}
function printInOrder(root){
    if (root != undefined){
        printInOrder(root.left_child())
        debug (" " + root.name + " ")
        printInOrder(root.right_child())
    }
}
function printInOrder(root){
    if (root != undefined){
        printInOrder(root.left_child())
        debug (" " + root.name + " ")
        printInOrder(root.right_child())
    }
}
function minimun(root){
    if (root == undefined)
        return root;
    if (root.left_child() == undefined)
        return root
    return minimun(root.left_child())
}
function maximun(root){
    if (root == undefined)
        return root;
    if (root.right_child() == undefined)
        return root
    return maximun(root.right_child())
}
function successor(root){
    if (root.right_child() != undefined)
        return minimun(root.right_child())
    p = root.node_parent()
    while (p != undefined && root == p.right_child()){
      root = p
      p = p.node_parent()
    }
    return p;
}
function predecessor(root){
    if (root.left_child() != undefined)
        return maximun(root.left_child())
    p = root.node_parent()
    while (p != undefined && root == p.left_child()){
      root = p
      p = p.node_parent()
    }
    return p;
}
function leftRotate(root){
    if (root.rightChild == undefined)
        return
    y = root.rightChild
    root.add_right_child(y.left_child())
    if (y.leftChild != undefined)
        y.leftChild.add_node_parent(root)

    if (root.nodeParent == undefined)
        graphs[0].set_root_node(y)
    else{
        if (root == root.nodeParent.left_child() )
        root.nodeParent.add_left_child(y)
        else
        root.nodeParent.add_right_child(y)
    }
    y.add_left_child(root)
    root.add_node_parent(y)
}

function rightRotate(root){
    if (root.left_child() == undefined)
        return
    y = root.left_child()
    root.add_left_child(y.right_child())
    if (y.right_child() != undefined)
        y.right_child().add_node_parent(root)

    if (root.node_parent() == undefined)
        graphs[0].set_root_node(y)
    else{
        if (root == root.node_parent().left_child() )
        root.node_parent().add_left_child(y)
        else
        root.node_parent().add_right_child(y)
    }
    y.add_right_child(root)
    root.add_node_parent(y)
}

function height(root){
    if (root == undefined)
        return -1;
    l = height(root.left_child())
    r = height(root.right_child())
    if (l < r)
        return r + 1
    else
        return l + 1
}


function adjustNodePosition(root){
    if (root == undefined)
        return;
    root.adjustPosition()
    adjustNodePosition(root.left_child());
    adjustNodePosition(root.right_child());

}

binaryTreeInsert(graphs[0].root_node(), 2)
binaryTreeInsert(graphs[0].root_node(), 1)
binaryTreeInsert(graphs[0].root_node(), 4)
binaryTreeInsert(graphs[0].root_node(), 3)
binaryTreeInsert(graphs[0].root_node(), 6)
binaryTreeInsert(graphs[0].root_node(), 7)

rightRotate(graphs[0].root_node())
// leftRotate(graphs[0].root_node())
// debug(graphs[0].root_node().node_parent().name)


adjustNodePosition(graphs[0].root_node())

// graphs[0].begin().pointTo(graphs[0].createNode(""))