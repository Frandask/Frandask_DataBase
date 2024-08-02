//B+树定义
#ifndef BPLUSTREE_H
#define BPLUSTREE_H


#include "BPLUSTREE_NODE.h"
template <typename KeyType>
class BPT{
    private:
        
        tree_infos tree_info;
        //文件操作部分
        void read_node(dl node_dl, BPT_NODE<KeyType>& node) const ;
        void write_node(dl node_dl, const BPT_NODE<KeyType>& node) const;

        //辅助函数部分
        dl search_leaf(dl node_dl, const KeyType& key) const;   //寻找叶结点的位置
        int find_index(dl node_dl, const KeyType& key) const;    //寻找在叶结点中键值的位置
        void reset_parent(dl node_dl); //内部节点分裂时重新设置子节点的父节点
        dl find_brother(dl node_dl);
        //插入部分
        void insert_notfull(dl node_dl, const KeyType& key, dl value);    //节点的插入

        void splitnode(dl node_dl);     //节点的分裂
        void split_leaf_node(dl node_dl);
        void split_internal_node(dl node_dl);
        

        //删除部分
        void delete_notleast(dl node_dl, const KeyType& key); //节点键值的删除
        void borrow_leaf(dl node_dl, dl brother_dl);    //叶节点向兄弟节点借
        void merge_leaf(dl node_dl, dl brother_dl);
        dl search(dl node_dl, const KeyType & key) const;   //寻找节点位置

        
        
    public:

        BPT();
        ~BPT();

        //B+树信息
        void initialize_tree();
        void save_tree();

        //公用插入删除函数
        void insertkey(const KeyType& key, dl value);
        void deletekey(const KeyType& key);

        //遍历打印树
        void traverse() const;  //层序遍历
        void travelleaf() const;    //遍历叶节点
};

#endif