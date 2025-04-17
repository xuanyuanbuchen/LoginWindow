#include "StockDetailDialog.h"

StockDetailDialog::StockDetailDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

StockDetailDialog::~StockDetailDialog()
{
}

Ui::StockDetailDialog* StockDetailDialog::getUi()
{
	return &ui;
}
