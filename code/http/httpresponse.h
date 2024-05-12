#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <unordered_map>
#include <fcntl.h>       // open
#include <unistd.h>      // close
#include <sys/stat.h>    // stat
#include <sys/mman.h>    // mmap, munmap

#include "../buffer/buffer.h"
#include "../log/log.h"


class HttpResponse {
public:
    // 属性重置默认值
    HttpResponse();

    // 解除内存映射
    ~HttpResponse();

    // 初始化响应信息
    void Init(const std::string& srcDir, std::string& path, bool isKeepAlive = false, int code = -1);

    // 生成响应信息（writeBuff_中保存着响应的一些信息）
    void MakeResponse(Buffer& buff);
    
    // 解除内存映射
    void UnmapFile();
    
    // 返回内存映射文件指针
    char* File();

    // 返回文件的大小
    size_t FileLen() const;

    void ErrorContent(Buffer& buff, std::string message);

    // 返回状态码
    int Code() const { return code_; }
    
private:
    // 添加响应状态行
    void AddStateLine_(Buffer &buff);
    // 添加响应头
    void AddHeader_(Buffer &buff);
    // 添加响应体
    void AddContent_(Buffer &buff);
    
    // 更新对应的状态码路径
    void ErrorHtml_();

    /* 判断文件类型 */
    std::string GetFileType_();

    int code_;  // 响应状态码
    bool isKeepAlive_;  // 是否保持连接

    std::string path_;  // 资源的路径
    std::string srcDir_;    // 资源的目录
    
    char* mmFile_;  // 文件内存映射的指针
    struct stat mmFileStat_;    // 文件的状态信息

    static const std::unordered_map<std::string, std::string> SUFFIX_TYPE;  // 后缀 - 类型
    static const std::unordered_map<int, std::string> CODE_STATUS;    // 状态码 - 描述 
    static const std::unordered_map<int, std::string> CODE_PATH;      // 状态码 - 路径
};


#endif //HTTP_RESPONSE_H