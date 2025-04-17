cmake_minimum_required(VERSION 3.16)
project(LoginWindow LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
        Charts
)
qt_standard_project_setup()


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

)

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
)

