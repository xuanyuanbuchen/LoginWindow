#pragma once

#include <QWidget>
#include "ui_CashierWindowClass.h"
#include "ProductWidget.h"
#include "TableViewPagination.h"
#include "ProductWidgetPagination.h"
#include <vector>

class CashierWindowClass : public QWidget
{
    Q_OBJECT

public:
    CashierWindowClass(QWidget* parent = nullptr);
    ~CashierWindowClass();
signals:
    void logoutRequested();  // 退出信号

private slots:
    void onLogoutClicked();
    void onNextPageClicked();
    void onPrevPageClicked();
    void onProductClicked();
    void updateOrderPage();
    void updateProductPage();  // 更新商品页面

private:
    Ui::CashierWindowClass ui;
    std::vector<ProductWidget*> vec_current_produc_widget;
    int currentPage;
    int totalPages;
    TableViewPagination* orderPagination;
    ProductWidgetPagination* productWidgetPagination;

    void loadPage(int page);
    
};
