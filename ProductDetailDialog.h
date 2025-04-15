#pragma once

#include <QDialog>
#include "ui_ProductDetailDialog.h"

class ProductDetailDialog : public QDialog
{
	Q_OBJECT

public:
	ProductDetailDialog(QWidget *parent = nullptr);
	ProductDetailDialog(QWidget* parent = nullptr, const QString& path = ":/res/default.jpg", const QString& ID = "NULL", const QString& name = "NULL", const QString& price = "NULL", const QString& category = "NULL", const QString& count = "NULL", const QString& description = "NULL");
	~ProductDetailDialog();

	void setProductDetail(const QString& id, const QString& name, const QString& price, const QString& category,
		const QPixmap& image, const QString& description, const QString& count);


private:
	Ui::ProductDetailDialog ui;
};
