#ifndef BNODE_H
#define BNODE_H

#include <iostream>
#include <vector>
#include "Macros.h"
using namespace std;

template<typename T>
class BNode {
  public: 
  typedef T value_t ;
  typedef vector<value_t> container_t;
  typedef vector<BNode*> pcontainer_t;

  unsigned int order;
  container_t data;
  //BNode** children;
  pcontainer_t children;
  size_t sizeOfContainer;

  BNode( unsigned int order = 4)
  :order(order){
    //children = new BNode*[B_NODE_ORDER];
    children.resize(order+1,nullptr);
    data.resize(order, 0);
    sizeOfContainer = 0;
  }
  
  void insertToContainer(value_t value){
    std::cout<<"Insert to Container\n";
    int index = sizeOfContainer - 1;

    while(index >= 0 && data[index] > value){
      data[index+1] = data[index];
      index--;
    }

    data[index+1] = value;
    sizeOfContainer++;
  }

  bool isContainerFull(){
    return sizeOfContainer == order;
  }

  void insert(value_t value, unsigned int order){
    std::cout<<"Insert "<<value<<"\n";
    // Si el children[0] es nullptr, es una leaf 
    if (!children[0]) {
      insertToContainer(value);
      return;
    }
    
    // Obtiene el hijo al que debe ir para insertar
    BNode* child = getNextChild(value);
    child->insert(value,order);
    
    if (child->isContainerFull()) {
      splitChild(child,order);
    }
  }

  BNode* getNextChild(value_t value){
    for(int i = 0; i < sizeOfContainer; i++){
      if (value <= data[i]) {
        return children[i];
      }
    }
    return children[sizeOfContainer];
  }

  void insertChildren(BNode* newNode){
    std::cout<<"Insert children\n";
    int index= order-1;
     while(index >= 0){
      if(!children[index]){
        index--;
        continue;
      }
      if(children[index]->data[0] <= newNode->data[0])
        break;
      children[index+1] = children[index];
      index--;
    }

    children[index+1] = newNode;
    std::cout<<"FInish children\n";
  }

  void splitChild(BNode* child, unsigned int order){
    std::cout<<"Spli child\n";
    BNode* newNode = new BNode(order);
    int m=order-(order/2)-1;
    int j=0;
    for(int i=m+1;i<order;i++){
      newNode->data[j]=child->data[i];
      newNode->sizeOfContainer++;
      child->data[i]=0;
      child->sizeOfContainer--;
      newNode->children[j]=child->children[i];
      child->children[i]=nullptr;
      j++;
    }
    newNode->children[j]=child->children[order];
    child->children[order]=nullptr;
    child->sizeOfContainer--;
    insertToContainer(child->data[m]);
    child->data[m]=0;
    newNode->printValues();
    std::cout<<std::endl;
    insertChildren(newNode);
  }

  void printNodes(){
    printValues();
    std::cout<<std::endl;
    printChilds();
  }

  void printValues(){
    for(int i=0;i<sizeOfContainer;i++)
      std::cout<<data[i]<<" ";
  }

  void printChilds(){
    for(int i =0;i<children.size();i++){
      if(!children[i]){
        std::cout<<"null\n";
        break;
      }
      children[i]->printNodes();
    }
  }

  
  ~BNode(void){}
};


#endif
