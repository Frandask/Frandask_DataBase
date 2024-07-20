//B+树节点定义

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

const int MAX_NODE_SIZE = 10;

using dl = streampos;

template <typename KeyType>
class BPT_NODE{
    public:
        bool is_leaf;   //该节点是否为叶节点
        BPT_NODE * parent;      //父节点
        vector <BPT_NODE *> children;  //子节点
        BPT_NODE * next;    //下个节点
        vector <KeyType> keys;  //节点的键值
        vector <dl> values;  //叶结点中存放数据在数据文件中的位置
        int node_size;
        //构造函数
        BPT_NODE(bool is_leaf_ = false, BPT_NODE * next_ = nullptr, int node_size_ = 0,
         BPT_NODE * parent_ = nullptr){
            is_leaf = is_leaf_;
            next = next_;
            node_size = node_size_;
            parent = parent_;
        }
};