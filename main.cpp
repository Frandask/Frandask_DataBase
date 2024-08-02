#include "BPLUSTREE.h"
#include <random>
#include <chrono>
const size_t blockSize = 4096;
const int insert_num = 500;
int main(){
    BPT<int> bpt;
    // 创建一个随机数生成器，使用当前时间作为种子
    std::random_device rd;  // 获取真正的随机数种子
    std::mt19937 gen(rd()); // 以随机种子初始化 Mersenne Twister 引擎
    auto start_clock = std::chrono::high_resolution_clock::now();
    // 创建一个均匀分布的整数随机数生成器
    std::uniform_int_distribution<> dis(1, insert_num);
    for(int i = 1; i <= insert_num; i ++){
        int a1 = dis(gen);
        int a2 = dis(gen);
        bpt.insertkey(1, a2);
    }
    auto end_clock = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_clock - start_clock;
    bpt.travelleaf();
    std::printf("\n插入%d条数据花费了%.6f秒\n", insert_num, duration.count());
    
    return 0;
}