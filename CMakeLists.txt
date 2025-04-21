cmake_minimum_required(VERSION 3.16)
project(LoginWindow LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Oracle 配置
set(ORACLE_INCLUDE_DIR "E:/oracle/instantclient_21_17/sdk/include")
set(ORACLE_LIB_DIR "E:/oracle/instantclient_21_17")  # 注意这里是 lib 目录

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
        Charts
        HttpServer
        Concurrent
)
qt_standard_project_setup()




# 添加 SmtpClient-for-Qt 源码
add_subdirectory(SmtpClient_for_QT/SmtpClient-for-Qt)

set(PROJECT_SOURCES
    main.cpp
    LoginWindow.ui
    LoginWindow.h
    LoginWindow.cpp
    CashierWindowClass.ui
    CashierWindowClass.h
    CashierWindowClass.cpp
    SearchEdit.ui
    SearchEdit.h
    SearchEdit.cpp
    ProductWidget.ui
    ProductWidget.h
    ProductWidget.cpp
    ProductDetailDialog.ui
    ProductDetailDialog.h
    ProductDetailDialog.cpp
    ManagerWindowClass.ui
    ManagerWindowClass.h
    ManagerWindowClass.cpp
    GoodsWidget.ui
    GoodsWidget.h
    GoodsWidget.cpp
    GoodsDetailDialog.ui
    GoodsDetailDialog.h
    GoodsDetailDialog.cpp
    CustomerDetailDialog.ui
    CustomerDetailDialog.h
    CustomerDetailDialog.cpp
    ProductWidgetPagination.h
    ProductWidgetPagination.cpp
    GoodsWidgetPagination.h
    GoodsWidgetPagination.cpp
    ClickableLabel.h
    ClickableLabel.cpp
    SqlTools.h
    SqlTools.cpp
    FinanceChart.ui
    FinanceChart.h
    FinanceChart.cpp
    TableViewPagination.h
    OrderDetailDialog.ui
    OrderDetailDialog.h
    OrderDetailDialog.cpp
    StockDetailDialog.ui
    StockDetailDialog.h
    StockDetailDialog.cpp
    StaffDetailDialog.ui
    StaffDetailDialog.h
    StaffDetailDialog.cpp
    UnifiedLoginManager.h
    UnifiedLoginManager.cpp
    OracleDB.h
    OracleDB.cpp

)

qt_add_resources(PROJECT_SOURCES resource.qrc)


qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt6::Charts
        Qt6::HttpServer
        Qt6::Concurrent
        SmtpMime  
        "E:/oracle/instantclient_21_17/oci.lib"         # OCI 库
        "E:/oracle/instantclient_21_17/oraocci21d.lib"   # OCCI 库（根据版本调整）

)

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ${ORACLE_INCLUDE_DIR}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/SmtpClient_for_QT/SmtpClient-for-Qt/src
)