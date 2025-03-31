#include "GoodsWidget.h"

GoodsWidget::GoodsWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	id = "null";
	QPalette pe;
	pe.setColor(QPalette::WindowText, Qt::red);
	ui.goodsPriceText->setPalette(pe);
	ui.goodsCountText->setPalette(pe);
}

GoodsWidget::~GoodsWidget()
{
}

void GoodsWidget::setProductInfo(const QString& name, const QString& price, const QString& count ,const QPixmap& image)
{
	ui.goodsNameText->setText(name);
	ui.goodsPriceText->setText(price);
	ui.goodsCountText->setText(count);
	ui.goodsPicture->setPixmap(image.scaled(200, 200, Qt::KeepAspectRatio));
}

void GoodsWidget::mousePressEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	emit clicked();
}
