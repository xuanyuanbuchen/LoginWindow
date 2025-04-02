#pragma once

#include <QWidget>
#include "ui_ManagerWindowClass.h"
#include "GoodsDetailDialog.h"
#include "GoodsWidget.h"
#include "GoodsWidgetPagination.h"
#include "TableViewPagination.h"
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
    void updateSalePage();
    void updateStaffPage();
    void updateGoodsPage();
    void onSaleChangedAcceptClicked();
    void onSaleChangedCancelClicked();
    void onSaleDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>());
    void onAddOrderClicked();
    void onStaffChangedAcceptClicked();
    void onStaffChangedCancelClicked();
    void onStaffDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>());
    void onAddStaffClicked();

private:
    Ui::ManagerWindowClass ui;
    std::vector<GoodsWidget*> vec_current_goods_widget;
	TableViewPagination* salePagination;
    TableViewPagination* staffPagination;
    GoodsWidgetPagination* goodsWidgetPagination;
};
