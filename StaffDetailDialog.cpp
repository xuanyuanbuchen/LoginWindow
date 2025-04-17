#include "StaffDetailDialog.h"

StaffDetailDialog::StaffDetailDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

StaffDetailDialog::~StaffDetailDialog()
{
}

Ui::StaffDetailDialog* StaffDetailDialog::getUi()
{
	return &ui;
}
