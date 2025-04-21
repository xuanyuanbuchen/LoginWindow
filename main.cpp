#include "LoginWindow.h"
#include "CashierWindowClass.h"
#include <QtWidgets/QApplication>
#include "UnifiedLoginManager.h"
#include "OracleDB.h"
#include "qfile.h"
#include "qdir.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UnifiedLoginManager::instance().startHttpServer(8080); // 启动 HTTP 服务

    std::vector<std::string> tables;
    try
    {
        // 创建数据库连接
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();
        if (NULL == conn) {
            printf("createConnection error.\n");
        }
        else
            std::cout << "conn success" << std::endl;
        //加入自己的功能
      
    }
    catch (oracle::occi::SQLException& e)
    {
        std::cout << e.what() << std::endl;
    }


    LoginWindow w;
    w.show();
    // 释放 OCCI 上下文环境
    //OracleDB::getInstance().cleanup();
    //std::cout << "end!" << std::endl;
    return a.exec();
}
