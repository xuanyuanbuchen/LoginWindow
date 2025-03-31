#pragma once

#include <vector>
#include "ProductWidget.h"

class ProductWidgetPagination
{
public:
    // 构造函数
    ProductWidgetPagination(std::vector<ProductWidget*>& widgets, int pageSize = 9);
    // 获取页总数
    int pageCount() const;
    // 设置当前页(自动处理边界)
    void setPage(int page);
    // 获取当前页
    int getCurrentPage() const;
    // 应用到商品展示网格
    void applyToGridLayout(QGridLayout* gridLayout);
    // 获取当前页的起始索引
    int startRow() const;
    // 获取当前页的结束索引(不包含)
    int endRow() const;

private:
    std::vector<ProductWidget*>& widgets;
    int pageSize;
    int currentPage;
};
