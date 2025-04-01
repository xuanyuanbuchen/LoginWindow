#pragma once

#include <QDialog>
#include "ui_GoodsDetailDialog.h"

class GoodsDetailDialog : public QDialog
{
	Q_OBJECT

public:
	GoodsDetailDialog(QWidget *parent = nullptr);
	GoodsDetailDialog(QWidget* parent, const QString& id);
	~GoodsDetailDialog();

	void setGoodsDetail(const QString& id, const QString& name, const QString& price, const QString& category,
		const QPixmap& image, const QString& description, const int count);

private:
	Ui::GoodsDetailDialog ui;
};
