#pragma once

#include <QWidget>
#include "ProductDetailDialog.h"
#include "ui_ProductWidget.h"
#include "ProductDetailDialog.h"

class ProductWidget : public QWidget
{
	Q_OBJECT

public:
	ProductWidget
	(
		QWidget* parent = nullptr,
		const QString& path = ":/res/default.jpg",
		const QString& ID = "NULL",
		const QString& name = "NULL",
		const QString& price = "NULL",
		const QString& category = "NULL",
		const QString& count = "NULL",
		const QString& description = "NULL"
	);
	~ProductWidget();

	void setProductInfo(const QString& name, const QString& price, const QPixmap& image);
	void showProductDetailDialog();
signals:
	void clicked();  // µã»÷ÐÅºÅ

protected:
	void mousePressEvent(QMouseEvent* event) override;

private:
	Ui::ProductWidget ui;
	QString id;
	ProductDetailDialog* productDetailDialog;
};