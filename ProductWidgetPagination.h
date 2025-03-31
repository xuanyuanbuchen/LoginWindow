#pragma once

#include <vector>
#include "ProductWidget.h"

class ProductWidgetPagination
{
public:
    // ���캯��
    ProductWidgetPagination(std::vector<ProductWidget*>& widgets, int pageSize = 9);
    // ��ȡҳ����
    int pageCount() const;
    // ���õ�ǰҳ(�Զ�����߽�)
    void setPage(int page);
    // ��ȡ��ǰҳ
    int getCurrentPage() const;
    // Ӧ�õ���Ʒչʾ����
    void applyToGridLayout(QGridLayout* gridLayout);
    // ��ȡ��ǰҳ����ʼ����
    int startRow() const;
    // ��ȡ��ǰҳ�Ľ�������(������)
    int endRow() const;

private:
    std::vector<ProductWidget*>& widgets;
    int pageSize;
    int currentPage;
};
