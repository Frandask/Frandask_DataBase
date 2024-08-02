//B+树方法实现

#include "BPLUSTREE.h"
string file_name = "test.bin";



//构造函数 析构函数
template <typename KeyType>
BPT<KeyType>::BPT() {
    initialize_tree();
}

template <typename KeyType>
BPT<KeyType>::~BPT() {
    save_tree();
}


//文件操作

//读取节点
template <typename KeyType>
void BPT<KeyType>::read_node(dl node_dl, BPT_NODE<KeyType>& node) const {
    std::ifstream meta_file(file_name, std::ios::binary | std::ios::in);
    if (!meta_file.is_open()) {
        std::cerr << "Error opening file for reading" << std::endl;
        return;
    }

    meta_file.seekg(node_dl, std::ios::beg);
    if (!meta_file) {
        std::cerr << "Error seeking to position in file" << std::endl;
        return;
    }

    meta_file.read(reinterpret_cast<char*>(&node), sizeof(BPT_NODE<KeyType>));
    if (!meta_file) {
        std::cerr << "Error reading node from file" << std::endl;
    }

    meta_file.close();
}

//写入节点
template <typename KeyType>
void BPT<KeyType>::write_node(dl node_dl, const BPT_NODE<KeyType>& node) const {
    // Open file for both reading and writing in binary mode
    FILE *file = std::fopen(file_name.c_str(), "r+b");
    if (!file) {
        std::cerr << "Error opening file for writing" << std::endl;
        return;
    }

    // Seek to the correct position in the file
    if (std::fseek(file, node_dl, SEEK_SET) != 0) {
        std::cerr << "Error seeking to position in file" << std::endl;
        std::fclose(file);
        return;
    }

    // Write data to the file
    if (std::fwrite(&node, sizeof(BPT_NODE<KeyType>), 1, file) != 1) {
        std::cerr << "Error writing node to file" << std::endl;
    }

    std::fclose(file);
}

//树的初始化
template <typename KeyType>
void BPT<KeyType>::initialize_tree(){
    ifstream meta_file(file_name, std::ios::binary);
    if (!meta_file) {
        std::cerr << "Error opening metadata file for reading" << std::endl;
        return;
    }
    meta_file.seekg(0, ios::beg);
    meta_file.read(reinterpret_cast<char*>(&tree_info), sizeof(tree_infos));
    if (tree_info.slot == 0)
        tree_info.slot = START_DL;
    meta_file.close();

}

//保存B+树
template <typename KeyType>
void BPT<KeyType>::save_tree(){
    std::ofstream meta_file(file_name, std::ios::in | std::ios::out);
    if (!meta_file) {
        std::cerr << "Error opening metadata file for writing" << std::endl;
        return;
    }
    meta_file.seekp(0, ios::beg);
    meta_file.write(reinterpret_cast<const char*>(&tree_info), sizeof(TREEINFOS));
    meta_file.close();

    std::cout << "Tree metadata saved successfully." << std::endl;    
}


//辅助函数部分

//寻找叶节点的位置
template <typename KeyType>
dl BPT<KeyType>::search_leaf(dl node_dl, const KeyType& key) const{
    BPT_NODE<KeyType> node;
    read_node(node_dl, node);
    if (node.is_leaf){
        return node_dl;
    }
    else{
        int i = upper_bound(node.keys, node.keys + node.node_size, key) - node.keys;
        return search_leaf(node.children[i], key);
    }
}

//寻找在叶结点中键值的位置
template <typename KeyType>
int BPT<KeyType>::find_index(dl node_dl, const KeyType& key) const{
    BPT_NODE<KeyType> node;
    read_node(node_dl, node);
    int i = upper_bound(node.keys, node.keys + node.node_size, key) - node.keys;
    return i;
}

//内部节点分裂时重新设置子节点的父节点
template <typename KeyType>
void BPT<KeyType>::reset_parent(dl node_dl){
    BPT_NODE<KeyType> node;
    read_node(node_dl, node);
    BPT_NODE<KeyType> children_node;
    for(int i = 0; i <= node.node_size; ++i){
        read_node(node.children[i], children_node);
        children_node.parent = node_dl;
        write_node(node.children[i], children_node);
    }

}

template <typename KeyType>
dl BPT<KeyType>::find_brother(dl node_dl){
    BPT_NODE<KeyType> node, parent_node, left_brother, right_brother;
    read_node(node_dl, node);
    read_node(node_dl, parent_node);

    auto it = find(begin(parent_node.children), parent_node.children + parent_node.node_size + 1, node_dl);
    int index = it - parent_node.children;
    if(index >= 1)
        read_node(parent_node.children[index - 1], left_brother);
    if(index <= parent_node.node_size - 1)
        read_node(parent_node.children[index + 1], right_brother);
    if(left_brother.node_size >= right_brother.node_size){
        node = left_brother;
        node_dl = parent_node.children[index - 1];
    }
    else{
        node = right_brother;
        node_dl = parent_node.children[index + 1];
    }
    if(node.node_size > MERGE_LOC)
        return node_dl;
    else
        return 0;


}


//插入操作

