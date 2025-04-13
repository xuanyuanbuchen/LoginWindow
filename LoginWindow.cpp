#include "LoginWindow.h"
#include <QDir>
#include <QDebug>
#include "CashierWindowClass.h"
#include "ManagerWindowClass.h"
#include "SqlTools.h"
#include <qmessagebox.h>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	InitBackground();
	InitConnect();
}

void LoginWindow::onLoginClicked()
{
	const int logIdentityType = SqlTools::Login_Account_Password_Check(ui.logAccountLineEdit->text().toStdString(), ui.logPasswordLineEdit->text().toStdString());
    if (logIdentityType == 1)
    {
        qDebug() << "��¼�ɹ�";
        this->hide();
        CashierWindowClass* cashierWindow = new CashierWindowClass;
        connect(cashierWindow, &CashierWindowClass::logoutRequested, this, &LoginWindow::show);  // �����˳��ź�
		cashierWindow->show();
	}
	else if (logIdentityType == 2)
	{
		qDebug() << "��¼�ɹ�";
		this->hide();
		ManagerWindowClass* managerWindow = new ManagerWindowClass;
		connect(managerWindow, &ManagerWindowClass::logoutRequested, this, &LoginWindow::show);  // �����˳��ź�
		managerWindow->show();
	}
	else
	{
		qDebug() << "��¼ʧ��";
		// ��ʾ������ʾ
		QMessageBox::warning(this, "��¼ʧ��", "�˺Ż�������������ԡ�");
		ui.logAccountLineEdit->clear(); // ����˺������
		ui.logPasswordLineEdit->clear(); // ������������
    }
  
}

LoginWindow::~LoginWindow()
{
    
}

void LoginWindow::InitBackground()
{
	// ���ô��ڱ���ͼƬ
	QPixmap background(":/res/LogBackground.png");  // ����Դ�ļ��м���ͼƬ
	background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // ����ͼƬ����Ӧ����

	QPalette palette;
	palette.setBrush(QPalette::Window, background);  // ���ñ���ͼƬ
	this->setPalette(palette);

}

void LoginWindow::InitConnect()
{
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
	connect(ui.logBtn, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
	//�ʼ���¼��ť�¼�
	connect(ui.mailLogBtn, &QPushButton::clicked, this, &LoginWindow::onMailLoginClicked);
	//ע�ᰴť�¼�
	connect(ui.signBtn, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
	//ע�ᷢ����֤�밴ť�¼�
	connect(ui.signSendCodeBtn, &QPushButton::clicked, this, &LoginWindow::onSignCodeSentClicked);
	connect(ui.mailSendCodeBtn, &QPushButton::clicked, this, &LoginWindow::onMailCodeSentClicked);
}

void LoginWindow::onMailLoginClicked()
{
	const int logIdentityType = SqlTools::Login_Email_Code_Check(ui.mailMailBoxLineEdit->text().toStdString(), ui.mailVerificationCodeLineEdit->text().toStdString());
	if (logIdentityType == 1)
	{
		qDebug() << "��¼�ɹ�";
		this->hide();
		CashierWindowClass* cashierWindow = new CashierWindowClass;
		connect(cashierWindow, &CashierWindowClass::logoutRequested, this, &LoginWindow::show);  // �����˳��ź�
		cashierWindow->show();
	}
	else if (logIdentityType == 2)
	{
		qDebug() << "��¼�ɹ�";
		this->hide();
		ManagerWindowClass* managerWindow = new ManagerWindowClass;
		connect(managerWindow, &ManagerWindowClass::logoutRequested, this, &LoginWindow::show);  // �����˳��ź�
		managerWindow->show();
	}
	else
	{
		qDebug() << "��¼ʧ��";
		// ��ʾ������ʾ
		QMessageBox::warning(this, "��¼ʧ��", "�������֤����������ԡ�");
		ui.mailMailBoxLineEdit->clear(); // ����˺������
		ui.mailVerificationCodeLineEdit->clear(); // ������������
	}
}

void LoginWindow::onRegisterClicked()
{
	if (SqlTools::Register_Account_Password_Check(ui.signAccountLineEdit->text().toStdString(), ui.signPasswordLineEdit->text().toStdString())
		&& SqlTools::Register_Email_Code_Check(ui.signMailLineEdit->text().toStdString(), ui.signPasswordLineEdit->text().toStdString())
		&& (ui.signPasswordLineEdit->text() == ui.signRePasswordLineEdit->text()))
	{
		if (SqlTools::Register_Account_Password_Email_Wirte(ui.signAccountLineEdit->text().toStdString(), ui.signPasswordLineEdit->text().toStdString(), ui.signMailLineEdit->text().toStdString()))
		{
			qDebug() << "ע��ɹ�";
			QMessageBox::information(this, "ע��ɹ�", "ע��ɹ������¼��");
			ui.stackedWidget->setCurrentIndex(0);
		}
		else
		{
			qDebug() << "ע��ʧ��";
			// ��ʾ������ʾ
			QMessageBox::warning(this, "ע��ʧ��", "������д��������Ժ����ԡ�");
			ui.signAccountLineEdit->clear(); // ����˺������
			ui.signPasswordLineEdit->clear(); // ������������
			ui.signRePasswordLineEdit->clear(); // ������������
			ui.signMailLineEdit->clear(); // ������������
			ui.signVerificationCodeLineEdit->clear(); // �����֤�������
		}
		
	}
	else
	{
		qDebug() << "ע��ʧ��";
		// ��ʾ������ʾ
		QMessageBox::warning(this, "ע��ʧ��", "�������������ԡ�");
		ui.signAccountLineEdit->clear(); // ����˺������
		ui.signPasswordLineEdit->clear(); // ������������
		ui.signRePasswordLineEdit->clear(); // ������������
		ui.signMailLineEdit->clear(); // ������������
		ui.signVerificationCodeLineEdit->clear(); // �����֤�������

	}
}

void LoginWindow::onSignCodeSentClicked()
{

}

void LoginWindow::onMailCodeSentClicked()
{

}
