#pragma once

#include <QWidget>
#include "ui_CashierWindowClass.h"
#include "ProductWidget.h"
#include "OrderPagination.h"
#include <vector>

class CashierWindowClass : public QWidget
{
	Q_OBJECT

public:
	CashierWindowClass(QWidget *parent = nullptr);
	~CashierWindowClass();
signals:
	void logoutRequested();  // ÍË³öÐÅºÅ

private slots:
	void onLogoutClicked();
	void onNextPageClicked();
	void onPrevPageClicked();
	void onProductClicked();
	void updateOrderPage();

private:
	Ui::CashierWindowClass ui;
	std::vector<ProductWidget*> vec_current_produc_widget;
	int currentPage;
	int totalPages;
	OrderPagination* orderPagination;

	void loadPage(int page);
};
