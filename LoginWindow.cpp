#include "LoginWindow.h"
#include <QDir>
#include <QDebug>
#include "CashierWindowClass.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    
    // ���ô��ڱ���ͼƬ
    QPixmap background(":/res/LogBackground.png");  // ����Դ�ļ��м���ͼƬ
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // ����ͼƬ����Ӧ����

    QPalette palette;
    palette.setBrush(QPalette::Window, background);  // ���ñ���ͼƬ
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
    connect(cashierWindow, &CashierWindowClass::logoutRequested, this, &LoginWindow::show);  // �����˳��ź�
    cashierWindow->show();
    
    
}

LoginWindow::~LoginWindow()
{
    
}
