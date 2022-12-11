#ifndef OURMAP_H
#define OURMAP_H

#include <ostream>
#include "error.h"
#include "strlib.h"
#include "vector.h"
using namespace std;

template <typename KeyType,typename ValueType>
class OurMap{
public:
    OurMap();
    ~OurMap();

    OurMap(const OurMap& map2);
    OurMap& operator=(const OurMap& map2);

    void clear();
    int size() const;
    bool isEmpty() const;

    void put(const KeyType& key,const ValueType& value);
    bool containsKey(const KeyType& key)const;
    void remove(const KeyType& key);

    KeyType firstKey() const;
    KeyType lastKey() const;

    Vector<KeyType> keys() const;
    Vector<ValueType> values() const;

    ValueType& operator[](const KeyType& key);

private:
    struct TreeNode{
        KeyType key;
        ValueType value;
        TreeNode* left;
        TreeNode* right;
    };

    TreeNode* root;
    int numItems;

    void freeTree(TreeNode* curNode);
    /* 为什么需要用Helper函数？ 因为需要递归传递参数；
     * 为什么需要递归（而不是循环）？ 因为树是扩散而非线性的； */
    ValueType& getHelper(TreeNode*& curNode,const KeyType& key);
    void putHelper(TreeNode*& curNode,const KeyType& key,const ValueType& value);
    Vector<KeyType> keysHelper(TreeNode* curNode) const;
    Vector<ValueType> valuesHelper(TreeNode* curNode) const;
    bool containsKeyHelper(TreeNode* curNode,const KeyType& key) const;
    void removeHelper(TreeNode*& curNode,const KeyType& key);
    void performDeletion(TreeNode*& curNode);
    void copyTree(TreeNode*& curNode,TreeNode* copyNode);
};

template <typename KeyType,typename ValueType>
OurMap<KeyType,ValueType>::OurMap(){
    root=nullptr;
    numItems=0;
}

template <typename KeyType,typename ValueType>
OurMap<KeyType,ValueType>::~OurMap(){
    freeTree(root);
}

template <typename KeyType,typename ValueType>
OurMap<KeyType,ValueType>::OurMap(const OurMap& map2){
    copyTree(root,map2.root);
}

template <typename KeyType,typename ValueType>
struct OurMap<KeyType,ValueType>::OurMap& OurMap<KeyType,ValueType>::operator=(const OurMap& map2){
    clear();
    copyTree(root,map2.root);
    return *this;
}

template <typename KeyType,typename ValueType>
void OurMap<KeyType,ValueType>::copyTree(TreeNode*& curNode,TreeNode* copyNode){
    if (copyNode!=nullptr){
        curNode=new TreeNode{copyNode->key,copyNode->value,nullptr,nullptr};
        copyTree(curNode->left,copyNode->left);
        copyTree(curNode->right,copyNode->right);
    }
}

template <typename KeyType,typename ValueType>
void OurMap<KeyType,ValueType>::freeTree(TreeNode* curNode){
    if (curNode==nullptr){
        return ;
    }
    else{
        TreeNode* tmp=curNode;
        freeTree(curNode->left);
        freeTree(curNode->right);
        delete tmp;
    }
}

template <typename KeyType,typename ValueType>
void OurMap<KeyType,ValueType>::clear(){
    freeTree(root);
    root=nullptr;
    numItems=0;
}

template <typename KeyType,typename ValueType>
int OurMap<KeyType,ValueType>::size() const{
    return numItems;
}

template <typename KeyType,typename ValueType>
bool OurMap<KeyType,ValueType>::isEmpty() const{
    return (numItems==0);
}

template <typename KeyType,typename ValueType>
Vector<KeyType> OurMap<KeyType,ValueType>::keys() const{
    return keysHelper(root);
}

template <typename KeyType,typename ValueType>
Vector<KeyType> OurMap<KeyType,ValueType>::keysHelper(TreeNode* curNode) const{
    if (curNode==nullptr){
        return {};
    }
    else{
        return keysHelper(curNode->left)+curNode->key+keysHelper(curNode->right);
    }
}

template <typename KeyType,typename ValueType>
Vector<ValueType> OurMap<KeyType,ValueType>::values() const{
    return valuesHelper(root);
}

