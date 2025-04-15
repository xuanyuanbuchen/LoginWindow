#include "TableViewPagination.h"

TableViewPagination::TableViewPagination(QStandardItemModel* model, int pageSize)
    : model(model), pageSize(pageSize), currentPage(0)
{
    Q_ASSERT(model != nullptr);
    Q_ASSERT(pageSize > 0);
}

int TableViewPagination::pageCount() const
{
    if (!dataLoader) 
    {
        return (model->rowCount() + pageSize - 1) / pageSize;
    }
    // ��̬����ʱ��ҳ�������ݼ���������
    auto [_, totalCount] = dataLoader(0, 0);
    return (totalCount + pageSize - 1) / pageSize;
}

void TableViewPagination::setPage(int page)
{
    currentPage = qBound(0, page, pageCount() - 1);
}

int TableViewPagination::getCurrentPage() const
{
    return currentPage;
}

void TableViewPagination::applyToTableView(QTableView* tableView)
{
    if (!dataLoader) 
    {
        // ��̬���ݴ���
        int start = currentPage * pageSize;
        int end = qMin(start + pageSize, model->rowCount());

        for (int i = 0; i < model->rowCount(); ++i) {
            tableView->setRowHidden(i, i < start || i >= end);
        }
    }
    else 
    {
        // ��̬���ݼ���
        int offset = currentPage * pageSize;
        auto [records, totalCount] = dataLoader(offset, pageSize);

        // ���ģ������
        model->removeRows(0, model->rowCount());

        // ��䵱ǰҳ����
        for (const auto& record : records) 
        {
            QList<QStandardItem*> rowItems;
            rowItems << new QStandardItem(QString::fromStdString(record.order_ID));
            rowItems << new QStandardItem(QString::fromStdString(record.customer_ID));
            rowItems << new QStandardItem(QString::fromStdString(record.date));
            rowItems << new QStandardItem(QString::fromStdString(record.price));
            rowItems << new QStandardItem(QString::fromStdString(record.state));
            model->appendRow(rowItems);
        }
    }
}

void TableViewPagination::setDataLoader(const std::function<std::pair<std::vector<OrderDetail>, int>(int offset, int limit)>& loader)
{
    dataLoader = loader;
}
