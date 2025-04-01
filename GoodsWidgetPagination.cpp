#include "GoodsWidgetPagination.h"
#include <QGridLayout>

GoodsWidgetPagination::GoodsWidgetPagination(std::vector<GoodsWidget*>& widgets, int pageSize)
    : widgets(widgets), pageSize(pageSize), currentPage(0)
{
    Q_ASSERT(pageSize > 0);
}

int GoodsWidgetPagination::pageCount() const
{
    if (widgets.empty()) return 1;
    return (widgets.size() + pageSize - 1) / pageSize;
}

void GoodsWidgetPagination::setPage(int page)
{
    currentPage = qBound(0, page, pageCount() - 1);
}

int GoodsWidgetPagination::getCurrentPage() const
{
    return currentPage;
}

void GoodsWidgetPagination::applyToGridLayout(QGridLayout* gridLayout)
{
    int start = currentPage * pageSize;
    int end = qMin(start + pageSize, static_cast<int>(widgets.size()));

    // 清空当前布局
    while (QLayoutItem* item = gridLayout->takeAt(0)) {
        if (item->widget()) {
            item->widget()->setVisible(false);
        }
        delete item;
    }

    // 添加当前页的商品
    int row = 0;
    int col = 0;
    for (int i = start; i < end; ++i) {
        gridLayout->addWidget(widgets[i], row, col);
        widgets[i]->setVisible(true);
        if (++col == 3) {
            col = 0;
            ++row;
        }
    }
}

int GoodsWidgetPagination::startRow() const
{
    return currentPage * pageSize;
}

int GoodsWidgetPagination::endRow() const
{
    return qMin(startRow() + pageSize, static_cast<int>(widgets.size()));
}
