#pragma once

#include <vector>
#include "ProductWidget.h"

class ProductWidgetPagination
{
public:
    // 构造函数
    ProductWidgetPagination(int kinds_count, int pageSize = 9);
    // 获取页总数
    int pageCount() const;
    // 设置当前页(自动处理边界)
    void setPage(int page);
    // 获取当前页
    int getCurrentPage() const;
    // 应用到商品展示网格
    void applyToGridLayout(QGridLayout* gridLayout, const std::function<std::vector<ProductWidget*>(int, int)>& fetchPageData);
    // 获取当前页的起始索引
    int startRow() const;
    // 获取当前页的结束索引(不包含)
    int endRow() const;

    int getPageSize() const;

    void updatePagination(int kinds_count, int pageSize);

private:
    int pageSize;
    int currentPage;
    int totalPage;
};
