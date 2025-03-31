#pragma once

#include <QWidget>
#include "ui_GoodsWidget.h"

class GoodsWidget : public QWidget
{
	Q_OBJECT

public:
	GoodsWidget(QWidget *parent = nullptr);
	~GoodsWidget();
	void setProductInfo(const QString& name, const QString& price, const QString& count, const QPixmap& image);
signals:
	void clicked();  // µã»÷ÐÅºÅ

protected:
	void mousePressEvent(QMouseEvent* event) override;

private:
	Ui::GoodsWidget ui;
	QString id;
};
