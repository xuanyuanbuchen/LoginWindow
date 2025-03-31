#pragma once

#include <QtWidgets/QWidget>
#include "ui_LoginWindow.h"

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
private slots:
    void onLoginClicked();

private:
    Ui::LoginWindowClass ui;
};
