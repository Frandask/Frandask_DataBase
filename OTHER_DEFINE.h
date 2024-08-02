#ifndef OTHER_DEFINE_H
#define OTHER_DEFINE_H

#include "includes.h"

typedef struct TREEINFOS tree_infos;


using dl = streampos;

const int MAX_NODE_SIZE = 500;
const int DL_SIZE = sizeof(streampos);
const int BP_ORDER = MAX_NODE_SIZE;


struct TREEINFOS{
    int tree_order = BP_ORDER;
    dl slot = 0;
    dl root_dl = 0;
};

const size_t START_DL = sizeof(tree_infos);
const int SPLIT_LOC = (BP_ORDER + 1) / 2;
const int MERGE_LOC = (BP_ORDER + 1) / 2 - 1;
#endif 