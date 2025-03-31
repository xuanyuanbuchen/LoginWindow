#include "LoginWindow.h"
#include <QDir>
#include <QDebug>
#include "CashierWindowClass.h"
#include "ManagerWindowClass.h"

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
	//ע�ᰴť�¼�
    connect(ui.signPageBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(2);
        });
	//ע�᷵�ص�¼���水ť�¼�
    connect(ui.signReturnToLogBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(0);
        });
	//�ʼ���ť�¼�
    connect(ui.mailPageBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(1);
        });
	//�ʼ����ص�¼���水ť�¼�
    connect(ui.mailReturnToLogBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(0);
        });
	//��¼��ť�¼�
    connect(ui.logBtn, &QPushButton::clicked,this,&LoginWindow::onLoginClicked);
	//�ʼ���¼��ť�¼�
	connect(ui.mailLogBtn, &QPushButton::clicked, this, &LoginWindow::onMailLoginClicked);
}

void LoginWindow::onLoginClicked()
{
    this->hide();
    CashierWindowClass* cashierWindow = new CashierWindowClass;
    connect(cashierWindow, &CashierWindowClass::logoutRequested, this, &LoginWindow::show);  // �����˳��ź�
    cashierWindow->show();   
}

LoginWindow::~LoginWindow()
{
    
}

void LoginWindow::onMailLoginClicked()
{
	this->hide();
	ManagerWindowClass* managerWindow = new ManagerWindowClass;
	connect(managerWindow, &ManagerWindowClass::logoutRequested, this, &LoginWindow::show);  // �����˳��ź�
	managerWindow->show();
}
