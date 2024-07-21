#include "BPLUSTREE.h"
int main(){
    BPT<int> bpt;
    for(int i = 1; i <= 5; i ++)
        bpt.insertkey(i, i);
    return 0;
}