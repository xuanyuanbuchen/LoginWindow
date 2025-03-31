#include "LoginWindow.h"
#include <QDir>
#include <QDebug>
#include "CashierWindowClass.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    
    // 设置窗口背景图片
    QPixmap background(":/res/LogBackground.png");  // 从资源文件中加载图片
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 缩放图片以适应窗口

    QPalette palette;
    palette.setBrush(QPalette::Window, background);  // 设置背景图片
    this->setPalette(palette);

    ui.stackedWidget->setCurrentIndex(0);

    connect(ui.signPageBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(2);
        });
    connect(ui.signReturnToLogBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(0);
        });
    connect(ui.mailPageBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(1);
        });
    connect(ui.mailReturnToLogBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(0);
        });
    connect(ui.logBtn, &QPushButton::clicked,this,&LoginWindow::onLoginClicked);

}

void LoginWindow::onLoginClicked()
{
   
    
    this->hide();
    CashierWindowClass *cashierWindow = new CashierWindowClass;
    connect(cashierWindow, &CashierWindowClass::logoutRequested, this, &LoginWindow::show);  // 连接退出信号
    cashierWindow->show();
    
    
}

LoginWindow::~LoginWindow()
{
    
}
