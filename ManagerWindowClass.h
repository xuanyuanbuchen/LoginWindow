#pragma once

#include <QWidget>
#include "ui_ManagerWindowClass.h"

class ManagerWindowClass : public QWidget
{
	Q_OBJECT

public:
	ManagerWindowClass(QWidget *parent = nullptr);
	~ManagerWindowClass();

private:
	Ui::ManagerWindowClass ui;
};
