#pragma once

#include <QWidget>
#include "ui_GoodsWidget.h"
#include <GoodsDetailDialog.h>

class GoodsWidget : public QWidget
{
    Q_OBJECT

public:
	GoodsWidget
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
    ~GoodsWidget();
    void setProductInfo(const QString& name, const QString& price, const QString& count, const QPixmap& image);
    void showGoodsDetailDialog();
    const QString& getID();

signals:
    void clicked();  // µã»÷ÐÅºÅ

protected:
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void onPictureChanged(const QPixmap& newImage);

private:
    Ui::GoodsWidget ui;
    QString id;
    GoodsDetailDialog* goodsDetailDialog;
};
