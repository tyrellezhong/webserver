#include <unistd.h>
#include "server/webserver.h"

int main() {
    /* 守护进程 后台运行 */
    // daemon(1, 0); 

    WebServer server(
        1316, 3, 10000, false,             /* 端口 ET模式 timeoutMs 优雅退出  */
        3306, "root", "ztn", "websource", /* Mysql配置 */
        8, 4, true, 1, 1024);             /* 数据库连接池数量 线程池数量 日志开关 日志等级 日志异步队列容量 */
    
    
    // 启动服务器
    server.Start();
} 
  