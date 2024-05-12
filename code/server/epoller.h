#ifndef EPOLLER_H
#define EPOLLER_H

#include <sys/epoll.h> //epoll_ctl()
#include <fcntl.h>  // fcntl()
#include <unistd.h> // close()
#include <assert.h> // close()
#include <vector>
#include <errno.h>

class Epoller {
public:
    // 初始化最大监听事件个数，不是文件描述符的个数，监听文件描述符的个数可以大于这个值
    explicit Epoller(int maxEvent = 10240);
    
    // 关闭对应文件描述符
    ~Epoller();

    // 添加文件描述符
    bool AddFd(int fd, uint32_t events);

    // 修改文件描述符事件
    bool ModFd(int fd, uint32_t events);

    // 从epoll删除指定文件描述符
    bool DelFd(int fd);

    // 调用epoll_wait()进行事件检测
    int Wait(int timeoutMs = -1);

    // 获取产生事件的文件描述符
    int GetEventFd(size_t i) const;

    // 获取事件
    uint32_t GetEvents(size_t i) const;
        
private:
    int epollFd_;   // epoll_create()创建一个epoll对象，返回值就是epollFd

    std::vector<struct epoll_event> events_;     // 检测到的事件的集合 
};

#endif //EPOLLER_H