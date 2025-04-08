#include "GoodsWidget.h"

GoodsWidget::GoodsWidget(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    id = "null";
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::red);
    ui.goodsPriceText->setPalette(pe);
    ui.goodsCountText->setPalette(pe);
    goodsDetailDialog = new GoodsDetailDialog(this);
    connect(goodsDetailDialog, &GoodsDetailDialog::pictureChanged, this, &GoodsWidget::onPictureChanged);
    goodsDetailDialog->hide();
}

GoodsWidget::GoodsWidget(const QString& path, QWidget* parent)
{
	ui.setupUi(this);
	id = "null";
	QPalette pe;
	pe.setColor(QPalette::WindowText, Qt::red);
	ui.goodsPriceText->setPalette(pe);
	ui.goodsCountText->setPalette(pe);
	goodsDetailDialog = new GoodsDetailDialog(this);
	connect(goodsDetailDialog, &GoodsDetailDialog::pictureChanged, this, &GoodsWidget::onPictureChanged);
	goodsDetailDialog->hide();
	QPixmap image(path);
	ui.goodsPicture->setPixmap(image.scaled(200, 200, Qt::KeepAspectRatio));
	goodsDetailDialog->setGoodsDetail("null", "null", "null", "null", image, "null", 0);
}

GoodsWidget::~GoodsWidget()
{
}

void GoodsWidget::setProductInfo(const QString& name, const QString& price, const QString& count, const QPixmap& image)
{
    ui.goodsNameText->setText(name);
    ui.goodsPriceText->setText(price);
    ui.goodsCountText->setText(count);
    ui.goodsPicture->setPixmap(image.scaled(200, 200, Qt::KeepAspectRatio));
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
