#include <iostream>
#include <vector>
#include <stack>
#include <time.h>
#include <chrono> 
#include <stdio.h>
#include <unistd.h>
 
#include "Tree.h"


// CHICOS UNANSE AL ZOOM
// ZOOM: https://utec.zoom.us/j/815711285

int main() {
  typedef BSTNode<float> bst_node;
  typedef BNode<int> b_node;
  typedef Tree<bst_node> bst_tree;
  typedef Tree<b_node> b_tree;
  bst_tree bsttree(4); 
  bsttree.insert(30);
  bsttree.insert(10);
  bsttree.insert(60);

  cout << bsttree << endl;
  b_tree tree(3);
  tree.insert(16);
  tree.insert(5);
  tree.insert(6);
  tree.insert(10);
  tree.insert(27);
  tree.insert(30);
  tree.insert(1);
  tree.insert(22);
  tree.insert(13);
  tree.insert(14);
  tree.insert(4);
  tree.insert(3);
  tree.insert(12);
  cout<<tree<<endl;
  
}

// https://repl.it/login?goto=/join/znxxkwam-carlosarias3
