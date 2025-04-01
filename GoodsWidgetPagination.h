#pragma once

#include <vector>
#include "GoodsWidget.h"

class GoodsWidgetPagination
{
public:
    // ���캯��
    GoodsWidgetPagination(std::vector<GoodsWidget*>& widgets, int pageSize = 9);
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
    std::vector<GoodsWidget*>& widgets;
    int pageSize;
    int currentPage;
};
