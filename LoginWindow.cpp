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
        qDebug() << "登录成功";
        this->hide();
        CashierWindowClass* cashierWindow = new CashierWindowClass
		(
			ui.logAccountLineEdit->text().toStdString(),
			ui.logPasswordLineEdit->text().toStdString()
		);
        connect(cashierWindow, &CashierWindowClass::logoutRequested, this, &LoginWindow::show);  // 连接退出信号
		cashierWindow->show();
	}
	else if (logIdentityType == 2)
	{
		qDebug() << "登录成功";
		this->hide();
		ManagerWindowClass* managerWindow = new ManagerWindowClass;
		connect(managerWindow, &ManagerWindowClass::logoutRequested, this, &LoginWindow::show);  // 连接退出信号
		managerWindow->show();
	}
	else
	{
		qDebug() << "登录失败";
		// 显示错误提示
		QMessageBox::warning(this, "登录失败", "账号或密码错误，请重试。");
		ui.logAccountLineEdit->clear(); // 清空账号输入框
		ui.logPasswordLineEdit->clear(); // 清空密码输入框
    }
  
}

LoginWindow::~LoginWindow()
{
    
}

void LoginWindow::InitBackground()
{
	// 设置窗口背景图片
	QPixmap background(":/res/LogBackground.png");  // 从资源文件中加载图片
	background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 缩放图片以适应窗口

	QPalette palette;
	palette.setBrush(QPalette::Window, background);  // 设置背景图片
	this->setPalette(palette);

}

void LoginWindow::InitConnect()
{
	ui.stackedWidget->setCurrentIndex(0);
	//注册按钮事件
	connect(ui.signPageBtn, &QPushButton::clicked, [=]() {
		ui.stackedWidget->setCurrentIndex(2);
		});
	//注册返回登录界面按钮事件
	connect(ui.signReturnToLogBtn, &QPushButton::clicked, [=]() {
		ui.stackedWidget->setCurrentIndex(0);
		});
	//邮件按钮事件
	connect(ui.mailPageBtn, &QPushButton::clicked, [=]() {
		ui.stackedWidget->setCurrentIndex(1);
		});
	//邮件返回登录界面按钮事件
	connect(ui.mailReturnToLogBtn, &QPushButton::clicked, [=]() {
		ui.stackedWidget->setCurrentIndex(0);
		});
	//登录按钮事件
	connect(ui.logBtn, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
	//邮件登录按钮事件
	connect(ui.mailLogBtn, &QPushButton::clicked, this, &LoginWindow::onMailLoginClicked);
	//注册按钮事件
	connect(ui.signBtn, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
	//注册发送验证码按钮事件
	connect(ui.signSendCodeBtn, &QPushButton::clicked, this, &LoginWindow::onSignCodeSentClicked);
	connect(ui.mailSendCodeBtn, &QPushButton::clicked, this, &LoginWindow::onMailCodeSentClicked);
}

void LoginWindow::onMailLoginClicked()
{
	const int logIdentityType = SqlTools::Login_Email_Code_Check(ui.mailMailBoxLineEdit->text().toStdString(), ui.mailVerificationCodeLineEdit->text().toStdString());
	if (logIdentityType == 1)
	{
		qDebug() << "登录成功";
		this->hide();
		// 获取账号
		auto[account, password] = SqlTools::Login_Get_Account_Password
		(
			ui.mailMailBoxLineEdit->text().toStdString(),
			ui.mailVerificationCodeLineEdit->text().toStdString()
		);
		CashierWindowClass* cashierWindow = new CashierWindowClass(account, password);
		connect(cashierWindow, &CashierWindowClass::logoutRequested, this, &LoginWindow::show);  // 连接退出信号
		cashierWindow->show();
	}
	else if (logIdentityType == 2)
	{
		qDebug() << "登录成功";
		this->hide();
		ManagerWindowClass* managerWindow = new ManagerWindowClass;
		connect(managerWindow, &ManagerWindowClass::logoutRequested, this, &LoginWindow::show);  // 连接退出信号
		managerWindow->show();
	}
	else
	{
		qDebug() << "登录失败";
		// 显示错误提示
		QMessageBox::warning(this, "登录失败", "邮箱或验证码错误，请重试。");
		ui.mailMailBoxLineEdit->clear(); // 清空账号输入框
		ui.mailVerificationCodeLineEdit->clear(); // 清空密码输入框
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
			qDebug() << "注册成功";
			QMessageBox::information(this, "注册成功", "注册成功，请登录。");
			ui.stackedWidget->setCurrentIndex(0);
		}
		else
		{
			qDebug() << "注册失败";
			// 显示错误提示
			QMessageBox::warning(this, "注册失败", "服务器写入出错，请稍后再试。");
			ui.signAccountLineEdit->clear(); // 清空账号输入框
			ui.signPasswordLineEdit->clear(); // 清空密码输入框
			ui.signRePasswordLineEdit->clear(); // 清空密码输入框
			ui.signMailLineEdit->clear(); // 清空邮箱输入框
			ui.signVerificationCodeLineEdit->clear(); // 清空验证码输入框
		}
		
	}
	else
	{
		qDebug() << "注册失败";
		// 显示错误提示
		QMessageBox::warning(this, "注册失败", "输入有误，请重试。");
		ui.signAccountLineEdit->clear(); // 清空账号输入框
		ui.signPasswordLineEdit->clear(); // 清空密码输入框
		ui.signRePasswordLineEdit->clear(); // 清空密码输入框
		ui.signMailLineEdit->clear(); // 清空邮箱输入框
		ui.signVerificationCodeLineEdit->clear(); // 清空验证码输入框

	}
}

void LoginWindow::onSignCodeSentClicked()
{

}

void LoginWindow::onMailCodeSentClicked()
{

}
