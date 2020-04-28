#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>


template <typename T>
class SS_Traits{
public:
    typedef T  value_t;
    typedef std::vector<value_t> container_t;
    typedef typename container_t::iterator iterator_t;

    class simple_search{
    public:
        bool operator() (container_t a, value_t v,int &pos, int size) {
            for(int i=0; i < size;i++){
                pos=i;
                if(i==size-1 || v<a[i])
                    return false;
                if(a[i]== v)
                    return true;
            }
            return false;
        }
    };
    typedef simple_search functor_t;
};

template <typename T>
class BS_Traits{
public:
    typedef T  value_t;
    typedef std::vector<value_t> container_t;
    typedef typename container_t::iterator iterator_t;

    class binary_search{
    public:
        bool operator() (container_t a, value_t v,int &pos,int size)  {
            int i=binS(a,0,size-2,v,pos);
            if(i!=-1){
                pos=i;
                return true;
            }
            return false;
        }
        int binS(container_t a,int l, int r, value_t v,int &pos){
            pos=l;
            if (r >= l) {
                int m = l + (r - l) / 2;
                if (a[m] == v)
                    return m;
                if (a[m] > v)
                    return binS(a, l, m - 1, v,pos);
                return binS(a, m + 1, r, v,pos);
            }
            return -1;
        }
    };
    typedef binary_search functor_t;
};

template <typename T, int S>
class BNode {
public:
    typedef typename T::value_t value_t;
    typedef typename T::container_t container_t;
    typedef std::vector<BNode*> pcontainer_t;
    typedef typename T::functor_t functor_t;

    BNode* parent;
    container_t keys;
    pcontainer_t ptrs;
    std::size_t  order;
    std::size_t actualSize;
    bool leaf,empty,isRoot;
    functor_t search;

    BNode(bool isLeaf):order(S){
        keys=container_t(order,0);
        ptrs=pcontainer_t(order+1,nullptr);
        actualSize=0;
        leaf=isLeaf;
        empty=true;
        isRoot=false;
    }

    bool searchValueInNode(value_t v){
      int pos=0;
      if(search(keys,v,pos,actualSize+1))
        return true;
      if(leaf)
        return false;
      return ptrs[pos]->searchValueInNode(v);
    }

    void insert(value_t v){
        BNode* insertNode=searchNode(v);
        insertNode->insertToContainer_t(v);
        if(insertNode->actualSize==order)
            insertNode->split();
    }

    void insertToContainer_t(value_t v){
        if(empty){
            empty=false;
            keys[0]=v;
            actualSize++;
            return;
        }
        int i=0;
        for(i;i<actualSize;i++){
            if(v<keys[i])
                break;
        }

        for(int j=actualSize-1;j>=i;j--)
            keys[j+1]=keys[j];

        keys[i]=v;
        actualSize++;
    }

    BNode* searchNode(value_t v){
        int pos=0;
        search(keys,v,pos,actualSize+1);
        if(leaf){
            return this;
        }
        return ptrs[pos]->searchNode(v);
    }

    void split(){
        if(isRoot)
            splitRoot();
        else if(leaf)
            splitLeaf();
        else
            splitNoLeaf();
    }

    void splitLeaf(){
        int m=actualSize-1-actualSize/2;
        BNode* newNode = new BNode(true);
        int j=0;
        for(int i=m+1; i<order;i++){
            newNode->keys[j]=this->keys[i];
            newNode->actualSize++;
            this->keys[i]=0;
            j++;
        }
        newNode->empty=false;
        actualSize=m;
        newNode->parent=this->parent;
        parent->splitParent(this->keys[m],newNode);
    }

    void splitParent(value_t v, BNode* child){
        insertToContainer_t(v);
        int pos=0;
        search(keys,child->keys[0],pos,actualSize+1);
        for(int i=order;i>pos;i--){
           if(ptrs[i]!= nullptr && ptrs[i-1]->keys[0]<child->keys[0] && pos==i-1) {
                pos++;
                break;
            }
            this->ptrs[i]=this->ptrs[i-1];
        }
        this->ptrs[pos]=child;
        if(actualSize==order)
            if(parent)
                splitNoLeaf();
            else
                splitRoot();
    }

    void splitNoLeaf(){
        int m=order-1-order/2;
        BNode* newNode = new BNode(false);
        int j=0;
        for(int i=m+1;i<order;i++){
            newNode->keys[j]=this->keys[i];
            newNode->actualSize++;
            newNode->ptrs[j]=this->ptrs[i];
            this->ptrs[i]=nullptr;
            this->keys[i]=0;
            j++;
        }
        newNode->empty=false;
        newNode->ptrs[j]=this->ptrs[order];
        actualSize=m;
        newNode->parent=this->parent;
        parent->splitParent(this->keys[m],newNode);

    }

