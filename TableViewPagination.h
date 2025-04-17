#pragma once
#include <QStandardItemModel>
#include <vector>

// 使用模板类
class TableViewPagination
{
public:
    TableViewPagination(int pageSize = 30)
        : pageSize(pageSize), currentPage(0)
    {
        Q_ASSERT(pageSize > 0);
    }

    // 获取页总数
    int pageCount(int totalItemCount) const
    {
        return (totalItemCount + pageSize - 1) / pageSize;
    }

    // 设置当前页(自动处理边界)
    void setPage(int page, int totalItemCount)
    {
        currentPage = qBound(0, page, pageCount(totalItemCount) - 1);
    }

    // 获取当前页
    int getCurrentPage() const
    {
        return currentPage;
    }

    // 获取每页大小
    int getPageSize() const
    {
        return pageSize;
    }

private:
    int pageSize;
    int currentPage;
};
