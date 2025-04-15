#include "GoodsWidgetPagination.h"
#include <QGridLayout>

GoodsWidgetPagination::GoodsWidgetPagination(int kinds_count, int pageSize)
    : pageSize(pageSize), currentPage(0), totalPage((kinds_count + pageSize - 1) / pageSize)
{
    Q_ASSERT(pageSize > 0);
}

int GoodsWidgetPagination::pageCount() const
{
    return totalPage;
}

void GoodsWidgetPagination::applyToGridLayout(QGridLayout* gridLayout, const std::function<std::vector<GoodsWidget*>(int, int)>& fetchPageData)
{
    if (!gridLayout)
    {
        return; // �������Ϊ�գ�ֱ�ӷ���
    }

    // ��ȡ��ǰҳ����Ʒ����
    int start = currentPage * pageSize;
    std::vector<GoodsWidget*> widgets = fetchPageData(start, pageSize);

    // ��յ�ǰ����
    while (QLayoutItem* item = gridLayout->takeAt(0))
    {
        if (item->widget())
        {
            item->widget()->setVisible(false);
        }
        delete item;
    }

    // ��ӵ�ǰҳ����Ʒ
    int row = 0;
    int col = 0;
    for (GoodsWidget* widget : widgets) {
        gridLayout->addWidget(widget, row, col);
        widget->setVisible(true);
        if (++col == 3) { // ÿ��3��
            col = 0;
            ++row;
        }
    }
}

void GoodsWidgetPagination::updatePagination(int kinds_count, int pageSize)
{
    this->pageSize = pageSize;
    this->totalPage = (kinds_count + pageSize - 1) / pageSize;
    this->currentPage = qBound(0, this->currentPage, this->totalPage - 1); // ȷ����ǰҳ����Ч��Χ��
}


void GoodsWidgetPagination::setPage(int page)
{
    currentPage = qBound(0, page, pageCount() - 1);
}

int GoodsWidgetPagination::getCurrentPage() const
{
    return currentPage;
}

int GoodsWidgetPagination::startRow() const
{
    return currentPage * pageSize;
}

int GoodsWidgetPagination::endRow() const
{
    return qMin(startRow() + pageSize, totalPage * pageSize);
}

int GoodsWidgetPagination::getPageSize() const
{
    return pageSize;
}
