#ifndef HEAP_TIMER_H
#define HEAP_TIMER_H

#include <queue>
#include <unordered_map>
#include <time.h>
#include <algorithm>
#include <arpa/inet.h> 
#include <functional> 
#include <assert.h> 
#include <chrono>
#include "../log/log.h"

typedef std::function<void()> TimeoutCallBack;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds MS;
typedef Clock::time_point TimeStamp;

// 定时器节点
struct TimerNode {
    int id;
    TimeStamp expires;
    TimeoutCallBack cb;
    bool operator<(const TimerNode& t) {
        return expires < t.expires;
    }
};
// 定时器类
class HeapTimer {
public:
    // 预留64个定时器
    HeapTimer() { heap_.reserve(64); }

    ~HeapTimer() { clear(); }
    
    // 调整定时器
    void adjust(int id, int newExpires);

    // id：文件描述符 timeOut 超时时间长度  回调函数
    void add(int id, int timeOut, const TimeoutCallBack& cb);

    /* 删除指定id结点，并触发回调函数 */
    void doWork(int id);

    // 定时器队列清空
    void clear();

    /* 清除超时结点 */
    void tick();

    // 弹出堆顶节点
    void pop();

    // 获取下一个超时时间间隔
    int GetNextTick();

private:
    /* 删除指定位置的结点 */
    void del_(size_t i);
    
    // 从指定位置上浮，索引从0开始
    void siftup_(size_t i);

    // 从指定位置下沉 
    bool siftdown_(size_t index, size_t n);

    // 交换节点
    void SwapNode_(size_t i, size_t j);

    // 定时器数组，维护小顶堆特性
    std::vector<TimerNode> heap_;
    // 定时器map
    std::unordered_map<int, size_t> ref_;
};

#endif //HEAP_TIMER_H