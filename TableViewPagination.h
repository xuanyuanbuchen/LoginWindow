#pragma once
#include <functional>
#include <QAbstractItemModel>
#include <QTableView>
#include "SqlTools.h"
#include <QStandardItemModel>

class TableViewPagination
{
public:
    // 构造函数
    TableViewPagination(QStandardItemModel* model, int pageSize = 15);

    // 获取页总数
    int pageCount() const;

    // 设置当前页(自动处理边界)
    void setPage(int page);

    // 获取当前页
    int getCurrentPage() const;

    // 应用到表格视图
    void applyToTableView(QTableView* tableView);

    // 设置动态数据加载回调
    void setDataLoader(const std::function<std::pair<std::vector<OrderDetail>, int>(int offset, int limit)>& loader);

private:
    QStandardItemModel* model;
    int pageSize;
    int currentPage;
    std::function<std::pair<std::vector<OrderDetail>, int>(int offset, int limit)> dataLoader; // 数据加载回调
};
