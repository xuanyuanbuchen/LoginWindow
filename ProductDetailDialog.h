#pragma once

#include <QDialog>
#include "ui_ProductDetailDialog.h"

class ProductDetailDialog : public QDialog
{
	Q_OBJECT

public:
	ProductDetailDialog(QWidget *parent = nullptr);
	ProductDetailDialog(QWidget* parent, const QString& id);
	~ProductDetailDialog();

	void setProductDetail(const QString& id, const QString& name, const QString& price, const QString& category,
		const QPixmap& image, const QString& description, const int count);


private:
	Ui::ProductDetailDialog ui;
};
