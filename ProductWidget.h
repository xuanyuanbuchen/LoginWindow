#pragma once

#include <QWidget>
#include "ProductDetailDialog.h"
#include "ui_ProductWidget.h"

class ProductWidget : public QWidget
{
	Q_OBJECT

public:
	ProductWidget(QWidget *parent = nullptr);
	~ProductWidget();

	void setProductInfo(const QString& name, const QString& price, const QPixmap& image);
signals:
	void clicked();  // µã»÷ÐÅºÅ

protected:
	void mousePressEvent(QMouseEvent* event) override;

private:
	Ui::ProductWidget ui;
	QString id;
};
