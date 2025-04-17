#include "CustomerDetailDialog.h"
#include <QFileDialog>
#include <QPixmap>

CustomerDetailDialog::CustomerDetailDialog(QWidget* parent)
	: QDialog(parent), avatarPath(":/res/default.jpg") // 初始化头像路径为默认图片
{
	ui.setupUi(this);

	// 连接 acceptBtn 和 cancelBtn 的点击信号到槽函数
	connect(ui.acceptBtn, &QPushButton::clicked, this, &CustomerDetailDialog::onAcceptBtnClicked);
	connect(ui.cancelBtn, &QPushButton::clicked, this, &CustomerDetailDialog::onCancelBtnClicked);

	// 连接双击信号到槽函数
	connect(ui.customerPicture, &ClickableLabel::doubleClicked, this, &CustomerDetailDialog::onCustomerPictureDoubleClicked);
}

CustomerDetailDialog::CustomerDetailDialog(QWidget* parent, const QString& id, const QString& birthDate,
	const QString& logDate, const QString& email,
	const QString& password, const QString& description,
	const QString& avatarPath)
	: QDialog(parent), avatarPath(avatarPath)
{
	ui.setupUi(this);

	// 设置初始数据
	ui.customerIDEdit->setText(id);
	ui.customerBirthDateEdit->setText(birthDate);
	ui.customerLogDateEdit->setText(logDate);
	ui.customerEmailEdit->setText(email);
	ui.customerPasswordEdit->setText(password);
	ui.customerTextEdit->setPlainText(description);

	// 设置头像
	QPixmap avatar(avatarPath);
	if (!avatar.isNull()) {
		ui.customerPicture->setPixmap(avatar);
	}

	// 连接按钮和槽函数
	connect(ui.acceptBtn, &QPushButton::clicked, this, &CustomerDetailDialog::onAcceptBtnClicked);
	connect(ui.cancelBtn, &QPushButton::clicked, this, &CustomerDetailDialog::onCancelBtnClicked);
	connect(ui.customerPicture, &ClickableLabel::doubleClicked, this, &CustomerDetailDialog::onCustomerPictureDoubleClicked);
}

CustomerDetailDialog::~CustomerDetailDialog()
{
}

void CustomerDetailDialog::onAcceptBtnClicked()
{
	// 收集用户输入的数据
	QString id = ui.customerIDEdit->text();
	QString birthDate = ui.customerBirthDateEdit->text();
	QString logDate = ui.customerLogDateEdit->text();
	QString email = ui.customerEmailEdit->text();
	QString password = ui.customerPasswordEdit->text();
	QString description = ui.customerTextEdit->toPlainText();

	// 发射信号，将数据返回给外部调用者
	emit dataAccepted(id, birthDate, logDate, email, password, description, avatarPath);

	// 关闭窗口
	close();
}

void CustomerDetailDialog::onCancelBtnClicked()
{
	// 关闭窗口并取消所有修改
	close();
}

void CustomerDetailDialog::onCustomerPictureDoubleClicked()
{
	// 弹出文件选择窗口
	QString filePath = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("Images (*.png *.xpm *.jpg *.jpeg *.bmp)"));
	if (!filePath.isEmpty())
	{
		// 加载并设置新图片
		QPixmap newPixmap(filePath);
		if (!newPixmap.isNull())
		{
			ui.customerPicture->setPixmap(newPixmap);
			avatarPath = filePath; // 保存选择的图片路径
		}
	}
}
