#ifndef TREE_HELPER_H
#define TREE_HELPER_H

#include <iostream>
#include "BNode.h"
#include "BPlusNode.h"
#include "BSTNode.h"
using namespace std;

template<typename  Node, typename  ValueNode, int Flag>
struct TreeHelper{
  typedef Node  node_t;
  typedef typename node_t::value_t value_t;
  
  static void  insert (node_t** head, const value_t& val, unsigned int order){
    cout << "el tipo de dato no es compatible" << endl;
  }

  static void  print (node_t** head, std::ostream &out){
  }
};


template<typename  ValueNode>
struct TreeHelper<BSTNode<ValueNode>,ValueNode,BST_NODE_FLAGXX>{
  typedef BSTNode<ValueNode>  node_t;
  typedef typename node_t::value_t value_t;
  
  static void  insert (node_t** head, const value_t& val, unsigned int order){
    auto temp = new node_t(val);
    
    if ((*head)== nullptr) {
      (*head) = temp;     
      return;
    }
    
    if ((*head)->data > val){
     insert(&(*head)->children[0], val, order);
     return;
    }

    if ((*head)->data < val){
      insert(&(*head)->children[1], val, order);
      return;
    }
    cout << "Duplicates not allowed" << endl;
  }

  //BST Print
  static void  print (node_t** head, std::ostream &out){
    std::stack<node_t*> nodes;
    node_t *curr = *head;
    
    while(curr) {
      out << "Node: " << curr->data << endl;
      if (curr->children[1]) {
        nodes.push(curr->children[1]);
      }
      if (curr->children[0]) {
        curr = curr->children[0];
      } else {
        if (!nodes.empty()) {
          curr = nodes.top(); 
          nodes.pop();
        } else {
          curr = nullptr;
        }
      } 
    }
  }
};

template<typename ValueNode>
struct TreeHelper<BNode<ValueNode>,ValueNode,B_NODE_FLAGXX>{
  typedef BNode<ValueNode>  node_t;
  typedef typename node_t::value_t value_t;
  
  static void  insert (node_t** head, const value_t& val, unsigned int order){
    cout << "Insertando para un nodo B" << endl;
    if(!(*head)){
      (*head)=new node_t(order);
      (*head)->data[0]=val;
      (*head)->sizeOfContainer++;
      return;
    }
    (*head)->insert(val,order);
    std::cout<<"size: "<<(*head)->sizeOfContainer<<"\n";
    if((*head)->isContainerFull())
      splitRoot(head,order);
  }

  static void splitRoot(node_t** head,unsigned int order){
    std::cout<<"Root split\n";
    auto Left= new node_t(order);
    auto Right = new node_t(order);
    int m= order-(order/2)-1;
    //Guardar la data necesaria para el nuevo head
    value_t temp = (*head)->data[m];
    auto child = (*head)->children[m];
    //Pasar los datos a Right y Left y limpiar el head
    int j=0,k=0;
    for(int i=0;i<order;i++){
      if(i<m){
        Left->data[j]=(*head)->data[i];
        Left->sizeOfContainer++;
        Left->children[j]=(*head)->children[i];
        j++;
      }else if(i>m){
        Right->data[k]=(*head)->data[i];
        Right->sizeOfContainer++;
        Right->children[k]=(*head)->children[i];
        k++;
      }
      (*head)->data[i]=0;
      (*head)->children[i]=nullptr;
    }
    //Pasar la data que falta
    Left->children[m]=child;
    Right->children[k]=(*head)->children[order];
    (*head)->children[order]=nullptr;
    //Inicializar la data nueva al head
    (*head)->sizeOfContainer=1;
    (*head)->data[0]=temp;
    (*head)->children[0]=Left;
    (*head)->children[1]=Right;
  }

  static void  print (node_t** head, std::ostream &out){
    (*head)->printNodes();
  }
};

#endif
