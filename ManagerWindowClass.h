#pragma once

#include <QWidget>
#include "ui_ManagerWindowClass.h"
#include "GoodsDetailDialog.h"
#include "GoodsWidget.h"
#include "GoodsWidgetPagination.h"
#include "TableViewPagination.h"
#include <vector>
#include <QStandardItemModel>

//struct FinancialData {
//    QList<QPointF> income;  // 收入数据（x: 时间戳，y: 金额）
//    QList<QPointF> expense; // 支出数据
//};

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
    void onDeleteGoodsClicked();
    void onDeleteSaleClicked();
    void onDeleteStaffClicked();
    void onCustomerChangedAcceptClicked();
    void onCustomerChangedCancelClicked();
    void onAddCustomerClicked();
    void onDeleteCustomerClicked();
    void onCustomerDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>());
    void onCustomerAvatarDoubleClicked(const QModelIndex& index);
    void updateCustomerPage();

    

private:
    Ui::ManagerWindowClass ui;
    std::vector<GoodsWidget*> vec_current_goods_widget;
	TableViewPagination* salePagination;
    TableViewPagination* staffPagination;
    GoodsWidgetPagination* goodsWidgetPagination;
    QStandardItemModel* customerModel;
    TableViewPagination* customerPagination;
};
