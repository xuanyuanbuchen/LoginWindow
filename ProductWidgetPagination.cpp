#include "ProductWidgetPagination.h"
#include <QGridLayout>

ProductWidgetPagination::ProductWidgetPagination(int kinds_count, int pageSize)
    : pageSize(pageSize), currentPage(0), totalPage((kinds_count + pageSize - 1) / pageSize)
{
    Q_ASSERT(pageSize > 0);
}

int ProductWidgetPagination::pageCount() const
{
    return totalPage;
}

void ProductWidgetPagination::setPage(int page)
{
    currentPage = qBound(0, page, pageCount() - 1);
}

int ProductWidgetPagination::getCurrentPage() const
{
    return currentPage;
}

void ProductWidgetPagination::applyToGridLayout(QGridLayout* gridLayout, const std::function<std::vector<ProductWidget*>(int, int)>& fetchPageData)
{
    if (!gridLayout) 
    {
        return; // 如果布局为空，直接返回
    }

    // 获取当前页的商品数据
    int start = currentPage * pageSize;
    std::vector<ProductWidget*> widgets = fetchPageData(start, pageSize);

    // 清空当前布局
    while (QLayoutItem* item = gridLayout->takeAt(0)) 
    {
        if (item->widget()) 
        {
            item->widget()->setVisible(false);
        }
        delete item;
    }

    // 添加当前页的商品
    int row = 0;
    int col = 0;
    for (ProductWidget* widget : widgets) {
        gridLayout->addWidget(widget, row, col);
        widget->setVisible(true);
        if (++col == 3) { // 每行3列
            col = 0;
            ++row;
        }
    }
}

int ProductWidgetPagination::startRow() const
{
    return currentPage * pageSize;
}

int ProductWidgetPagination::endRow() const
{
    return qMin(startRow() + pageSize, totalPage * pageSize);
}

int ProductWidgetPagination::getPageSize() const
{
    return 0;
}

void ProductWidgetPagination::updatePagination(int kinds_count, int pageSize)
{
    this->pageSize = pageSize;
    this->totalPage = (kinds_count + pageSize - 1) / pageSize;
    this->currentPage = qBound(0, this->currentPage, this->totalPage - 1); 
}
