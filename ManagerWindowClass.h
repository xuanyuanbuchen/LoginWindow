#pragma once

#include <QWidget>
#include "ui_ManagerWindowClass.h"
#include "GoodsDetailDialog.h"
#include "GoodsWidget.h"
#include "GoodsWidgetPagination.h"
#include "TableViewPagination.h"
#include <vector>
#include <QStandardItemModel>
#include <memory>
#include "SqlTools.h"

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

private:
	void InitBackground();
	void InitConnect();
    void InitManagerGoodsPage();
	void InitProcessSalesPage();
	void InitProcessStockInfoPage();
    void InitManagerStaffPage();
    void InitManagerCustomPage();
	void InitFinancialManagerPage();


signals:
    void logoutRequested();  // 退出信号

private slots:
    void onLogoutClicked();
    void onNextPageClicked();
    void onPrevPageClicked();
    //库存管理
    void onGoodsClicked();
    void updateGoodsPage();
    void onGoodsComboxCategoryClicked(int index);
	void onGoodsComboxPriceClicked(int index);
    void onGoodstNameLineSearchClicked(const QString& text);
	void onAddGoodsClicked(const QString& id, int count);
	void onDeleteGoodsClicked(const QString& id);
    //销售数据
    void updateSalePage();
    void onSaleChangedAcceptClicked();
    void onSaleChangedCancelClicked();
    void onSaleDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>());
    void onSaleComboxCategoryClicked(int index);
    void onSaleComboxPriceClicked(int index);
    void onAddSaleClicked();
	void onDeleteSaleClicked(const QString& id, int count);
    void onSaleNameLineSearchClicked(const QString& text);
    //进货数据
    void updateStockPage();
    void onAddStockClicked();
    void onStockChangedAcceptClicked();
    void onStockChangedCancelClicked();
    void onStockDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>());
    void onStockComboxCategoryClicked(int index);
    void onStockComboxPriceClicked(int index);
    void onStockNameLineSearchClicked(const QString& text);
    void onDeleteStockClicked(const QString& id, int count);

    //员工管理
    void updateStaffPage();
    void onAddStaffClicked();
    void onStaffChangedAcceptClicked();
    void onStaffChangedCancelClicked();
    void onStaffDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>());
    void onStaffComboxCategoryClicked(int index);
    void onstaffNameLineSearchClicked(const QString& text);
    void onDeleteStaffClicked();

    //顾客管理
    void updateCustomerPage();
    void onAddCustomerClicked();
    void onCustomerChangedAcceptClicked();
    void onCustomerChangedCancelClicked();
    void onCustomerDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>());
    void onCustomerNameLineSearchClicked(const QString& text);
    void onCustomerAvatarDoubleClicked(const QModelIndex& index);
    void onDeleteCustomerClicked();

    //财务管理
    void onDateRangeChanged();

    void onSaleHeaderClicked(int column);
    void onStockHeaderClicked(int column);
    void onStaffHeaderClicked(int column);
    void onCustomerHeaderClicked(int column);


private:
    Ui::ManagerWindowClass ui;
    std::vector<std::unique_ptr<GoodsWidget>> vec_current_goods_widget;
	TableViewPagination* salePagination;
    TableViewPagination* stockPagination;

    TableViewPagination* staffPagination;
    GoodsWidgetPagination* goodsWidgetPagination;
    QStandardItemModel* customerModel;
    TableViewPagination* customerPagination;

    int goodsWidgetPageSize = 9; // 每页显示的商品数量
    std::string account;
    std::string password;

    std::vector < std::unique_ptr<SaleTableLine> > vec_changed_sale_data;
    std::vector < std::unique_ptr<StockDetail> > vec_changed_stock_data;
    std::vector < std::unique_ptr<StaffDetail> > vec_changed_Staff_data;
    std::vector < std::unique_ptr<CustomerDetail> > vec_changed_Customer_data;


	int salePageSize = 30; // 每页显示的销售数据数量
	int salePageCount = 0; // 销售数据总页数
	int stockPageSize = 30; // 每页显示的进货数据数量
	int stockPageCount = 0; // 进货数据总页数
	int staffPageSize = 30; // 每页显示的员工数据数量
	int staffPageCount = 0; // 员工数据总页数
	int customerPageSize = 30; // 每页显示的客户数据数量
	int customerPageCount = 0; // 客户数据总页数

    // 标记销售表格的列排序状态
    bool saleReverseFlags[5] = { false, false, false, false, false }; // 对应销售表格的 5 列
    int currentSaleSortColumn = -1; // 当前排序的列索引，-1 表示未排序

    // 标记进货表格的列排序状态
    bool stockReverseFlags[4] = { false, false, false, false }; // 对应进货表格的 4 列
    int currentStockSortColumn = -1; // 当前排序的列索引，-1 表示未排序

    // 标记员工表格的列排序状态
    bool staffReverseFlags[6] = { false, false, false, false, false, false }; // 对应员工表格的 6 列
    int currentStaffSortColumn = -1; // 当前排序的列索引，-1 表示未排序

    // 标记顾客表格的列排序状态
    bool customerReverseFlags[7] = { false, false, false, false, false, false, false }; // 对应顾客表格的 7 列
    int currentCustomerSortColumn = -1; // 当前排序的列索引，-1 表示未排序


};