template <typename KeyType,typename ValueType>
Vector<ValueType> OurMap<KeyType,ValueType>::valuesHelper(TreeNode* curNode) const{
    if (curNode==nullptr){
        return {};
    }
    else{
        return valuesHelper(curNode->left)+curNode->value+valuesHelper(curNode->right);
    }
}

template <typename KeyType,typename ValueType>
bool OurMap<KeyType,ValueType>::containsKey(const KeyType& key)const{
    return containsKeyHelper(root,key);
}

template <typename KeyType,typename ValueType>
bool OurMap<KeyType,ValueType>::containsKeyHelper(TreeNode* curNode,const KeyType& key) const{
    if (curNode==nullptr){      /* 因为后面需要访问curNode里面的对象，所以先要判断是否为nullptr */
        return false;
    }
    else if (key>curNode->key){
        return containsKeyHelper(curNode->right,key);
    }
    else if(key<curNode->key){
        return containsKeyHelper(curNode->left,key);
    }
    else{
        return true;
    }
}

template <typename KeyType,typename ValueType>
KeyType OurMap<KeyType,ValueType>::firstKey() const{
    TreeNode* tmp=root;
    if (tmp!=nullptr){
        while (tmp->left!=nullptr){
            tmp=tmp->left;
        }
        return tmp->key;
    }
    else{
        return KeyType();
    }
}

template <typename KeyType,typename ValueType>
KeyType OurMap<KeyType,ValueType>::lastKey() const{
    TreeNode* tmp=root;
    if (tmp!=nullptr){
        while (tmp->right!=nullptr){
            tmp=tmp->right;
        }
        return tmp->key;
    }
    else{
        return KeyType();
    }
}

template <typename KeyType,typename ValueType>
ValueType& OurMap<KeyType,ValueType>::operator[](const KeyType& key){
    if (!containsKey(key)){
        put(key,ValueType());   //必须要创建key，这样才能通过返回引用来接受新的值(ValueType)
    }
    return getHelper(root,key);
}

template <typename KeyType,typename ValueType>
ValueType& OurMap<KeyType,ValueType>::getHelper(TreeNode*& curNode,const KeyType& key){
    if(key>curNode->key){
        return getHelper(curNode->right,key);
    }
    else if(key<curNode->key){
        return getHelper(curNode->left,key);
    }
    else{
        return curNode->value;
    }
}

template <typename KeyType,typename ValueType>
void OurMap<KeyType,ValueType>::put(const KeyType& key,const ValueType& value){
    putHelper(root,key,value);
}

template <typename KeyType,typename ValueType>
void OurMap<KeyType,ValueType>::putHelper(TreeNode*& curNode,const KeyType& key,const ValueType& value){
    if (curNode==nullptr){
        curNode=new TreeNode{key,value,nullptr,nullptr};
        numItems++;
    }
    else if (curNode->key==key){
        curNode->value=value;
    }
    else if(key>curNode->key){
        putHelper(curNode->right,key,value);
    }
    else{
        putHelper(curNode->left,key,value);
    }
}

template <typename KeyType,typename ValueType>
void OurMap<KeyType,ValueType>::remove(const KeyType& key){
    if (containsKey(key)){
        removeHelper(root,key);
    }
    else{
        error("remove: the key does not exist");
    }
}

template <typename KeyType,typename ValueType>
void OurMap<KeyType,ValueType>::removeHelper(TreeNode*& curNode,const KeyType& key){
    if (key>curNode->key){
        removeHelper(curNode->right,key);
    }
    else if (key<curNode->key){
        removeHelper(curNode->left,key);
    }
    else{
            performDeletion(curNode);
            numItems--;
    }
}

template <typename KeyType,typename ValueType>
void OurMap<KeyType,ValueType>::performDeletion(TreeNode*& curNode){
    if (curNode->left==nullptr && curNode->right==nullptr){
        delete curNode;
        curNode=nullptr;
    }
    else if (curNode->left==nullptr){
        TreeNode* tmp=curNode;
        curNode=curNode->right;
        delete tmp;                /* 如果不传入引用，这里就无法修改curNode的值 */
    }
    else if (curNode->right==nullptr){
        TreeNode* tmp=curNode;
        curNode=curNode->left;
        delete tmp;
    }
    else{
    TreeNode* tmp=curNode->left;
    while (tmp->right!=nullptr){
        tmp=tmp->right;
    }
    curNode->value=tmp->value;
    curNode->key=tmp->key;
    delete tmp;
    }
}

#endif // OURMAP_H
