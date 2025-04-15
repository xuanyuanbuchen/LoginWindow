#pragma once

#include <QWidget>
#include "ui_CashierWindowClass.h"
#include "ProductWidget.h"
#include "TableViewPagination.h"
#include "ProductWidgetPagination.h"
#include <vector>
#include <memory>

class CashierWindowClass : public QWidget
{
    Q_OBJECT

public:
    CashierWindowClass(QWidget* parent = nullptr);
    CashierWindowClass(std::string account, std::string password, QWidget* parent = nullptr);
    ~CashierWindowClass();
signals:
    void logoutRequested();  // �˳��ź�

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
    void updateOrderPage();
    void updateProductPage();  // ������Ʒҳ��
	void updateSalesInformationPage();  // ����������Ϣҳ��
	void onCategoryComboxChanged(int index);

private:
    Ui::CashierWindowClass ui;
    std::vector<std::unique_ptr<ProductWidget>> vec_current_produc_widget;
    int currentPage;
    int totalPages;
    TableViewPagination* orderPagination;
    ProductWidgetPagination* productWidgetPagination;
	int productWidgetPageSize = 9; // ÿҳ��ʾ����Ʒ����
	std::string account;
	std::string password;

    void loadPage(int page);
    
};
