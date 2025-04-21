#pragma once
#include <occi.h>
#include <string>
#include <memory>
#include <mutex>

class OracleDB {
public:
    // 获取单例实例
    static OracleDB& getInstance();

    // 获取 Oracle 连接
    oracle::occi::Connection* getConnection();

    // 关闭连接
    void cleanup();

private:
    OracleDB(); // 构造函数
    ~OracleDB(); // 析构函数禁止外部调用

    OracleDB(const OracleDB&) = delete;
    OracleDB& operator=(const OracleDB&) = delete;

    oracle::occi::Environment* env;
    oracle::occi::Connection* conn;
    std::mutex conn_mutex;
    std::string name = "SUPERMARKET";  // 数据库用户名
    std::string pass = "04314449ll"; // 数据库密码
    std::string srvName = "localhost:1521/orcl"; // 连接的服务名
};