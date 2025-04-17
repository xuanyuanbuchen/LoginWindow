#pragma once

#include <QDialog>
#include "ui_OrderDetailDialog.h"

class OrderDetailDialog : public QDialog
{
	Q_OBJECT

public:
	OrderDetailDialog(QWidget *parent = nullptr);
	~OrderDetailDialog();

	Ui::OrderDetailDialog* getUi();

private:
	Ui::OrderDetailDialog ui;
};
