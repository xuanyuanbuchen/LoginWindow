#pragma once

#include <QtWidgets/QWidget>
#include "ui_LoginWindow.h"

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
private:
	void InitBackground();
	void InitConnect();

private slots:
    void onLoginClicked();
    void onMailLoginClicked();
	void onRegisterClicked();
    void onSignCodeSentClicked();
	void onMailCodeSentClicked();

private:
    Ui::LoginWindowClass ui;
};
