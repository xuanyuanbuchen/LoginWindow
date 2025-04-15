#include "ProductWidget.h"

ProductWidget::ProductWidget(QWidget* parent, const QString& path, const QString& ID, const QString& name, const QString& price, const QString& category, const QString& count, const QString& description)
	: QWidget(parent)
{
	ui.setupUi(this);
	QPalette pe;
	pe.setColor(QPalette::WindowText, Qt::red);
	ui.productPriceText->setPalette(pe);
	setProductInfo(name, price, path);
	productDetailDialog = new ProductDetailDialog(this, path, ID, name, price, category, count, description);

	productDetailDialog->hide();
}

ProductWidget::~ProductWidget()
{
	
}

void ProductWidget::setProductInfo(const QString& name, const QString& price, const QPixmap& image)
{
	ui.productNameText->setText(name);
	ui.productPriceText->setText(price);
	ui.productPicture->setPixmap(image.scaled(200, 200, Qt::KeepAspectRatio));
}

void ProductWidget::showProductDetailDialog()
{
	productDetailDialog->exec();
}

void ProductWidget::mousePressEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	emit clicked();
}
