#pragma once

#include <vector>
#include "ProductWidget.h"

class ProductWidgetPagination
{
public:
    // ���캯��
    ProductWidgetPagination(int kinds_count, int pageSize = 9);
    // ��ȡҳ����
    int pageCount() const;
    // ���õ�ǰҳ(�Զ�����߽�)
    void setPage(int page);
    // ��ȡ��ǰҳ
    int getCurrentPage() const;
    // Ӧ�õ���Ʒչʾ����
    void applyToGridLayout(QGridLayout* gridLayout, const std::function<std::vector<ProductWidget*>(int, int)>& fetchPageData);
    // ��ȡ��ǰҳ����ʼ����
    int startRow() const;
    // ��ȡ��ǰҳ�Ľ�������(������)
    int endRow() const;

    int getPageSize() const;

    void updatePagination(int kinds_count, int pageSize);

private:
    int pageSize;
    int currentPage;
    int totalPage;
};
