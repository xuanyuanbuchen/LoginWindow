#pragma once

#include <QWidget>
#include "ui_CashierWindowClass.h"
#include "ProductWidget.h"
#include "TableViewPagination.h"
#include "ProductWidgetPagination.h"
#include <vector>
#include <memory>
#include "SqlTools.h"
#include "UnifiedLoginManager.h"

class CashierWindowClass : public QWidget
{
    Q_OBJECT

public:
    CashierWindowClass(QWidget* parent = nullptr);
    CashierWindowClass(std::string account, std::string password, QWidget* parent = nullptr);
    ~CashierWindowClass();
signals:
    void logoutRequested();  // 退出信号

private:
	void InitBackground();
	void InitConnect();
	void InitProductWidget();
	void InitOrderTableView();

private slots:
    void onLogoutClicked();
    void onNextPageClicked();
    void onPrevPageClicked();
    void onProductClicked();
    void updateProductPage();  // 更新商品页面
    void onProductNameLineSearchClicked (const QString& text); 
	void onProcuctCategoryComboxChanged(int index);
    void onProcuctPriceComboxChanged(int index);

	void updateSalesInformationPage();  // 更新销售信息页面
    void onSalesNameLineSearchClicked(const QString& text);
    void onSalesCategoryComboxChanged(int index);
    void onSalesPriceComboxChanged(int index);

    void updateOrderPage();

    void onOrderHeaderClicked(int column);
private:
    Ui::CashierWindowClass ui;
    std::vector<std::unique_ptr<ProductWidget>> vec_current_produc_widget;
    int currentPage;
    int totalPages;
    TableViewPagination* orderPagination;
    ProductWidgetPagination* productWidgetPagination;
	int productWidgetPageSize = 9; // 每页显示的商品数量
	std::string account;
	std::string password;

	int orderPageSize = 30; // 每页显示的订单数量
	int orderPageCount = 0; // 总页数

    void loadPage(int page);
    // 标记订单表格的列排序状态
    bool orderReverseFlags[5] = { false, false, false, false, false }; // 对应订单表格的 5 列
    int currentOrderSortColumn = -1; // 当前排序的列索引，-1 表示未排序

    // 标记商品表格的列排序状态
    bool productReverseFlags[7] = { false, false, false, false, false, false, false }; // 对应商品表格的 7 列
    int currentProductSortColumn = -1; // 当前排序的列索引，-1 表示未排序

    
};
