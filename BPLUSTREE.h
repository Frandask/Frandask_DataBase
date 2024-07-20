//B+树定义

#include "BPLUSTREE_NODE.h"

template <typename KeyType>
class BPT{
    private:
        BPT_NODE<KeyType> * root;
        BPT_NODE<KeyType>* search_leaf(BPT_NODE<KeyType>* node, const KeyType& key) const;
        int find_index(BPT_NODE<KeyType>* parent, BPT_NODE<KeyType>* child) const;
        void insert_notfull(BPT_NODE<KeyType> * node, const KeyType& key, dl value);    //节点的插入(非满)
        void splitchild(BPT_NODE<KeyType> * parent, int index);     //节点的分裂(满)
        void mergechild(BPT_NODE<KeyType> * me);     //节点合并
        BPT_NODE <KeyType> * search(BPT_NODE<KeyType> * node, const KeyType & key) const;   //寻找节点位置
    public:
        BPT();
        ~BPT();
        void insertkey(const KeyType& key, dl value);
        BPT_NODE<KeyType> * search(const KeyType & key) const;
        void traveltree() const;
        void deletekey(const KeyType& key);
};