#ifndef OTHER_DEFINE_H
#define OTHER_DEFINE_H

#include "includes.h"

typedef struct TREEINFOS tree_infos;


using dl = streampos;

const int MAX_NODE_SIZE = 5;
const int DL_SIZE = sizeof(streampos);
const int BP_ORDER = 5;


struct TREEINFOS{
    int tree_order = BP_ORDER;

    int internal_node_num = 0;
    int leaf_node_num = 0;
    int user_record_num = 0;
    int height = 0;
    dl slot = 0;
    dl root_dl = 0;
    dl leaf_dl = 0;
};

const size_t START_DL = sizeof(tree_infos);
const int SPLIT_LOC = (BP_ORDER + 1) / 2 - 1;
#endif 