//公用插入函数
template <typename KeyType>
void BPT<KeyType>::insertkey(const KeyType& key, dl value){
    //如果树空，创造根节点
    if (tree_info.root_dl == 0){
        BPT_NODE<KeyType> new_root(true);
        new_root.keys[0] = key;
        new_root.values[0] = value;
        new_root.node_size = 1;
        new_root.parent = -1;
        new_root.next = -1;
        tree_info.root_dl = tree_info.slot;
        write_node(tree_info.root_dl, new_root);
        tree_info.slot += sizeof(BPT_NODE<KeyType>);
        save_tree();
        return;
    }
    else{
        dl aim_dl = search_leaf(tree_info.root_dl, key);
        insert_notfull(aim_dl, key, value);
    }
}


//插入非满节点，插入后满了分裂
template <typename KeyType>
void BPT<KeyType>::insert_notfull(dl node_dl, const KeyType& key, dl value){
    BPT_NODE<KeyType> node;
    read_node(node_dl, node);
    if(node.is_leaf){
        int index = find_index(node_dl, key);

        if(node.node_size > 0){
            std::move_backward(node.keys + index, node.keys + node.node_size, node.keys + node.node_size + 1);
            std::move_backward(node.values + index, node.values + node.node_size, node.values + node.node_size + 1);
        }
        node.keys[index] = key;
        node.values[index] = value;
        ++ node.node_size;
        write_node(node_dl, node);
        if (node.node_size == MAX_NODE_SIZE){
            splitnode(node_dl);
        }
    }
    else{
        int index = find_index(node_dl, key);

        if(node.node_size > 0){
            std::move_backward(node.keys + index, node.keys + node.node_size, node.keys + node.node_size + 1);
            std::move_backward(node.children + index, node.children + node.node_size + 1, node.children + node.node_size + 2);
        }
        node.keys[index] = key;
        node.children[index] = value;
        BPT_NODE<KeyType> children_node;
        read_node(node.children[index], children_node);
        node.children[index + 1] = children_node.next;
        ++ node.node_size;
        write_node(node_dl, node);
        if (node.node_size == MAX_NODE_SIZE - 1){
            splitnode(node_dl);
        }        
    }
}

//节点分裂
template <typename KeyType>
void BPT<KeyType>::splitnode(dl node_dl){
    BPT_NODE<KeyType> node;
    read_node(node_dl, node);
    //叶节点的分裂
    if(node.is_leaf){
        split_leaf_node(node_dl);
    }
    else{
        split_internal_node(node_dl);
    }
}

// 分裂叶子节点
template <typename KeyType>
void BPT<KeyType>::split_leaf_node(dl node_dl) {
    BPT_NODE<KeyType> node;
    read_node(node_dl, node);

    BPT_NODE<KeyType> new_node(true);
    int mid = SPLIT_LOC;

    // 复制到新节点
    std::move(node.keys + mid, node.keys + node.node_size, new_node.keys);
    std::move(node.values + mid, node.values + node.node_size, new_node.values);
    new_node.parent = node.parent;
    new_node.next = node.next;
    new_node.node_size = node.node_size - mid;
    node.node_size = mid;

    // 更新当前节点
    dl new_node_dl = tree_info.slot;
    node.next = new_node_dl;
    tree_info.slot += sizeof(BPT_NODE<KeyType>);
    

    // 更新父节点
    if (node_dl == tree_info.root_dl) {
        // 如果是根节点，则需要创建新的根节点
        BPT_NODE<KeyType> new_root(false);
        new_root.keys[0] = node.keys[node.node_size - 1];
        new_root.children[0] = node_dl;
        new_root.children[1] = new_node_dl;
        new_root.node_size = 1;

        tree_info.root_dl = tree_info.slot;
        tree_info.slot += sizeof(BPT_NODE<KeyType>);
        
        node.parent = tree_info.root_dl;
        new_node.parent = tree_info.root_dl;
        write_node(node_dl, node);
        write_node(new_node_dl, new_node);
        write_node(tree_info.root_dl, new_root);
        save_tree();
    } else {
        // 否则，插入新键值到父节点
        write_node(node_dl, node);
        write_node(new_node_dl, new_node);
        insert_notfull(node.parent, node.keys[node.node_size - 1], node_dl);
    }
}


