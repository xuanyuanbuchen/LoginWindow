#include "CashierWindowClass.h"
#include <qstandarditemmodel.h>
#include <qtimezone.h>
#include "SqlTools.h"
#include <qmessagebox.h>

CashierWindowClass::CashierWindowClass(QWidget *parent)
    : QWidget(parent), currentPage(0), totalPages(0)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui.stackedWidget->setCurrentIndex(0);
	InitBackground();
	InitConnect();
	InitProductWidget();
    InitOrderTableView();
  
    

  
   
}

CashierWindowClass::CashierWindowClass(std::string account, std::string password, QWidget* parent)
    : QWidget(nullptr), currentPage(0), totalPages(0), account(std::move(account)), password(std::move(password))
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui.stackedWidget->setCurrentIndex(0);
    InitBackground();
    InitConnect();
    InitProductWidget();
    InitOrderTableView();
}

CashierWindowClass::~CashierWindowClass()
{
    vec_current_produc_widget.clear();
}

void CashierWindowClass::InitBackground()
{
    // 设置窗口背景图片
    QPixmap background(":/res/supermarket.jpg");  // 从资源文件中加载图片
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 缩放图片以适应窗口
    QPalette palette;
    palette.setBrush(QPalette::Window, background);  // 设置背景图片
    this->setPalette(palette);
}

void CashierWindowClass::InitConnect()
{
    // 商品分页按钮
    connect(ui.nextProductWidgetPageBtn, &QPushButton::clicked, this, &CashierWindowClass::onNextPageClicked);
    connect(ui.preProductWidgetPageBtn, &QPushButton::clicked, this, &CashierWindowClass::onPrevPageClicked);

    // 页面跳转按钮
    connect(ui.checkGoodsBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(0);
        });
    connect(ui.processOrderBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(1);
        });
    connect(ui.checkSalesInformationBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(2);
        });

    // 登出按钮
    connect(ui.logOutBtn, &QPushButton::clicked, this, &CashierWindowClass::onLogoutClicked);

    // 商品名称搜索
    connect(ui.produceNameLine->findChild<QAction*>(), &QAction::triggered, [=]() {
        onProductNameLineSearchClicked(ui.produceNameLine->text());
        });

    // 商品类别筛选
    connect(ui.produceComboBoxCategory, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CashierWindowClass::onProcuctCategoryComboxChanged);

    // 商品价格筛选
    connect(ui.produceComboBoxPrice, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CashierWindowClass::onProcuctPriceComboxChanged);

    // 销售名称搜索
    connect(ui.salesNameLine->findChild<QAction*>(), &QAction::triggered, [=]() {
        onSalesNameLineSearchClicked(ui.salesNameLine->text());
        });

    // 销售类别筛选
    connect(ui.salesComboBoxCategory, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CashierWindowClass::onSalesCategoryComboxChanged);

    // 销售价格筛选
    connect(ui.salesComboBoxPrice, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CashierWindowClass::onSalesPriceComboxChanged);
}

void CashierWindowClass::InitProductWidget()
{
    auto [records, kinds_count] = SqlTools::SearchProductTable_Kind_Price_Name();

    for (const auto& record : records)
    {
        vec_current_produc_widget.emplace_back(
            std::make_unique<ProductWidget>(
                new ProductWidget
                { 
                    nullptr ,
                    QString::fromStdString(record.path),
                    QString::fromStdString(record.price),
                    QString::fromStdString(record.category),
                    QString::fromStdString(record.name),
                    QString::fromStdString(record.ID),
                    QString::fromStdString(record.count),
                    QString::fromStdString(record.description)
                }
            ));
    }

    //设置商品展示网格
    for (int i = 0; i < 19; i++)
    {
        ProductWidget* pro = new ProductWidget(nullptr, ":/res/default.jpg");
        pro->setStyleSheet("QWidget { background-color: rgba(255,255,255,0.8); }");
        vec_current_produc_widget.push_back(std::make_unique<ProductWidget>(pro));
        //点击唤出详细页面
        connect(pro, &ProductWidget::clicked, this, &CashierWindowClass::onProductClicked);

    }
    //设置滚动区域
    ui.scrollAreaWidgetContents_3->setMaximumSize(1600, 800);
    ui.scrollAreaWidgetContents_3->setMinimumSize(1600, 800);
    ui.gridLayout->setSpacing(5);
    ui.gridLayout->setContentsMargins(10, 10, 10, 10);
    ui.scrollArea->setWidgetResizable(true);
    ui.scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui.scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // 初始化商品分页管理器
    productWidgetPagination = new ProductWidgetPagination(kinds_count, 9);
    updateProductPage();
}

