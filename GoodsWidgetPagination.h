#pragma once

#include <vector>
#include <functional>
#include "GoodsWidget.h"

class GoodsWidgetPagination
{
public:
    // ���캯��
    GoodsWidgetPagination(int kinds_count, int pageSize = 9);

    // ��ȡҳ����
    int pageCount() const;

    // ���õ�ǰҳ(�Զ�����߽�)
    void setPage(int page);

    // ��ȡ��ǰҳ
    int getCurrentPage() const;

    // Ӧ�õ���Ʒչʾ����
    void applyToGridLayout(QGridLayout* gridLayout, const std::function<std::vector<GoodsWidget*>(int, int)>& fetchPageData);

    // ���·�ҳ��
    void updatePagination(int kinds_count, int pageSize);

    // ��ȡ��ǰҳ����ʼ����
    int startRow() const;

    // ��ȡ��ǰҳ�Ľ�������(������)
    int endRow() const;

    int getPageSize() const;

private:
    int pageSize;
    int currentPage;
    int totalPage;
};
