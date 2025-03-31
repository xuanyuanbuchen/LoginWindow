#pragma once

#include <QWidget>
#include "ui_ManagerWindowClass.h"

class ManagerWindowClass : public QWidget
{
	Q_OBJECT

public:
	ManagerWindowClass(QWidget *parent = nullptr);
	~ManagerWindowClass();

signals:
    void logoutRequested();  // 退出信号

private slots:
    void onLogoutClicked();

private:
    Ui::ManagerWindowClass ui;

};
