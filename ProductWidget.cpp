#include "ProductWidget.h"

ProductWidget::ProductWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	id = "null";
	QPalette pe;
	pe.setColor(QPalette::WindowText, Qt::red);
	ui.productPriceText->setPalette(pe);
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

void ProductWidget::mousePressEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	emit clicked();
}
