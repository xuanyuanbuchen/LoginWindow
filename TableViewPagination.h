#pragma once
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <SearchEdit.h>
class TableViewPagination 
{
public:

    // 构造函数
    TableViewPagination(QAbstractItemModel* model, int pageSize = 15);
    //获取页总数
    int pageCount() const;
    // 设置当前页(自动处理边界)
    void setPage(int page);
    // 获取当前页
    int getCurrentPage() const;
    // 应用到表格视图
    void applyToTableView(QTableView* tableView);
    // 获取当前页的起始行号
    int startRow() const;
    // 获取当前页的结束行号(不包含)
    int endRow() const;

private:
    QAbstractItemModel* model;
    int pageSize;
    int currentPage;

};