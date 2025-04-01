#pragma once

#include <QWidget>
#include "ui_ManagerWindowClass.h"
#include "GoodsDetailDialog.h"
#include "GoodsWidget.h"
#include "GoodsWidgetPagination.h"
#include <vector>

class ManagerWindowClass : public QWidget
{
    Q_OBJECT

public:
    ManagerWindowClass(QWidget* parent = nullptr);
    ~ManagerWindowClass();

signals:
    void logoutRequested();  // 退出信号

private slots:
    void onLogoutClicked();
    void onGoodsClicked();
    void onNextPageClicked();
    void onPrevPageClicked();
    void updateGoodsPage();

private:
    Ui::ManagerWindowClass ui;
    std::vector<GoodsWidget*> vec_current_goods_widget;
    GoodsWidgetPagination* goodsWidgetPagination;
};
