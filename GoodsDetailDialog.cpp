#include "GoodsDetailDialog.h"

GoodsDetailDialog::GoodsDetailDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

GoodsDetailDialog::GoodsDetailDialog(QWidget* parent, const QString& id)
{
	ui.setupUi(this);
	ui.goodsIDEdit->setText(id);
	connect(ui.cancelBtn, &QPushButton::clicked, [=]() {
		this->close();
		//以后有了数据库应该调用setGoodsDetail
		ui.goodsCategoryEdit->setText("NULL");
		ui.goodsCountEdit->setText("NULL");
		ui.goodsIDEdit->setText("NULL");
		ui.goodsNameEdit->setText("NULL");
		ui.goodsPriceEdit->setText("NULL");
		ui.goodsTextEdit->setText("NULL");
		});
	connect(ui.acceptBtn, &QPushButton::clicked, [=]() {
		this->close();
		//以后有了数据库应该在这里写回数据库
		});
}

GoodsDetailDialog::~GoodsDetailDialog()
{
}

void GoodsDetailDialog::setGoodsDetail(const QString& id, const QString& name, const QString& price, const QString& category, const QPixmap& image, const QString& description, const int count)
{
	ui.goodsPicture->setPixmap(image.scaled(200, 200, Qt::KeepAspectRatio));
	ui.goodsIDEdit->setText(id);
	ui.goodsNameEdit->setText(name);
	ui.goodsPriceEdit->setText(price);
	ui.goodsCategoryEdit->setText(category);
	ui.goodsCountEdit->setText(QString::number(count));
	ui.goodsDescription->setText(description);
}
