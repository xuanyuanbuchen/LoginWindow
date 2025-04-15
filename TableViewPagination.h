#pragma once
#include <functional>
#include <QAbstractItemModel>
#include <QTableView>
#include "SqlTools.h"
#include <QStandardItemModel>

class TableViewPagination
{
public:
    // ���캯��
    TableViewPagination(QStandardItemModel* model, int pageSize = 15);

    // ��ȡҳ����
    int pageCount() const;

    // ���õ�ǰҳ(�Զ�����߽�)
    void setPage(int page);

    // ��ȡ��ǰҳ
    int getCurrentPage() const;

    // Ӧ�õ������ͼ
    void applyToTableView(QTableView* tableView);

    // ���ö�̬���ݼ��ػص�
    void setDataLoader(const std::function<std::pair<std::vector<OrderDetail>, int>(int offset, int limit)>& loader);

private:
    QStandardItemModel* model;
    int pageSize;
    int currentPage;
    std::function<std::pair<std::vector<OrderDetail>, int>(int offset, int limit)> dataLoader; // ���ݼ��ػص�
};
