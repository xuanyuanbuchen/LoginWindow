#pragma once

#include <vector>
#include <functional>
#include "GoodsWidget.h"

class GoodsWidgetPagination
{
public:
    // 构造函数
    GoodsWidgetPagination(int kinds_count, int pageSize = 9);

    // 获取页总数
    int pageCount() const;

    // 设置当前页(自动处理边界)
    void setPage(int page);

    // 获取当前页
    int getCurrentPage() const;

    // 应用到商品展示网格
    void applyToGridLayout(QGridLayout* gridLayout, const std::function<std::vector<GoodsWidget*>(int, int)>& fetchPageData);

    // 更新分页器
    void updatePagination(int kinds_count, int pageSize);

    // 获取当前页的起始索引
    int startRow() const;

    // 获取当前页的结束索引(不包含)
    int endRow() const;

    int getPageSize() const;

private:
    int pageSize;
    int currentPage;
    int totalPage;
};
