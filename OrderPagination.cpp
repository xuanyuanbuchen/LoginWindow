#include "OrderPagination.h"

OrderPagination::OrderPagination(QAbstractItemModel* model, int pageSize)
	: model(model), pageSize(pageSize), currentPage(0)
{
	Q_ASSERT(model != nullptr);
	Q_ASSERT(pageSize > 0);
}

int OrderPagination::pageCount() const
{
	//空数据处理
	if (model->rowCount() == 0) return 1;
	return (model->rowCount() + pageSize - 1) / pageSize;
}

void OrderPagination::setPage(int page)
{
	currentPage = qBound(0, page, pageCount() - 1);
}

int OrderPagination::getCurrentPage() const
{
	return currentPage;
}

void OrderPagination::applyToTableView(QTableView* tableView)
{
	int start = currentPage * pageSize;
	int end = qMin(start + pageSize, model->rowCount());

	// 显示当前页数据，隐藏其他行
	for (int i = 0; i < model->rowCount(); ++i) {
		tableView->setRowHidden(i, i < start || i >= end);
	}
}

int OrderPagination::startRow() const
{
	return currentPage * pageSize;
}

int OrderPagination::endRow() const
{
	return qMin(startRow() + pageSize, model->rowCount());
}
