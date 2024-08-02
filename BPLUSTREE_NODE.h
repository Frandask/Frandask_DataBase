//B+树节点定义
#ifndef BPLUSTREE_NODE_H
#define BPLUSTREE_NODE_H

#include "OTHER_DEFINE.h"
#pragma pack(push, 4) 
template <typename KeyType>
class BPT_NODE{
    public:
        bool is_leaf;   //该节点是否为叶节点
        dl parent;      //父节点
        KeyType children[MAX_NODE_SIZE + 3];  //子节点
        dl next;    //下个节点
        KeyType keys[MAX_NODE_SIZE + 3];  //节点的键值
        dl values[MAX_NODE_SIZE + 3];  //叶结点中存放数据在数据文件中的位置
        int node_size;
        //构造函数
        BPT_NODE(bool is_leaf_ = false, dl parent_ = -1, dl next_ =-1, int node_size_ = 0){
            is_leaf = is_leaf_;
            parent = parent_;
            next = next_;
            node_size = node_size_;
            memset(keys, 0, sizeof(keys));
            memset(values, 0, sizeof(values));
            memset(children, 0, sizeof(children));
        }

};

#pragma pack(pop)
#endif