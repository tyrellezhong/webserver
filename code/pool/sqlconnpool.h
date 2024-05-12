#ifndef SQLCONNPOOL_H
#define SQLCONNPOOL_H

#include <mysql/mysql.h>
#include <string>
#include <queue>
#include <mutex>
#include <semaphore.h>
#include <thread>
#include "../log/log.h"

class SqlConnPool {
public:
    // 返回一个数据库池静态指针，每次调用返回一样的
    static SqlConnPool *Instance();

    // 返回一个数据库指针
    MYSQL *GetConn();

    // 释放一个数据库指针
    void FreeConn(MYSQL * conn);

    // 获得空闲数据大小
    int GetFreeConnCount();

    // 初始化主机名，端口号，数据库用户，密码，数据库名字，数据库池大小（默认10）
    void Init(const char* host, int port,
              const char* user,const char* pwd, 
              const char* dbName, int connSize);
    // 关闭数据库池
    void ClosePool();

private:
    SqlConnPool();
    ~SqlConnPool();

    int MAX_CONN_;  // 最大的连接数
    int useCount_;  // 当前的用户数
    int freeCount_; // 空闲的用户数

    std::queue<MYSQL *> connQue_;   // 队列（MYSQL *）
    std::mutex mtx_;    // 互斥锁
    sem_t semId_;   // 信号量-同步用
};


#endif // SQLCONNPOOL_H