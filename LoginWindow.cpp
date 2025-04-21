#include "LoginWindow.h"
#include <QDir>
#include <QDebug>
#include "CashierWindowClass.h"
#include "ManagerWindowClass.h"
#include "SqlTools.h"
#include <qmessagebox.h>
#include <QTimer>


LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	InitBackground();
	InitConnect();
}

void LoginWindow::onLoginClicked() {
	int userType = 0;
	if (UnifiedLoginManager::instance().localPasswordLogin(ui.logAccountLineEdit->text(), ui.logPasswordLineEdit->text(), userType)) {
		this->hide();
		if (userType == 1) {
			ManagerWindowClass* managerWindow = new ManagerWindowClass;
			connect(managerWindow, &ManagerWindowClass::logoutRequested, this, &LoginWindow::show);
			managerWindow->show();
		}
		else if (userType == 2) {
			CashierWindowClass* cashierWindow = new CashierWindowClass(
				ui.logAccountLineEdit->text().toStdString(),
				ui.logPasswordLineEdit->text().toStdString()
			);
			connect(cashierWindow, &CashierWindowClass::logoutRequested, this, &LoginWindow::show);
			cashierWindow->show();
		}
	}
	else {
		QMessageBox::warning(this, "登录失败", "账号或密码错误，请重试。");
		ui.logAccountLineEdit->clear();
		ui.logPasswordLineEdit->clear();
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
	connect(ui.signAccountLineEdit, &QLineEdit::textChanged, [=]() {
		ui.signMailLineEdit->setText(ui.signAccountLineEdit->text());
		});
	connect(ui.signMailLineEdit, &QLineEdit::textChanged, [=]() {
		ui.signAccountLineEdit->setText(ui.signMailLineEdit->text());
		});
}

void LoginWindow::onMailLoginClicked() {
	int userType = 0;
	if (UnifiedLoginManager::instance().localEmailLogin(ui.mailMailBoxLineEdit->text(), ui.mailVerificationCodeLineEdit->text(), userType)) {
		this->hide();
		if (userType == 1) {
			ManagerWindowClass* managerWindow = new ManagerWindowClass;
			connect(managerWindow, &ManagerWindowClass::logoutRequested, this, &LoginWindow::show);
			managerWindow->show();
		}
		else if (userType == 2) {
			auto [account, password] = SqlTools::Login_Get_Account_Password(
				ui.mailMailBoxLineEdit->text().toStdString(),
				ui.mailVerificationCodeLineEdit->text().toStdString()
			);
			CashierWindowClass* cashierWindow = new CashierWindowClass(account, password);
			connect(cashierWindow, &CashierWindowClass::logoutRequested, this, &LoginWindow::show);
			cashierWindow->show();
		}
	}
	else {
		QMessageBox::warning(this, "登录失败", "邮箱或验证码错误，请重试。");
		ui.mailMailBoxLineEdit->clear();
		ui.mailVerificationCodeLineEdit->clear();
	}
}

void LoginWindow::onRegisterClicked()
{
	if (
		 SqlTools::Register_Email_Code_Check(ui.signMailLineEdit->text().toStdString(), ui.signVerificationCodeLineEdit->text().toStdString())
		&& SqlTools::Register_Account_Password_Check(ui.signAccountLineEdit->text().toStdString(), ui.signPasswordLineEdit->text().toStdString())
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

void LoginWindow::onSignCodeSentClicked() {
	QString email = ui.signMailLineEdit->text().trimmed();
	if (email.isEmpty()) {
		QMessageBox::warning(this, "错误", "请输入邮箱地址！");
		return;
	}

	// 调用 UnifiedLoginManager 发送验证码
	UnifiedLoginManager::instance().sendLocalVerificationCode(email);

	// 禁用按钮并启动倒计时
	ui.signSendCodeBtn->setEnabled(false);
	QTimer* timer = new QTimer(this);
	int countdown = 60; // 倒计时 60 秒
	ui.signSendCodeBtn->setText(QString("%1s").arg(countdown));

	connect(timer, &QTimer::timeout, this, [=]() mutable {
		countdown--;
		if (countdown > 0) {
			ui.signSendCodeBtn->setText(QString("%1s").arg(countdown));
		}
		else {
			timer->stop();
			ui.signSendCodeBtn->setEnabled(true);
			ui.signSendCodeBtn->setText("发送验证码");
			timer->deleteLater();
		}
		});

	timer->start(1000); // 每秒更新一次
}

void LoginWindow::onMailCodeSentClicked() {
	QString email = ui.mailMailBoxLineEdit->text().trimmed();
	if (email.isEmpty()) {
		QMessageBox::warning(this, "错误", "请输入邮箱地址！");
		return;
	}

	// 调用 UnifiedLoginManager 发送验证码
	UnifiedLoginManager::instance().sendLocalVerificationCode(email);

	// 禁用按钮并启动倒计时
	ui.mailSendCodeBtn->setEnabled(false);
	QTimer* timer = new QTimer(this);
	int countdown = 60; // 倒计时 60 秒
	ui.mailSendCodeBtn->setText(QString("%1s").arg(countdown));

	connect(timer, &QTimer::timeout, this, [=]() mutable {
		countdown--;
		if (countdown > 0) {
			ui.mailSendCodeBtn->setText(QString("%1s").arg(countdown));
		}
		else {
			timer->stop();
			ui.mailSendCodeBtn->setEnabled(true);
			ui.mailSendCodeBtn->setText("发送验证码");
			timer->deleteLater();
		}
		});

	timer->start(1000); // 每秒更新一次
}