void CashierWindowClass::InitOrderTableView()
{
    // 设置订单展示和处理表格
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({ "订单ID", "顾客ID", "订单时间", "总金额", "订单状态", "操作" });

    ui.tableView->setModel(model);
    ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableView->verticalHeader()->setVisible(false); // 隐藏行号
    ui.tableView->horizontalHeader()->setStretchLastSection(true); // 最后一列拉伸
    ui.tableView->setStyleSheet("QTableView { border: 1px solid #ddd; }");

    // 启用表头排序功能
    ui.tableView->setSortingEnabled(true);

    // 连接表头点击信号到槽函数
    connect(ui.tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &CashierWindowClass::onOrderHeaderClicked);

    // 初始化订单分页管理器
    orderPagination = new TableViewPagination(30); // 每页显示 30 条记录

    // 更新数据
    updateOrderPage();
}



void CashierWindowClass::onNextPageClicked()
{
    productWidgetPagination->setPage(productWidgetPagination->getCurrentPage() + 1);
    updateProductPage();
}

void CashierWindowClass::onPrevPageClicked()
{
    productWidgetPagination->setPage(productWidgetPagination->getCurrentPage() - 1);
    updateProductPage();
}


void CashierWindowClass::onProductClicked()
{
    ProductWidget* productWidget = qobject_cast<ProductWidget*>(sender());
    if (productWidget) {
		productWidget->showProductDetailDialog();
    }
}

void CashierWindowClass::updateOrderPage()
{
    // 获取当前页的偏移量和返回数量
    int offset = orderPagination->getCurrentPage() * orderPagination->getPageSize();
    int limit = orderPagination->getPageSize();

    // 调用 SqlTools 获取当前页的订单数据
    auto [records, totalCount] = SqlTools::SearchOrderTable_State_Price_ID(
        ui.salesComboBoxCategory->currentText().toStdString(),
        ui.salesComboBoxPrice->currentText().toStdString(),
        ui.salesNameLine->text().toStdString(),
        limit,
        offset,
        orderReverseFlags[0], // reverse_order_ID
        orderReverseFlags[1], // reverse_customer_ID
        orderReverseFlags[2], // reverse_date
        orderReverseFlags[3], // reverse_price
        orderReverseFlags[4]  // reverse_state
    );

    // 清空表格模型
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.tableView->model());
    model->removeRows(0, model->rowCount());

    // 填充表格数据
    for (const auto& record : records)
    {
        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(QString::fromStdString(record.order_ID));
        rowItems << new QStandardItem(QString::fromStdString(record.customer_ID));
        rowItems << new QStandardItem(QString::fromStdString(record.date));
        rowItems << new QStandardItem(QString::fromStdString(record.price));
        rowItems << new QStandardItem(QString::fromStdString(record.state));

        // 添加操作按钮
        QPushButton* btn = new QPushButton("同意", this);
        connect(btn, &QPushButton::clicked, [=]() {
            // 创建修改后的订单数据
            OrderDetail modifiedOrder = record;
            modifiedOrder.state = "已完成";

            // 调用 SqlTools 提交修改
            bool success = SqlTools::Change_OrderTable_State({ modifiedOrder },account);

            if (success) {
                // 提交成功，更新按钮状态和表格
                btn->setText("操作完成");
                btn->setEnabled(false);
                model->setData(model->index(model->rowCount() - 1, 4), "已完成");
            }
            else {
                // 提交失败，显示错误提示
                QMessageBox::warning(this, "失败", "订单状态更新失败，请重试！");
            }
            });

        // 将按钮放入表格
        QWidget* widget = new QWidget;
        QHBoxLayout* layout = new QHBoxLayout(widget);
        layout->addWidget(btn);
        layout->setAlignment(Qt::AlignCenter);
        layout->setContentsMargins(0, 0, 0, 0);
        widget->setLayout(layout);

        model->appendRow(rowItems);
        ui.tableView->setIndexWidget(model->index(model->rowCount() - 1, 5), widget);
    }

    // 更新分页标签和按钮状态
    ui.orderPageLabel->setText(QString("第 %1/%2 页")
        .arg(orderPagination->getCurrentPage() + 1)
        .arg(orderPagination->pageCount(totalCount)));
    ui.preOrderPageBtn->setEnabled(orderPagination->getCurrentPage() > 0);
    ui.nextOrderPageBtn->setEnabled(orderPagination->getCurrentPage() < orderPagination->pageCount(totalCount) - 1);
}

