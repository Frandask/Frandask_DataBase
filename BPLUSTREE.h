//B+树定义
#ifndef BPLUSTREE_H
#define BPLUSTREE_H


#include "BPLUSTREE_NODE.h"
template <typename KeyType>
class BPT{
    private:

        tree_infos tree_info;
        
        dl search_leaf(dl node_dl, const KeyType& key) const;
        
        
        dl find_index(dl node_dl, const KeyType& key) const;
        void insert_notfull(dl node_dl, const KeyType& key, dl value);    //节点的插入(非满)
        void split_leaf_node(dl node_dl);
        void split_internal_node(dl node_dl);
        void splitnode(dl node_dl);     //节点的分裂(满)
        void mergechild(dl me_);     //节点合并
        dl search(dl node_dl, const KeyType & key) const;   //寻找节点位置
        void read_node(dl node_dl, BPT_NODE<KeyType>& node) const ;
        void write_node(dl node_dl, const BPT_NODE<KeyType>& node) const;
    public:
        BPT();
        ~BPT();
        void initialize_tree(); //从文件中读取并初始化树信息
        void save_tree();       //将树信息保存到文件中
        void insertkey(const KeyType& key, dl value);
        dl search(const KeyType & key) const;
        void traveltree() const;
        void deletekey(const KeyType& key);
};

#endif