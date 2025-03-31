#include "ManagerWindowClass.h"

ManagerWindowClass::ManagerWindowClass(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.stackedWidget->setCurrentIndex(0);
	// 设置窗口背景图片
	QPixmap background(":/res/manager.png");  // 从资源文件中加载图片
	background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 缩放图片以适应窗口
	QPalette palette;
	palette.setBrush(QPalette::Window, background);  // 设置背景图片
	this->setPalette(palette);
	//设置登出按钮
	connect(ui.logOutBtn, &QPushButton::clicked, this, &ManagerWindowClass::onLogoutClicked);
}

ManagerWindowClass::~ManagerWindowClass()
{
}

void ManagerWindowClass::onLogoutClicked()
{
	emit logoutRequested();
	this->close();
}