//分裂内部节点
template <typename KeyType>
void BPT<KeyType>::split_internal_node(dl node_dl) {
    BPT_NODE<KeyType> node;
    read_node(node_dl, node);

    BPT_NODE<KeyType> new_node(false);
    int mid = SPLIT_LOC;

    //内部节点children会多一个
    std::move(node.keys + mid, node.keys + node.node_size, new_node.keys);
    std::move(node.children + mid, node.children + node.node_size + 1, new_node.children);

    new_node.node_size = node.node_size - mid;
    new_node.parent = node.parent;
    node.node_size = mid - 1;

    dl new_node_dl = tree_info.slot;
    tree_info.slot += sizeof(BPT_NODE<KeyType>);
    new_node.next = node.next;
    node.next = new_node_dl;
    
    
    //根节点
    if(node_dl == tree_info.root_dl){
        BPT_NODE <KeyType> new_root(false);
        new_root.keys[0] = node.keys[mid - 1];
        new_root.children[0] = node_dl;
        new_root.children[1] = new_node_dl;
        new_root.node_size = 1;

        tree_info.root_dl = tree_info.slot;
        tree_info.slot += sizeof(BPT_NODE<KeyType>);

        node.parent = tree_info.root_dl;
        new_node.parent = tree_info.root_dl;
        write_node(node_dl, node);
        write_node(new_node_dl, new_node);
        //调整一下分裂后的内部节点的子节点的父节点
        reset_parent(new_node_dl);
        write_node(tree_info.root_dl, new_root);
        save_tree();
    }else {
        //插入键值到父节点
        write_node(node_dl, node);
        write_node(new_node_dl, new_node);
        reset_parent(new_node_dl);
        insert_notfull(node.parent, node.keys[mid - 1], node_dl);
    }
    
    return;
}


//删除部分

//公用删除函数
template <typename KeyType>
void BPT<KeyType>::deletekey(const KeyType& key){
    dl aim_dl = search_leaf(tree_info.root_dl, key);
    delete_notleast(aim_dl, key);    
}

//删除节点
template <typename KeyType>
void BPT<KeyType>::delete_notleast(dl node_dl, const KeyType& key){
    BPT_NODE<KeyType> node;
    read_node(node_dl, node);

    if(node.is_leaf){
        int index = find_index(node_dl, key);
        std::move(node.children + index, node.children + node.node_size, node.children + index - 1);
        std::move(node.values + index, node.values + node.node_size, node.values + index - 1);

        if(node.node_size < MERGE_LOC){
            dl brother_dl = find_brother(node_dl);
            if(brother_dl)
                borrow_leaf(node_dl, brother_dl);
            else
                merge_leaf(node_dl, brother_dl);
        }
    }
}

//向兄弟节点借节点
template <typename KeyType>
void BPT<KeyType>::borrow_leaf(dl node_dl, dl brother_dl){
    BPT_NODE<KeyType> node, brother_node, parent_node;
    read_node(node_dl, node);
    read_node(brother_dl, brother_node);
    read_node(node.parent, parent_node);

    //向右边的兄弟节点借
    if(node.next == brother_dl){
        node.keys[node.node_size] = brother_node.keys[0];
        node.values[node.node_size] = brother_node.values[0];
        std::move(brother_node.keys + 1, brother_node.keys + brother_node.node_size, brother_node.keys);
        std::move(brother_node.values + 1, brother_node.values + brother_node.node_size, brother_node.values);
        ++ node.node_size;
        -- brother_node.node_size;

        int index = find_index(node.parent, node.keys[0]);
        parent_node.keys[index] = node.keys[node.node_size - 1];

        write_node(node_dl, node);
        write_node(brother_dl, brother_node);
        write_node(node.parent, parent_node);
    }
    //向左边的兄弟节点借
    else{
        std::move(node.keys, node.keys + node.node_size, node.keys + 1);
        std::move(node.values, node.values + node.node_size, node.values + 1);
        node.keys[0] = brother_node.keys[brother_node.node_size - 1];
        node.values[0] = brother_node.values[brother_node.node_size - 1];
        ++ node.node_size;
        -- brother_node.node_size;

        int index = find_index(node.parent, brother_node.keys[0]);
        parent_node.keys[index] = brother_node.keys[brother_node.node_size - 1];

        write_node(node_dl, node);
        write_node(brother_dl, brother_node);
        write_node(node.parent, parent_node);
    }
}

//叶节点合并
template <typename KeyType>
void BPT<KeyType>::merge_leaf(dl node_dl, dl brother_dl){

}
//层序遍历B+树
template <typename KeyType>
void BPT<KeyType>::traverse() const {
    if (tree_info.root_dl == 0) {
        std::cout << "The tree is empty." << std::endl;
        return;
    }

    queue<dl> node_queue;
    node_queue.push(tree_info.root_dl);

    while (!node_queue.empty()) {
        dl current_dl = node_queue.front();
        node_queue.pop();

        BPT_NODE<KeyType> node;
        read_node(current_dl, node);

        cout << "Node at address: " << current_dl << " [";
        for (int i = 0; i < node.node_size; ++i) {
            std::cout << node.keys[i];
            if (i < node.node_size - 1) {
                cout << ", ";
            }
        }
        cout << "]" << std::endl;

        if (!node.is_leaf) {
            for (int i = 0; i <= node.node_size; ++i) {
                node_queue.push(node.children[i]);
            }
        }
    }
}


//遍历叶节点
template <typename KeyType>
void BPT<KeyType>::travelleaf() const {
    dl node_dl = tree_info.root_dl;
    BPT_NODE<KeyType> node;
    do{
        read_node(node_dl, node);
        node_dl = node.children[0];
    }while(!node.is_leaf);
    dl x;
    while(x != -1){
        for(int i = 0; i < node.node_size; ++i)
            printf("%d ", node.keys[i]);
        x = node.next;
        if (x != -1)
            read_node(x, node);
    }
    cout << std::endl;
}
template class BPT<int>;