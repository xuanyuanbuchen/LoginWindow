#include "OracleDB.h"
#include <iostream>
using namespace oracle::occi;

OracleDB::OracleDB() : env(nullptr), conn(nullptr) {
    try {
        env = Environment::createEnvironment(Environment::DEFAULT);
        conn = env->createConnection(name, pass, srvName);
        std::cout << "OracleDB connected successfully." << std::endl;
    }
    catch (SQLException& e) {
        std::cerr << "OracleDB connection failed: " << e.getMessage() << std::endl;
    }
}

OracleDB::~OracleDB() {
    cleanup();
}

OracleDB& OracleDB::getInstance() {
    static OracleDB instance;
    return instance;
}

Connection* OracleDB::getConnection() {
    std::lock_guard<std::mutex> lock(conn_mutex);
    return conn;
}

void OracleDB::cleanup() {
    if (conn) {
        env->terminateConnection(conn);
        conn = nullptr;
    }
    if (env) {
        Environment::terminateEnvironment(env);
        env = nullptr;
    }
}

//主程序结束前调用OracleDB::getInstance().cleanup();