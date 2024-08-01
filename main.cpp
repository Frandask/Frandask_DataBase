#include "BPLUSTREE.h"
#include <random>
int main(){
    BPT<int> bpt;
    // 创建一个随机数生成器，使用当前时间作为种子
    std::random_device rd;  // 获取真正的随机数种子
    std::mt19937 gen(rd()); // 以随机种子初始化 Mersenne Twister 引擎

    // 创建一个均匀分布的整数随机数生成器
    std::uniform_int_distribution<> dis(1, 100);
    for(int i = 1; i <= 200; i ++){
        int a1 = dis(gen);
        int a2 = dis(gen);
        printf("%d\n", a1);
        bpt.insertkey(a1, a2);
    }
    bpt.travelleaf();
    return 0;
}