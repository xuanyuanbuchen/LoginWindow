#include "GoodsWidget.h"

GoodsWidget::GoodsWidget(QWidget* parent, const QString& path, const QString& ID, const QString& name, const QString& price, const QString& category, const QString& count, const QString& description)
{
	ui.setupUi(this);
	id = ID;
	QPalette pe;
	pe.setColor(QPalette::WindowText, Qt::red);
	ui.goodsPriceText->setPalette(pe);
	ui.goodsCountText->setPalette(pe);
	setProductInfo(name, "价格：" + price + "元", "库存：" + count, path);
	goodsDetailDialog = new GoodsDetailDialog(this);
	connect(goodsDetailDialog, &GoodsDetailDialog::pictureChanged, this, &GoodsWidget::onPictureChanged);
	goodsDetailDialog->hide();
	goodsDetailDialog->setGoodsDetail(ID, name, price, category, path, description, 0);
}

GoodsWidget::~GoodsWidget()
{
}

void GoodsWidget::setProductInfo(const QString& name, const QString& price, const QString& count, const QString& path)
{
    ui.goodsNameText->setText(name);
    ui.goodsPriceText->setText(price);
    ui.goodsCountText->setText(count);
    ui.goodsPicture->setPixmap(path);
}

void GoodsWidget::showGoodsDetailDialog()
{
    goodsDetailDialog->exec();
}

const QString& GoodsWidget::getID()
{
	return this->id;
}
void GoodsWidget::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    emit clicked();
}

void GoodsWidget::onPictureChanged(const QPixmap& newImage)
{
    ui.goodsPicture->setPixmap(newImage.scaled(200, 200, Qt::KeepAspectRatio));
}
