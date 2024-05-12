#ifndef BUFFER_H
#define BUFFER_H
#include <cstring>   //perror
#include <iostream>
#include <unistd.h>  // write
#include <sys/uio.h> //readv
#include <vector> //readv
#include <atomic>
#include <assert.h>
class Buffer {
public:
    // 默认初始值1024字节
    Buffer(int initBuffSize = 1024);
    ~Buffer() = default;

    // 可以写的数据大小，缓冲区的总大小 - 写位置
    size_t WritableBytes() const; 

    // 可以读的数据的大小  写位置 - 读位置，中间的数据就是可以读的大小
    size_t ReadableBytes() const ;

    // 前面可以用的空间，当前读取到哪个位置，就是前面可以用的空间大小
    size_t PrependableBytes() const;

    // 缓冲区开始读的位置
    const char* Peek() const;
    // 保证可以写数据
    void EnsureWriteable(size_t len);

    // 更新写的位置
    void HasWritten(size_t len);

    // 更新读的位置
    void Retrieve(size_t len);
    // 更新读到的位置
    void RetrieveUntil(const char* end);

    // 清空缓冲区，重置读写位置
    void RetrieveAll() ;

    // 转化成字符串并清空缓冲区
    std::string RetrieveAllToStr();

    // 获取开始写的位置，也是已存数据的大小
    const char* BeginWriteConst() const;

    // 返回写的位置
    char* BeginWrite();

    // 追加数据
    void Append(const std::string& str);
    // 追加数据
    void Append(const char* str, size_t len);
    // 追加数据
    void Append(const void* data, size_t len);
    // 追加数据
    void Append(const Buffer& buff);

    // 从fd中读数据
    ssize_t ReadFd(int fd, int* Errno); 
    // 向fd中写数据
    ssize_t WriteFd(int fd, int* Errno);

private:
    // 获取内存起始位置
    char* BeginPtr_();   
    // 获取内存起始位置           
    const char* BeginPtr_() const; 
    // 给buffer创建空间 
    void MakeSpace_(size_t len);    

    std::vector<char> buffer_;  // 具体装数据的vector
    std::atomic<std::size_t> readPos_;  // 读的位置
    std::atomic<std::size_t> writePos_; // 写的位置
};

#endif //BUFFER_H