void CashierWindowClass::onOrderHeaderClicked(int column)
{
    // 重置其他列的排序状态
    for (int i = 0; i < 5; ++i) {
        if (i != column) {
            orderReverseFlags[i] = false;
        }
    }

    // 切换当前列的排序状态
    orderReverseFlags[column] = !orderReverseFlags[column];
    currentOrderSortColumn = column;

    // 更新订单表格数据
    updateOrderPage();
}



void CashierWindowClass::loadPage(int page)
{
}

void CashierWindowClass::onLogoutClicked() {
    UnifiedLoginManager::instance().localLogout(QString::fromStdString(account));
    emit logoutRequested();
    this->close();
}

void CashierWindowClass::updateProductPage()
{
    int offset = productWidgetPagination->getCurrentPage() * productWidgetPagination->getPageSize();
    int returnCount = productWidgetPagination->getPageSize();

    // 调用 SqlTools 获取当前页的数据
    auto [records, kinds_count] = SqlTools::SearchProductTable_Kind_Price_Name(
        ui.produceComboBoxCategory->currentText().toStdString(),
        ui.produceComboBoxPrice->currentText().toStdString(),
        ui.produceNameLine->text().toStdString(),
        returnCount,
        offset
    );

    // 清空当前商品数据
    vec_current_produc_widget.clear();

    // 创建新的商品小部件
    for (const auto& record : records) {
        vec_current_produc_widget.emplace_back(std::make_unique<ProductWidget>(
            new ProductWidget(
                nullptr,
                QString::fromStdString(record.path),
                QString::fromStdString(record.ID),
                QString::fromStdString(record.name),
                QString::fromStdString(record.price),
                QString::fromStdString(record.category),
                QString::fromStdString(record.count),
                QString::fromStdString(record.description)
            )
        ));
    }

    // 提供一个回调函数，获取当前页的商品数据
    auto fetchPageData = [this](int start, int count) -> std::vector<ProductWidget*> {
        std::vector<ProductWidget*> widgets;
        for (int i = start; i < start + count && i < vec_current_produc_widget.size(); ++i) {
            widgets.push_back(vec_current_produc_widget[i].get());
        }
        return widgets;
        };

    // 调用 applyToGridLayout，传递回调函数
    productWidgetPagination->applyToGridLayout(ui.gridLayout, fetchPageData);

    // 更新分页标签和按钮状态
    ui.productWidgetPageLabel->setText(QString("第 %1/%2 页")
        .arg(productWidgetPagination->getCurrentPage() + 1)
        .arg(productWidgetPagination->pageCount()));
    ui.preProductWidgetPageBtn->setEnabled(productWidgetPagination->getCurrentPage() > 0);
    ui.nextProductWidgetPageBtn->setEnabled(productWidgetPagination->getCurrentPage() < productWidgetPagination->pageCount() - 1);
}

