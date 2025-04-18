﻿#include "ProductDetailDialog.h"
#include "ProductDetailDialog.h"
#include <string>

ProductDetailDialog::ProductDetailDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.productTextEdit->setReadOnly(true);
}

ProductDetailDialog::ProductDetailDialog(QWidget* parent, const QString& path, const QString& ID, const QString& name, const QString& price, const QString& category, const QString& count, const QString& description)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.productPicture->setPixmap(path);
	ui.productID2->setText(ID);
	ui.productName2->setText(name);
	ui.productCategory2->setText(category);
	ui.productCount2->setText(count);
	ui.productDescription->setText(description);
	ui.productTextEdit->setReadOnly(true);

	setProductDetail(ID, name, price, category, path, description, count);
}

ProductDetailDialog::~ProductDetailDialog()
{
}

void ProductDetailDialog::setProductDetail(const QString& id, const QString& name, const QString& price, const QString& category,
	const QPixmap& image, const QString& description, const QString& count)
{
	ui.productPicture->setPixmap(image.scaled(200, 200, Qt::KeepAspectRatio));
	ui.productID2->setText(id);
	ui.productName2->setText(name);
	ui.productPrice2->setText(price);
	ui.productCategory2->setText(category);
	ui.productCount2->setText(count);
	ui.productDescription->setText(description);
}
