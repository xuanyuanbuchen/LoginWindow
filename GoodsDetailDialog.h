#pragma once

#include <QDialog>
#include "ui_GoodsDetailDialog.h"

class GoodsDetailDialog : public QDialog
{
    Q_OBJECT

public:
    GoodsDetailDialog(QWidget* parent = nullptr);
    GoodsDetailDialog(QWidget* parent, const QString& id);
    ~GoodsDetailDialog();

    void setGoodsDetail(const QString& id, const QString& name, const QString& price, const QString& category,
        const QString& path, const QString& description, const int count);
    Ui::GoodsDetailDialog& getUi();
	std::string GetPath() const { return pixpath; } // 获取图片路径

signals:
    void pictureChanged(const QPixmap& newImage);

private slots:
    void onGoodsPictureClicked();

private:
    Ui::GoodsDetailDialog ui;
    std::string pixpath;
};