void CashierWindowClass::updateSalesInformationPage()
{
    // 调用 SqlTools 获取销售信息
    SalesDetail salesDetail = SqlTools::Search_Sale_Information(account, password);

    // 更新销售信息到 UI
    ui.salePricelabel->setText(QString("您今日的销售金额是: %1").arg(QString::fromStdString(salesDetail.max_sales_amount)));
    ui.saleCountlabel->setText(QString("您今日的销售数量是: %1").arg(QString::fromStdString(salesDetail.max_sales_count)));
    ui.saleProductDescriptionLabel->setText(QString("商品描述: %1").arg(QString::fromStdString(salesDetail.product_detail.description)));

    // 更新商品详细信息
    const ProductDetail& product = salesDetail.product_detail;
    ui.productID2->setText(QString::fromStdString(product.ID));
    ui.productName2->setText(QString::fromStdString(product.name));
    ui.productPrice2->setText(QString::fromStdString(product.price));
    ui.productCategory2->setText(QString::fromStdString(product.category));
    ui.productCount2->setText(QString::fromStdString(product.count));
    ui.productTextEdit->setText(QString::fromStdString(product.description));

    // 更新商品图片
    QPixmap productImage(QString::fromStdString(product.path));
    if (!productImage.isNull()) 
    {
        ui.productPicture->setPixmap(productImage);
    }
    else 
    {
        ui.productPicture->setPixmap(QPixmap(":/res/default.jpg")); // 默认图片
    }
}

void CashierWindowClass::onProductNameLineSearchClicked(const QString& text)
{
    QString selectedCategory = ui.produceComboBoxCategory->currentText();
    QString selectedPrice = ui.produceComboBoxPrice->currentText();
    int returnCount = productWidgetPagination->getPageSize();

    // 调用 SqlTools 获取筛选后的商品总数
    auto [records, kinds_count] = SqlTools::SearchProductTable_Kind_Price_Name(
        selectedCategory.toStdString(),
        selectedPrice.toStdString(),
        text.toStdString(),
        returnCount,
        0 // 从偏移量 0 开始
    );

    // 更新商品分页器
    productWidgetPagination->updatePagination(kinds_count, returnCount);

    // 加载第一页的数据
    updateProductPage();
}

void CashierWindowClass::onProcuctCategoryComboxChanged(int index)
{
    QString selectedCategory = ui.produceComboBoxCategory->itemText(index);
    QString selectedPrice = ui.produceComboBoxPrice->currentText();
    QString produceName = ui.produceNameLine->text();
    int returnCount = productWidgetPagination->getPageSize();

    // 调用 SqlTools 获取筛选后的商品总数
    auto [records, kinds_count] = SqlTools::SearchProductTable_Kind_Price_Name(
        selectedCategory.toStdString(),
        selectedPrice.toStdString(),
        produceName.toStdString(),
        returnCount,
        0 // 从偏移量 0 开始
    );

    // 更新商品分页器
    productWidgetPagination->updatePagination(kinds_count, returnCount);

    // 加载第一页的数据
    updateProductPage();
}

void CashierWindowClass::onProcuctPriceComboxChanged(int index)
{
    QString selectedCategory = ui.produceComboBoxCategory->currentText();
    QString selectedPrice = ui.produceComboBoxPrice->itemText(index);
    QString produceName = ui.produceNameLine->text();
    int returnCount = productWidgetPagination->getPageSize();

    // 调用 SqlTools 获取筛选后的商品总数
    auto [records, kinds_count] = SqlTools::SearchProductTable_Kind_Price_Name(
        selectedCategory.toStdString(),
        selectedPrice.toStdString(),
        produceName.toStdString(),
        returnCount,
        0 // 从偏移量 0 开始
    );

    // 更新商品分页器
    productWidgetPagination->updatePagination(kinds_count, returnCount);

    // 加载第一页的数据
    updateProductPage();
}

void CashierWindowClass::onSalesNameLineSearchClicked(const QString& text)
{
    // 重置到第一页
    orderPagination->setPage(0, 0);
    updateOrderPage();
}


void CashierWindowClass::onSalesCategoryComboxChanged(int index)
{
    QString selectedCategory = ui.salesComboBoxCategory->itemText(index);
    // 重置到第一页
    orderPagination->setPage(0, 0);
    updateOrderPage();
}

void CashierWindowClass::onSalesPriceComboxChanged(int index)
{
    QString selectedPrice = ui.salesComboBoxPrice->itemText(index);
    // 重置到第一页
    orderPagination->setPage(0, 0);
    updateOrderPage();
}