    void splitRoot(){
        int m= order-1-order/2;
        BNode* newNode= new BNode(false);
        newNode->isRoot=true;
        newNode->insertToContainer_t(this->keys[m]);
        newNode->ptrs[0]=this;
        BNode* right= new BNode(false);
        newNode->ptrs[1]=right;
        this->parent=right->parent=newNode;
        int j=0;
        for(int i=m+1;i<order;i++){
            right->keys[j]=this->keys[i];
            this->keys[i]=0;
            right->actualSize++;
            j++;
        }
        right->empty=false;
        newNode->empty=false;
        this->actualSize=m;
        j=0;
        for(int i=m+1;i <ptrs.size();i++){
            right->ptrs[j]=this->ptrs[i];
            if(right->ptrs[j]!=nullptr)
                right->ptrs[j]->parent=right;
            this->ptrs[i]=nullptr;
            j++;
        }
        this->leaf=isLeaf();
        this->isRoot=false;
        right->leaf=right->isLeaf();
    }

    bool isLeaf(){
        if(ptrs[0]==nullptr)
            return true;
        return false;
    }

    void printNodes(){
      printValues();  
      std::cout<<std::endl;
      printChild();
    }

    void printValues(){
      for(int i=0;i<actualSize;i++)
        std::cout<<keys[i]<<" ";
    }

    void printChild(){
        for(int i=0; i<ptrs.size();i++){
            if(!ptrs[i]){
                //std::cout<<"nul\n";
                break;
            }
            ptrs[i]->printNodes();
        }
    }

    BNode* getRoot(){
        if(!parent)
            return this;
        return parent->getRoot();
    }

    ~BNode(void){}
};


template<typename N>
class PreOrder
{
  public:
  class pre_order_print
  {
    public:
    void operator()(N node){
      pre_order_print print;
      if(!node)
        return;
      node->printValues();
      for(auto x:node->ptrs)
        print(x);
    }
  };
    typedef pre_order_print print_t;
};

template<typename N>
class PostOrder
{
  public:
    class post_order_print{
      public:
        void operator()(N node){
          post_order_print print;
          if(!node)
            return;
          for(auto x:node->ptrs)
            print(x);
          node->printValues();
        }
    };
    typedef post_order_print print_t;
};


template <typename T,typename P ,int S>
class BTree {
public:
    typedef typename T::value_t value_t;

    typedef typename P::print_t print_t;

    BNode<T,S>* root;
    print_t print;


    BTree(void):root(NULL){
    }

    ~BTree(void){}

    void insert(const value_t& val){
        // TODO :: INSERT
        if(!root){
            root = new BNode<T,S>(true);
            root->isRoot=true;
        }
        root->insert(val);
        root=root->getRoot();
    }

    bool find(const value_t val= 0) {
        // TODO :: SEARCH
        if(!root)
            return false;
        return root->searchValueInNode(val);
    }

    friend std::ostream& operator<<(std::ostream& out, BTree<T,P,S> tree){
        //tree.print();// (out)
        // IN PRE POST LEVEL ORDER
        if(!tree.root)
          return out;
        tree.print(tree.root);
        return out;
    }
};


int main() {
    typedef BS_Traits<int> btrait_t;
    typedef PreOrder<BNode<btrait_t,4>*> print_t;
    BTree<btrait_t,print_t,4> tree;
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
    std::cout<<"Number find 16 response: "<<tree.find(16)<<std::endl;
    //tree.root->printNodes();
    std::cout<<"PreOrder\n";
    std::cout<<tree;
    std::cout<<std::endl;

    typedef SS_Traits<float> strait_t;
    typedef PostOrder<BNode<strait_t,7>*> print_t2;
    BTree<strait_t,print_t2,7> stree;
    stree.insert(16);
    stree.insert(5);
    stree.insert(6);
    stree.insert(10);
    stree.insert(27);
    stree.insert(30);
    stree.insert(1);
    stree.insert(22);
    stree.insert(13);
    stree.insert(14);
    stree.insert(4);
    stree.insert(3);
    stree.insert(12);
    std::cout<<"Number find 86 response: "<<stree.find(86)<<std::endl;
    //stree.root->printNodes();
    std::cout<<"PostOrder\n";
    std::cout<<stree;
    std::cout<<std::endl;
    
}


 
