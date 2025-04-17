#pragma once

#include <QDialog>
#include "ui_StaffDetailDialog.h"

class StaffDetailDialog : public QDialog
{
	Q_OBJECT

public:
	StaffDetailDialog(QWidget *parent = nullptr);
	~StaffDetailDialog();

	Ui::StaffDetailDialog* getUi();
private:
	Ui::StaffDetailDialog ui;
};
