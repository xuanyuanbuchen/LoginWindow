#pragma once

#include <QDialog>
#include "ui_StockDetailDialog.h"

class StockDetailDialog : public QDialog
{
	Q_OBJECT

public:
	StockDetailDialog(QWidget *parent = nullptr);
	~StockDetailDialog();

	Ui::StockDetailDialog* getUi();

private:
	Ui::StockDetailDialog ui;
};
