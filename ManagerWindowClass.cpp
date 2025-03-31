#include "ManagerWindowClass.h"

ManagerWindowClass::ManagerWindowClass(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.stackedWidget->setCurrentIndex(0);
	// ���ô��ڱ���ͼƬ
	QPixmap background(":/res/manager.png");  // ����Դ�ļ��м���ͼƬ
	background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // ����ͼƬ����Ӧ����
	QPalette palette;
	palette.setBrush(QPalette::Window, background);  // ���ñ���ͼƬ
	this->setPalette(palette);
	//���õǳ���ť
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