//B+树方法实现

#include "BPLUSTREE.h"


template <typename KeyType>
int BPT <KeyType> ::find_index(BPT_NODE<KeyType>* parent, BPT_NODE<KeyType>* child) const {
    auto it = std::find(parent->children.begin(), parent->children.end(), child);
    return it - parent->children.begin();
}

template <typename KeyType>
BPT_NODE <KeyType> * BPT<KeyType>::search_leaf(BPT_NODE<KeyType>* node, const KeyType& key) const {
    while (! node -> is_leaf) {
        // 在内部节点中查找适当的子节点
        auto it = upper_bound(node->keys.begin(), node->keys.end(), key);
        int index = it - node->keys.begin();
        node = node->children[index];
    }
    return node;
}

template <typename KeyType>
void BPT <KeyType>::insertkey(const KeyType& key, dl value) {
    if (root == nullptr) {
        // 如果树为空，创建根节点
        root = new BPT_NODE<KeyType>(true);
        root->keys.push_back(key);
        root->values.push_back(value);
    } else {
        // 找到适当的叶节点
        BPT_NODE<KeyType>* leaf = search_leaf(root, key);
        
        // 在叶节点中插入
        insert_in_leaf(leaf, key, value);
    }
}

template <typename KeyType>
void BPT<KeyType>::insert_notfull(BPT_NODE<KeyType>* node, const KeyType& key, dl value){
    //先判断是否是叶节点
    if(node->is_leaf){
        //在叶节点插入
        //判断是否满了，满了就先分裂
        if(node -> keys.size() == MAX_NODE_SIZE - 1){
            splitchild(node -> parent, find_index(node -> parent, node));
            //分裂完后判断插入的键值在左边还是右边
            int index = find_index(node -> parent, node);
            if(key > node -> parent -> keys[index])
                node = node -> parent -> children[index + 1];
        }

        //插入键值对
        auto it = lower_bound(node -> keys.begin(), node -> keys.end(), key);
        int index = it - node -> keys.begin();
        node -> keys.insert(it, key);
        node -> values.insert(node -> values.begin() + index, value);
    }
    else{
        //在内部节点中插入
        auto it = upper_bound(node -> keys.begin(), node -> keys.end(), key);
        int index = it - node -> keys.begin();
        //分裂
    }

}