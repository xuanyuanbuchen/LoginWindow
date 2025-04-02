#include "TableViewPagination.h"

TableViewPagination::TableViewPagination(QAbstractItemModel* model, int pageSize)
	: model(model), pageSize(pageSize), currentPage(0)
{
	Q_ASSERT(model != nullptr);
	Q_ASSERT(pageSize > 0);
}

int TableViewPagination::pageCount() const
{
	//�����ݴ���
	if (model->rowCount() == 0) return 1;
	return (model->rowCount() + pageSize - 1) / pageSize;
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
	int start = currentPage * pageSize;
	int end = qMin(start + pageSize, model->rowCount());

	// ��ʾ��ǰҳ���ݣ�����������
	for (int i = 0; i < model->rowCount(); ++i) {
		tableView->setRowHidden(i, i < start || i >= end);
	}
}

int TableViewPagination::startRow() const
{
	return currentPage * pageSize;
}

int TableViewPagination::endRow() const
{
	return qMin(startRow() + pageSize, model->rowCount());
}
