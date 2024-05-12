#ifndef LOG_H
#define LOG_H

#include <mutex>
#include <string>
#include <thread>
#include <sys/time.h>
#include <string.h>
#include <stdarg.h>           // vastart va_end
#include <assert.h>
#include <sys/stat.h>         //mkdir
#include "blockqueue.h"
#include "../buffer/buffer.h"
// 单例模式
class Log {
public:
    // 日志初始化
    void init(int level, const char* path = "./log", 
                const char* suffix =".log",
                int maxQueueCapacity = 1024);
    // 返回一个日志对象指针，是静态的，每次返回同一个值
    static Log* Instance();
    
    // 线程函数
    static void FlushLogThread();

    // 向日志写数据
    void write(int level, const char *format,...);

    // 刷新缓冲区
    void flush();

    // 返回日志等级
    int GetLevel();
    // 设置日志等级
    void SetLevel(int level);
    // 日志是否打开
    bool IsOpen() { return isOpen_; }
    
private:
    // 构造函数设为私有，防止外界创建单例对象
    Log();
    // 追加日志等级输出
    void AppendLogLevelTitle_(int level);

    virtual ~Log();

    // 异步写，即在子线程中写
    void AsyncWrite_();

private:
    static const int LOG_PATH_LEN = 256;
    static const int LOG_NAME_LEN = 256;
    static const int MAX_LINES = 50000;

    // 日志路径
    const char* path_;
    const char* suffix_;

    // 一篇日志记录的最大行数，超过够新建一个文件
    int MAX_LINES_;

    // 记录日志记录了多少行
    int lineCount_;
    // 当前天数
    int toDay_;

    // 日志是否打开
    bool isOpen_;
    
    // 缓存输出数据
    Buffer buff_;
    // 日志等级
    int level_;
    // 同步写还是异步写
    bool isAsync_;

    // 日志路径
    FILE* fp_;

    // 写任务队列
    std::unique_ptr<BlockDeque<std::string>> deque_; 

    // 写线程
    std::unique_ptr<std::thread> writeThread_;
    
    // 用于共享操作的保护互斥锁
    std::mutex mtx_;
};

#define LOG_BASE(level, format, ...) \
    do {\
        Log* log = Log::Instance();\
        if (log->IsOpen() && level >= log->GetLevel() ) {\
            log->write(level, format, ##__VA_ARGS__); \
            log->flush();\
        }\
    } while(0);

#define LOG_DEBUG(format, ...) do {LOG_BASE(0, format, ##__VA_ARGS__)} while(0);
#define LOG_INFO(format, ...) do {LOG_BASE(1, format, ##__VA_ARGS__)} while(0);
#define LOG_WARN(format, ...) do {LOG_BASE(2, format, ##__VA_ARGS__)} while(0);
#define LOG_ERROR(format, ...) do {LOG_BASE(3, format, ##__VA_ARGS__)} while(0);

#endif //LOG_H