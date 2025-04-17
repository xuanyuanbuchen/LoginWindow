#include "GoodsDetailDialog.h"
#include "ClickableLabel.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

GoodsDetailDialog::GoodsDetailDialog(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.cancelBtn, &QPushButton::clicked, [=]() {
        this->close();
        //以后有了数据库应该调用setGoodsDetail
        ui.goodsCategoryEdit->setText("NULL");
        ui.goodsCountEdit->setText("NULL");
        ui.goodsIDEdit->setText("NULL");
        ui.goodsNameEdit->setText("NULL");
        ui.goodsPriceEdit->setText("NULL");
        ui.goodsTextEdit->setText("NULL");
        ui.goodsPicture->setPixmap(QPixmap(":/res/default.jpg").scaled(200, 200, Qt::KeepAspectRatio));
        emit pictureChanged(QPixmap(":/res/default.jpg"));
        });
    connect(ui.acceptBtn, &QPushButton::clicked, [=]() {
        this->close();
        //以后有了数据库应该在这里写回数据库
        });
    //点击图片更换图片
    connect(ui.goodsPicture, &ClickableLabel::clicked, this, &GoodsDetailDialog::onGoodsPictureClicked);
}

GoodsDetailDialog::GoodsDetailDialog(QWidget* parent, const QString& id)
    : QDialog(parent)
{
    ui.setupUi(this);
    ui.goodsIDEdit->setText(id);
    connect(ui.cancelBtn, &QPushButton::clicked, [=]() {
        this->close();
        //以后有了数据库应该调用setGoodsDetail
        ui.goodsCategoryEdit->setText("NULL");
        ui.goodsCountEdit->setText("NULL");
        ui.goodsIDEdit->setText("NULL");
        ui.goodsNameEdit->setText("NULL");
        ui.goodsPriceEdit->setText("NULL");
        ui.goodsTextEdit->setText("NULL");
        });
    connect(ui.acceptBtn, &QPushButton::clicked, [=]() {
        this->close();
        //以后有了数据库应该在这里写回数据库
        });
	//点击图片更换图片
    connect(ui.goodsPicture, &ClickableLabel::clicked, this, &GoodsDetailDialog::onGoodsPictureClicked);
}

GoodsDetailDialog::~GoodsDetailDialog()
{
}

void GoodsDetailDialog::setGoodsDetail(const QString& id, const QString& name, const QString& price, const QString& category, const QString& path, const QString& description, const int count)
{
    ui.goodsPicture->setPixmap(path);
    ui.goodsIDEdit->setText(id);
    ui.goodsNameEdit->setText(name);
    ui.goodsPriceEdit->setText(price);
    ui.goodsCategoryEdit->setText(category);
    ui.goodsCountEdit->setText(QString::number(count));
    ui.goodsDescription->setText(description);
	pixpath = path.toStdString();
}

Ui::GoodsDetailDialog& GoodsDetailDialog::getUi()
{
    return ui;
}

void GoodsDetailDialog::onGoodsPictureClicked()
{
    QString imagePath = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    if (!imagePath.isEmpty())
    {
        QPixmap newImage(imagePath);
        ui.goodsPicture->setPixmap(newImage.scaled(200, 200, Qt::KeepAspectRatio));
        emit pictureChanged(newImage);
    }
}
