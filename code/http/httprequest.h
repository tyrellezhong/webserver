#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <regex>
#include <errno.h>     
#include <mysql/mysql.h>  //mysql

#include "../buffer/buffer.h"
#include "../log/log.h"
#include "../pool/sqlconnpool.h"
#include "../pool/sqlconnRAII.h"

class HttpRequest {
public:
    // 解析状态，正在解析的部分
    enum PARSE_STATE {
        REQUEST_LINE,   // 正在解析请求首行
        HEADERS,        // 头
        BODY,           // 体
        FINISH,         // 完成
    };
    // 请求类型，结果
    enum HTTP_CODE {
        NO_REQUEST = 0,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURSE,
        FORBIDDENT_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION,
    };
    
    HttpRequest() { Init(); }
    ~HttpRequest() = default;
    
    // 初始化请求对象信息
    void Init();

    // 解析请求数据，入口
    bool parse(Buffer& buff);
    
    // 获取请求资源的相对路径
    std::string path() const;
    // 获取请求资源的相对路径
    std::string& path();
    // 获取请求方法
    std::string method() const;
    // 获取请求版本
    std::string version() const;
    
    std::string GetPost(const std::string& key) const;
    std::string GetPost(const char* key) const;

    // 是否保持连接
    bool IsKeepAlive() const;

private:
    // 解析请求行
    bool ParseRequestLine_(const std::string& line);
    // 解析请求头
    void ParseHeader_(const std::string& line);
    // 解析请求体
    void ParseBody_(const std::string& line);

    // 解析出请求资源路径
    void ParsePath_();
    // 解析post请求
    void ParsePost_();
    // 解析post表单数据
    void ParseFromUrlencoded_();

    // 用户验证（整合了登录和注册的验证）
    static bool UserVerify(const std::string& name, const std::string& pwd, bool isLogin);

    PARSE_STATE state_;     // 解析的状态

    std::string method_, path_, version_, body_;    // 请求方法，请求路径，协议版本，请求体

    std::unordered_map<std::string, std::string> header_;   // 请求头

    std::unordered_map<std::string, std::string> post_;     // post请求表单数据

    // 默认的网页路径
    static const std::unordered_set<std::string> DEFAULT_HTML;  

    static const std::unordered_map<std::string, int> DEFAULT_HTML_TAG; 

    // 将十六进制字符转换成十进制整数
    static int ConverHex(char ch);  
};


#endif //HTTP_REQUEST_H