#include "ManagerWindowClass.h"
#include <qstandarditemmodel.h>
#include <qdatetime.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include "CustomerDetailDialog.h"
#include "FinanceChart.h"
#include <QRandomGenerator>
#include "OrderDetailDialog.h"
#include "StockDetailDialog.h"
#include "StaffDetailDialog.h"
#include <QStringConverter>

ManagerWindowClass::ManagerWindowClass(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui.stackedWidget->setCurrentIndex(0);

    convert.emplace("全部种类", "all");
    convert.emplace("生鲜", "fresh produce");
    convert.emplace("蔬菜", "vegetables");
    convert.emplace("水果", "fruit");
    convert.emplace("粮食", "grain");
    convert.emplace("饮品", "drink");
    convert.emplace("烟", "smoke");
    convert.emplace("零食", "snacks");
    convert.emplace("日用百货", "daily necessities");
    convert.emplace("五金零件", "hardware parts");
    convert.emplace("母婴用品", "baby and maternal products");

    convert.emplace("全部价格", " ");
    convert.emplace("<10元", "less10");
    convert.emplace("10元~50元 ", "10to50");
    convert.emplace("50元~100元", "50to100");
    convert.emplace(">100", "than100");



    InitBackground();
	InitConnect();
    InitManagerGoodsPage();
    InitProcessSalesPage();
    InitProcessStockInfoPage();
    InitManagerStaffPage();
    InitManagerCustomPage();
    InitFinancialManagerPage();

}

ManagerWindowClass::~ManagerWindowClass()
{
    vec_current_goods_widget.clear();
}

void ManagerWindowClass::InitBackground()
{
    // 设置窗口背景图片
    QPixmap background(":/res/manager.png");  // 从资源文件中加载图片
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 缩放图片以适应窗口
    QPalette palette;
    palette.setBrush(QPalette::Window, background);  // 设置背景图片
    this->setPalette(palette);
}

void ManagerWindowClass::InitConnect()
{
    // 页面切换按钮
    connect(ui.manageGoodsBtn, &QPushButton::clicked, [=]() { ui.stackedWidget->setCurrentIndex(0); });
    connect(ui.processSalesInfoBtn, &QPushButton::clicked, [=]() { ui.stackedWidget->setCurrentIndex(1); });
    connect(ui.processreStockInfoBtn, &QPushButton::clicked, [=]() { ui.stackedWidget->setCurrentIndex(2); });
    connect(ui.manageStaffBtn, &QPushButton::clicked, [=]() { ui.stackedWidget->setCurrentIndex(3); });
    connect(ui.managerCustomBtn, &QPushButton::clicked, [=]() { ui.stackedWidget->setCurrentIndex(4); });
    connect(ui.financialManagerBtn, &QPushButton::clicked, [=]() { ui.stackedWidget->setCurrentIndex(5); });

    // 登出按钮
    connect(ui.logOutBtn, &QPushButton::clicked, this, &ManagerWindowClass::onLogoutClicked);

    // 商品管理
    connect(ui.goodsComboBoxCategory, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ManagerWindowClass::onGoodsComboxCategoryClicked);
    connect(ui.goodsComboBoxPrice, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ManagerWindowClass::onGoodsComboxPriceClicked);
    connect(ui.goodsNameLine, &SearchEdit::textChanged, this, &ManagerWindowClass::onGoodstNameLineSearchClicked);
    connect(ui.goodsStockBtn, &QPushButton::clicked, [=]() {
        QString id = ui.goodsStockEdit->text();
        int count = ui.goodsSpinBox->value();
        onAddGoodsClicked(id, count);
        });
    connect(ui.goodsDeleteBtn, &QPushButton::clicked, [=]() {
        QString id = ui.goodsDeleteEdit->text();
        onDeleteGoodsClicked(id);
        });
    connect(ui.preGoodsWidgetPageBtn, &QPushButton::clicked, this, &ManagerWindowClass::onPrevPageClicked);
    connect(ui.nextGoodsWidgetPageBtn, &QPushButton::clicked, this, &ManagerWindowClass::onNextPageClicked);

    // 销售管理
    connect(ui.saleComboBoxCategory, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ManagerWindowClass::onSaleComboxCategoryClicked);
    connect(ui.saleComboBoxPrice, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ManagerWindowClass::onSaleComboxPriceClicked);
    connect(ui.saleNameLine, &SearchEdit::textChanged, this, &ManagerWindowClass::onSaleNameLineSearchClicked);
    connect(ui.saleChangedAcceptBtn, &QPushButton::clicked, this, &ManagerWindowClass::onSaleChangedAcceptClicked);
    connect(ui.saleChangedCancelBtn, &QPushButton::clicked, this, &ManagerWindowClass::onSaleChangedCancelClicked);
    connect(ui.addSaleBtn, &QPushButton::clicked, this, &ManagerWindowClass::onAddSaleClicked);
    connect(ui.saleDeleteBtn, &QPushButton::clicked, [=]() {
        QString id = ui.saleDeleteEdit->text();
        onDeleteSaleClicked(id, 0);
        });
    connect(ui.preSalePageBtn, &QPushButton::clicked, [=]() {
        salePagination->setPage(salePagination->getCurrentPage() - 1, salePageCount);
        updateSalePage();
        });
    connect(ui.nextSalePageBtn, &QPushButton::clicked, [=]() {
        salePagination->setPage(salePagination->getCurrentPage() + 1, salePageCount);
        updateSalePage();
        });

    // 进货管理
    connect(ui.stockComboBoxCategory, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ManagerWindowClass::onStockComboxCategoryClicked);
    connect(ui.stockComboBoxPrice, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ManagerWindowClass::onStockComboxPriceClicked);
    connect(ui.stockNameLine, &SearchEdit::textChanged, this, &ManagerWindowClass::onStockNameLineSearchClicked);
    connect(ui.stockChangedAcceptBtn, &QPushButton::clicked, this, &ManagerWindowClass::onStockChangedAcceptClicked);
    connect(ui.stockChangedCancelBtn, &QPushButton::clicked, this, &ManagerWindowClass::onStockChangedCancelClicked);
    connect(ui.addStockBtn, &QPushButton::clicked, this, &ManagerWindowClass::onAddStockClicked);
    connect(ui.stockDeleteBtn, &QPushButton::clicked, [=]() {
        QString id = ui.stockDeleteEdit->text();
        onDeleteStockClicked(id, 0);
        });
    connect(ui.preStockPageBtn, &QPushButton::clicked, [=]() {
        stockPagination->setPage(stockPagination->getCurrentPage() - 1, stockPageCount);
        updateStockPage();
        });
    connect(ui.nextStockPageBtn, &QPushButton::clicked, [=]() {
        stockPagination->setPage(stockPagination->getCurrentPage() + 1, stockPageCount);
        updateStockPage();
        });

    // 员工管理
    connect(ui.staffComboBoxRole, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ManagerWindowClass::onStaffComboxCategoryClicked);
    connect(ui.staffNameLine, &SearchEdit::textChanged, this, &ManagerWindowClass::onstaffNameLineSearchClicked);
    connect(ui.staffChangedAcceptBtn, &QPushButton::clicked, this, &ManagerWindowClass::onStaffChangedAcceptClicked);
    connect(ui.staffChangedCancelBtn, &QPushButton::clicked, this, &ManagerWindowClass::onStaffChangedCancelClicked);
    connect(ui.addStaffBtn, &QPushButton::clicked, this, &ManagerWindowClass::onAddStaffClicked);
    connect(ui.staffDeleteBtn, &QPushButton::clicked, this, &ManagerWindowClass::onDeleteStaffClicked);
    connect(ui.preStaffPageBtn, &QPushButton::clicked, [=]() {
        staffPagination->setPage(staffPagination->getCurrentPage() - 1, staffPageCount);
        updateStaffPage();
        });
    connect(ui.nextStaffPageBtn, &QPushButton::clicked, [=]() {
        staffPagination->setPage(staffPagination->getCurrentPage() + 1, staffPageCount);
        updateStaffPage();
        });

    // 顾客管理
    connect(ui.customerNameLine, &SearchEdit::textChanged, this, &ManagerWindowClass::onCustomerNameLineSearchClicked);
    connect(ui.customerChangedAcceptBtn, &QPushButton::clicked, this, &ManagerWindowClass::onCustomerChangedAcceptClicked);
    connect(ui.customerChangedCancelBtn, &QPushButton::clicked, this, &ManagerWindowClass::onCustomerChangedCancelClicked);
    connect(ui.addCustomerBtn, &QPushButton::clicked, this, &ManagerWindowClass::onAddCustomerClicked);
    connect(ui.customerDeleteBtn, &QPushButton::clicked, this, &ManagerWindowClass::onDeleteCustomerClicked);
    connect(ui.preCustomerPageBtn, &QPushButton::clicked, [=]() {
        customerPagination->setPage(customerPagination->getCurrentPage() - 1, customerPageCount);
        updateCustomerPage();
        });
    connect(ui.nextCustomerPageBtn, &QPushButton::clicked, [=]() {
        customerPagination->setPage(customerPagination->getCurrentPage() + 1, customerPageCount);
        updateCustomerPage();
        });
    connect(ui.customerTableView, &QTableView::doubleClicked, this, &ManagerWindowClass::onCustomerAvatarDoubleClicked);

    // 连接日期范围变化的信号到槽函数
    connect(ui.dateEditStart, &QDateEdit::dateChanged, this, &ManagerWindowClass::onDateRangeChanged);
    connect(ui.dateEditEnd, &QDateEdit::dateChanged, this, &ManagerWindowClass::onDateRangeChanged);



   
}

void ManagerWindowClass::InitManagerGoodsPage()
{
    /*qDebug() << ui.goodsComboBoxCategory->currentText().toStdU32String() << ui.goodsComboBoxPrice->currentText().toStdString() << ui.goodsComboBoxPrice->currentText().toUtf8().constData();
    std::string e = ui.goodsComboBoxPrice->currentText().toUtf8().data();
    qDebug() << e;
    auto fromUtf16 = QStringEncoder(QStringEncoder::Utf8);
    QString category = ui.goodsComboBoxCategory->currentText();
    std::string categoryStr = category.toUtf8().constData();
    qDebug() << categoryStr << ui.goodsComboBoxPrice->currentText().toStdString();

    库存管理页面*/
    auto [records, kinds_count] = SqlTools::SearchProductTable_Kind_Price_Name
    (
        convert[ui.goodsComboBoxCategory->currentText()],
        convert[ui.goodsComboBoxPrice->currentText()],
        ui.goodsNameLine->text().toStdString()

    );

    for (const auto& record : records)
    {
        auto widget = std::make_unique<GoodsWidget>(
            new GoodsWidget{
            nullptr,
            QString::fromStdString(record.path),
            QString::fromStdString(record.ID),
            QString::fromStdString(record.name),
            QString::fromStdString(record.price),
            QString::fromStdString(record.category),
            QString::fromStdString(record.count),
            QString::fromStdString(record.description)
            }
        );

        connect(widget.get(), &GoodsWidget::clicked, this, &ManagerWindowClass::onGoodsClicked);

        vec_current_goods_widget.emplace_back(std::move(widget));
    }

    if ( vec_current_goods_widget.size()<=9)
    {
        auto addGoo = std::make_unique<GoodsWidget>(nullptr, QString(":/res/Add_Icon.svg"));
        addGoo->setStyleSheet("QWidget { background-color: rgba(255,255,255,0.8); }");

        connect(addGoo.get(), &GoodsWidget::clicked, this, &ManagerWindowClass::onGoodsClicked);

        vec_current_goods_widget.push_back(std::move(addGoo));
        kinds_count++;
    }
   
    //设置滚动区域
    ui.scrollAreaWidgetContents_3->setMaximumSize(1600, 800);
    ui.scrollAreaWidgetContents_3->setMinimumSize(1600, 800);
    ui.gridLayout->setSpacing(5);
    ui.gridLayout->setContentsMargins(10, 10, 10, 10);
    ui.scrollArea->setWidgetResizable(true);
    ui.scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui.scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    goodsWidgetPagination = new GoodsWidgetPagination(kinds_count, 9);
    updateGoodsPage();
}

void ManagerWindowClass::InitProcessSalesPage()
{
    // 设置销售记录展示和处理表格
    QStandardItemModel* salemodel = new QStandardItemModel(this);
    salemodel->setHorizontalHeaderLabels({ "销售记录ID", "订单ID", "商品ID", "销售数量", "销售时间"});

    ui.saleTableView->setModel(salemodel);
    ui.saleTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.saleTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.saleTableView->verticalHeader()->setVisible(false); // 隐藏行号
    ui.saleTableView->horizontalHeader()->setStretchLastSection(true); // 最后一列拉伸
    ui.saleTableView->setStyleSheet("QTableView { border: 1px solid #ddd; }");

    // 启用表头排序功能
    ui.saleTableView->setSortingEnabled(true);

    // 连接表头点击信号到槽函数
    connect(ui.saleTableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &ManagerWindowClass::onSaleHeaderClicked);

    // 初始化订单分页管理器
    salePagination = new TableViewPagination(30); // 每页显示 30 条记录

    // 更新数据
    updateSalePage();
}


void ManagerWindowClass::InitProcessStockInfoPage()
{
    // 设置进货记录展示和处理表格
    QStandardItemModel* stockModel = new QStandardItemModel(this);
    stockModel->setHorizontalHeaderLabels({ "商品ID", "进货记录ID", "进货数量", "进货时间" });

    ui.stockTableView->setModel(stockModel);
    ui.stockTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.stockTableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui.stockTableView->verticalHeader()->setVisible(false); // 隐藏行号
    ui.stockTableView->horizontalHeader()->setStretchLastSection(true); // 最后一列拉伸
    ui.stockTableView->setStyleSheet("QTableView { border: 1px solid #ddd; }");

    // 启用表头排序功能
    ui.stockTableView->setSortingEnabled(true);

    // 连接表头点击信号到槽函数
    connect(ui.stockTableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &ManagerWindowClass::onStockHeaderClicked);

    // 初始化进货记录分页管理器
    stockPagination = new TableViewPagination(30); // 每页显示 30 条记录

    // 更新数据
    updateStockPage();
}


void ManagerWindowClass::InitManagerStaffPage()
{
    // 设置员工展示和处理表格
    QStandardItemModel* staffModel = new QStandardItemModel(this);
    staffModel->setHorizontalHeaderLabels({ "员工ID", "名字", "邮箱", "密码", "加入时间", "职责" });

    ui.staffTableView->setModel(staffModel);
    ui.staffTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.staffTableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui.staffTableView->verticalHeader()->setVisible(false); // 隐藏行号
    ui.staffTableView->horizontalHeader()->setStretchLastSection(true); // 最后一列拉伸
    ui.staffTableView->setStyleSheet("QTableView { border: 1px solid #ddd; }");

    // 启用表头排序功能
    ui.staffTableView->setSortingEnabled(true);

    // 连接表头点击信号到槽函数
    connect(ui.staffTableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &ManagerWindowClass::onStaffHeaderClicked);


    // 初始化员工分页管理器
    staffPagination = new TableViewPagination(staffPageSize);

    // 更新数据
    updateStaffPage();


}


void ManagerWindowClass::InitManagerCustomPage()
{
    // 设置顾客展示和处理表格
    customerModel = new QStandardItemModel(this);
    customerModel->setHorizontalHeaderLabels({ "头像", "顾客ID", "出生年月", "个人备注", "注册时间", "邮箱", "密码" });

    ui.customerTableView->setModel(customerModel);
    ui.customerTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.customerTableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui.customerTableView->verticalHeader()->setVisible(false); // 隐藏行号
    ui.customerTableView->horizontalHeader()->setStretchLastSection(true); // 最后一列拉伸
    ui.customerTableView->setStyleSheet("QTableView { border: 1px solid #ddd; }");

    // 启用表头排序功能
    ui.customerTableView->setSortingEnabled(true);

    // 连接表头点击信号到槽函数
    connect(ui.customerTableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &ManagerWindowClass::onCustomerHeaderClicked);


    // 初始化顾客分页管理器
    customerPagination = new TableViewPagination(customerPageSize);

    // 更新数据
    updateCustomerPage();

   
}



void ManagerWindowClass::InitFinancialManagerPage()
{
    // 初始化 FinanceChart
    FinanceChart* financeChart = new FinanceChart(ui.page_5);
    ui.chartLay->layout()->addWidget(financeChart);

    // 设置默认日期范围
    QDateTime currentDate = QDateTime::currentDateTime();
    ui.dateEditStart->setDate(currentDate.addDays(-7).date());
    ui.dateEditEnd->setDate(currentDate.date());

    // 初始化折线图数据
    onDateRangeChanged();
}


void ManagerWindowClass::onNextPageClicked()
{
    goodsWidgetPagination->setPage(goodsWidgetPagination->getCurrentPage() + 1);
    updateGoodsPage();
}

void ManagerWindowClass::onPrevPageClicked()
{
    goodsWidgetPagination->setPage(goodsWidgetPagination->getCurrentPage() - 1);
    updateGoodsPage();
}

void ManagerWindowClass::updateSalePage()
{
    // 获取当前页的偏移量和返回数量
    int offset = salePagination->getCurrentPage() * salePagination->getPageSize();
    int limit = salePagination->getPageSize();

    // 调用 SqlTools 获取当前页的销售数据
    auto [records, totalCount] = SqlTools::Search_SaleTable_State_Price_ID(
        convert[ui.saleComboBoxCategory->currentText()],
        convert[ui.saleComboBoxPrice->currentText()],
        ui.saleNameLine->text().toStdString(),
        limit,
        offset,
        saleReverseFlags[0], // reverse_order_ID
        saleReverseFlags[1], // reverse_customer_ID
        saleReverseFlags[2], // reverse_date
        saleReverseFlags[3], // reverse_price
        saleReverseFlags[4]  // reverse_state
    );

    // 清空表格模型
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.saleTableView->model());
    model->removeRows(0, model->rowCount());

    // 填充表格数据
    for (int row = 0; row < records.size(); ++row) {
        const auto& record = records[row];
        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(QString::fromStdString(record.sale_ID));
        rowItems << new QStandardItem(QString::fromStdString(record.order_ID));
        rowItems << new QStandardItem(QString::fromStdString(record.goods_ID));
        rowItems << new QStandardItem(QString::fromStdString(record.count));
        rowItems << new QStandardItem(QString::fromStdString(record.date));
        model->appendRow(rowItems);
    }

    // 更新分页标签和按钮状态
    ui.salePageLabel->setText(QString("第 %1/%2 页")
        .arg(salePagination->getCurrentPage() + 1)
        .arg((totalCount + salePagination->getPageSize() - 1) / salePagination->getPageSize()));
    ui.preSalePageBtn->setEnabled(salePagination->getCurrentPage() > 0);
    ui.nextSalePageBtn->setEnabled(salePagination->getCurrentPage() < (totalCount - 1) / salePagination->getPageSize());
}

void ManagerWindowClass::onSaleChangedAcceptClicked()
{
    std::vector<SaleTableLine> changedData;

    // 遍历 vec_changed_sale_data，收集需要提交的数据
    for (const auto& sale : vec_changed_sale_data) {
        changedData.push_back(*sale);
    }

    // 调用 SqlTools 更新数据库
    if (!changedData.empty()) {
        bool success = SqlTools::Change_SaleTable(changedData);
        if (success) {
            QMessageBox::information(this, "成功", "销售数据已成功更新！");
            vec_changed_sale_data.clear(); // 清空修改记录
            updateSalePage(); // 刷新页面
        }
        else {
            QMessageBox::warning(this, "失败", "销售数据更新失败！");
        }
    }
}

void ManagerWindowClass::onSaleChangedCancelClicked()
{
    // 清空修改记录
    vec_changed_sale_data.clear();

    // 重新加载当前页数据
    updateSalePage();

    QMessageBox::information(this, "提示", "所有修改已取消！");
}

void ManagerWindowClass::onSaleDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    QAbstractItemModel* model = ui.saleTableView->model();
    for (int row = topLeft.row(); row <= bottomRight.row(); ++row)
    {
        SaleTableLine* sale = new SaleTableLine();
        sale->sale_ID = model->data(model->index(row, 0)).toString().toStdString();
        sale->order_ID = model->data(model->index(row, 1)).toString().toStdString();
        sale->goods_ID = model->data(model->index(row, 2)).toString().toStdString();
        sale->count = model->data(model->index(row, 3)).toString().toStdString();
        sale->date = model->data(model->index(row, 4)).toString().toStdString();

        // 将修改记录存储到 vec_changed_sale_data
        vec_changed_sale_data.push_back(std::unique_ptr<SaleTableLine>(sale));

        // 将修改后的数据字体变为红色
        model->setData(model->index(row, 0), QBrush(Qt::red), Qt::ForegroundRole);
    }
}

void ManagerWindowClass::onSaleComboxCategoryClicked(int index)
{
    QString selectedCategory = ui.saleComboBoxCategory->itemText(index);
    salePagination->setPage(0, 0); // 重置到第一页
    updateSalePage();
}

void ManagerWindowClass::onSaleComboxPriceClicked(int index)
{
    QString selectedPrice = ui.saleComboBoxPrice->itemText(index);
    salePagination->setPage(0, 0); // 重置到第一页
    updateSalePage();
}

void ManagerWindowClass::onAddSaleClicked()
{
    // 创建并显示 OrderDetailDialog
    OrderDetailDialog dialog(this);

    // 连接确定按钮的信号槽
    connect(dialog.getUi()->confirmButton, &QPushButton::clicked, [&]() {
        QString orderID = dialog.getUi()->saleIDEdit->text().trimmed();
        QString customerID = dialog.getUi()->oderIDEdit->text().trimmed();
        QString date = dialog.getUi()->goodsIDEdit->text().trimmed();
        QString price = dialog.getUi()->countEdit->text().trimmed();
        QString state = dialog.getUi()->dateEdit->text().trimmed();

        // 检查输入是否有效
        if (orderID.isEmpty() || customerID.isEmpty() || date.isEmpty() || price.isEmpty() || state.isEmpty()) {
            QMessageBox::warning(&dialog, "警告", "所有字段均为必填项！");
            return;
        }

        // 调用 SqlTools 添加订单
        SaleTableLine newSale(orderID.toStdString(), customerID.toStdString(), date.toStdString(), price.toStdString(), state.toStdString());
        bool success = SqlTools::Add_Sale(newSale);

        if (success) {
            QMessageBox::information(&dialog, "成功", "订单已成功添加！");
            dialog.accept(); // 关闭对话框
            updateSalePage(); // 刷新销售页面
        }
        else {
            QMessageBox::warning(&dialog, "失败", "订单添加失败，请检查输入！");
        }
        });

    // 连接取消按钮的信号槽
    connect(dialog.getUi()->cancelButton, &QPushButton::clicked, [&]() {
        dialog.reject(); // 关闭对话框
        });

    dialog.exec(); // 显示对话框
}

void ManagerWindowClass::onDeleteSaleClicked(const QString& id, int count)
{
    if (id.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入订单ID！");
        return;
    }

    // 调用 SqlTools 删除数据库中的记录
    bool success = SqlTools::Delete_Sale(id.toStdString());
    if (success) {
        QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.saleTableView->model());
        if (!model) return;

        // 遍历表格，找到对应的订单ID
        for (int row = 0; row < model->rowCount(); ++row) {
            QModelIndex index = model->index(row, 0); // 假设订单ID在第0列
            if (model->data(index).toString() == id) {
                model->removeRow(row); // 删除该行
                break;
            }
        }

        QMessageBox::information(this, "成功", "销售记录已成功删除！");
        updateSalePage(); // 刷新页面
    }
    else {
        QMessageBox::warning(this, "失败", "删除销售记录失败，请检查订单ID！");
    }
}

void ManagerWindowClass::onSaleNameLineSearchClicked(const QString& text)
{
    salePagination->setPage(0, 0); // 重置到第一页
    updateSalePage();
}

void ManagerWindowClass::updateStockPage()
{
    // 获取当前页的偏移量和返回数量
    int offset = stockPagination->getCurrentPage() * stockPagination->getPageSize();
    int limit = stockPagination->getPageSize();
    qDebug() << ui.stockComboBoxCategory->currentText().toStdU32String() << ui.stockComboBoxPrice->currentText() << ui.stockNameLine->text();

    // 调用 SqlTools 获取当前页的进货记录数据
    auto [records, totalCount] = SqlTools::Search_StockTable_State_Price_ID(
        convert[ui.stockComboBoxCategory->currentText()],
        convert[ui.stockComboBoxPrice->currentText()],
        ui.stockNameLine->text().toStdString(),
        limit,
        offset,
        stockReverseFlags[0], // reverse_goods_ID
        stockReverseFlags[1], // reverse_stock_ID
        stockReverseFlags[2], // reverse_count
        stockReverseFlags[3]  // reverse_date
    );

    // 清空表格模型
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.stockTableView->model());
    model->removeRows(0, model->rowCount());

    // 填充表格数据
    for (const auto& record : records)
    {
        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(QString::fromStdString(record.goods_ID));
        rowItems << new QStandardItem(QString::fromStdString(record.stock_ID));
        rowItems << new QStandardItem(QString::fromStdString(record.count));
        rowItems << new QStandardItem(QString::fromStdString(record.date));
        model->appendRow(rowItems);
    }

    // 更新分页标签和按钮状态
    ui.stockPageLabel->setText(QString("第 %1/%2 页")
        .arg(stockPagination->getCurrentPage() + 1)
        .arg(stockPagination->pageCount(totalCount)));
    ui.preStockPageBtn->setEnabled(stockPagination->getCurrentPage() > 0);
    ui.nextStockPageBtn->setEnabled(stockPagination->getCurrentPage() < stockPagination->pageCount(totalCount) - 1);
}

void ManagerWindowClass::onAddStockClicked()
{
    // 创建并显示 StockDetailDialog
    StockDetailDialog dialog(this);

    // 连接确定按钮的信号槽
    connect(dialog.getUi()->confirmButton, &QPushButton::clicked, [&]() {
        QString goodsID = dialog.getUi()->goodsIDEdit->text().trimmed();
        QString stockID = dialog.getUi()->stockIDEdit->text().trimmed();
        QString count = dialog.getUi()->countEdit->text().trimmed();
        QString date = dialog.getUi()->dateEdit->text().trimmed();

        // 检查输入是否有效
        if (goodsID.isEmpty() || stockID.isEmpty() || count.isEmpty() || date.isEmpty()) {
            QMessageBox::warning(&dialog, "警告", "所有字段均为必填项！");
            return;
        }

        // 调用 SqlTools 添加进货记录
        StockDetail newStock(goodsID.toStdString(), stockID.toStdString(), count.toStdString(), date.toStdString());
        bool success = SqlTools::Add_Stock(newStock);

        if (success) {
            QMessageBox::information(&dialog, "成功", "进货记录已成功添加！");
            dialog.accept(); // 关闭对话框
            updateStockPage(); // 刷新进货记录页面
        }
        else {
            QMessageBox::warning(&dialog, "失败", "进货记录添加失败，请检查输入！");
        }
        });

    // 连接取消按钮的信号槽
    connect(dialog.getUi()->cancelButton, &QPushButton::clicked, [&]() {
        dialog.reject(); // 关闭对话框
        });

    dialog.exec(); // 显示对话框
}

void ManagerWindowClass::onStockChangedAcceptClicked()
{
    std::vector<StockDetail> changedData;

    // 遍历 vec_changed_stock_data，收集需要提交的数据
    for (const auto& stock : vec_changed_stock_data) {
        changedData.push_back(*stock);
    }

    // 调用 SqlTools 更新数据库
    if (!changedData.empty()) {
        bool success = SqlTools::Change_StockTable_State(changedData);
        if (success) {
            QMessageBox::information(this, "成功", "进货记录已成功更新！");
            vec_changed_stock_data.clear(); // 清空修改记录
            updateStockPage(); // 刷新页面
        }
        else {
            QMessageBox::warning(this, "失败", "进货记录更新失败！");
        }
    }
}

void ManagerWindowClass::onStockChangedCancelClicked()
{
    // 清空修改记录
    vec_changed_stock_data.clear();

    // 重新加载当前页数据
    updateStockPage();

    QMessageBox::information(this, "提示", "所有修改已取消！");
}

void ManagerWindowClass::onStockDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    QAbstractItemModel* model = ui.stockTableView->model();
    for (int row = topLeft.row(); row <= bottomRight.row(); ++row)
    {
        StockDetail* stock = new StockDetail();
        stock->goods_ID = model->data(model->index(row, 0)).toString().toStdString();
        stock->stock_ID = model->data(model->index(row, 1)).toString().toStdString();
        stock->count = model->data(model->index(row, 2)).toString().toStdString();
        stock->date = model->data(model->index(row, 3)).toString().toStdString();

        // 将修改记录存储到 vec_changed_stock_data
        vec_changed_stock_data.push_back(std::unique_ptr<StockDetail>(stock));

        // 将修改后的数据字体变为红色
        model->setData(model->index(row, 0), QBrush(Qt::red), Qt::ForegroundRole);
    }
}

void ManagerWindowClass::onStockComboxCategoryClicked(int index)
{
    QString selectedCategory = ui.stockComboBoxCategory->itemText(index);
    stockPagination->setPage(0, 0); // 重置到第一页
    updateStaffPage();
}

void ManagerWindowClass::onStockComboxPriceClicked(int index)
{
    QString selectedPrice = ui.stockComboBoxPrice->itemText(index);
    stockPagination->setPage(0, 0); // 重置到第一页
    updateStockPage();
}

void ManagerWindowClass::onStockNameLineSearchClicked(const QString& text)
{
    stockPagination->setPage(0, 0); // 重置到第一页
    updateStaffPage();
}

void ManagerWindowClass::onDeleteStockClicked(const QString& id, int count)
{
    if (id.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入进货记录ID！");
        return;
    }

    // 调用 SqlTools 删除数据库中的记录
    bool success = SqlTools::Delete_Stock(id.toStdString());
    if (success) {
        QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.stockTableView->model());
        if (!model) return;

        // 遍历表格，找到对应的进货记录ID
        for (int row = 0; row < model->rowCount(); ++row) {
            QModelIndex index = model->index(row, 1); // 假设进货记录ID在第1列
            if (model->data(index).toString() == id) {
                model->removeRow(row); // 删除该行
                break;
            }
        }

        QMessageBox::information(this, "成功", "进货记录已成功删除！");
        updateStockPage(); // 刷新页面
    }
    else {
        QMessageBox::warning(this, "失败", "删除进货记录失败，请检查进货记录ID！");
    }
}

void ManagerWindowClass::updateStaffPage()
{
    // 获取当前页的偏移量和返回数量
    int offset = staffPagination->getCurrentPage() * staffPagination->getPageSize();
    int limit = staffPagination->getPageSize();

    // 调用 SqlTools 获取当前页的员工数据
    auto [records, totalCount] = SqlTools::Search_StaffTable_Role_Name(
        convert[ui.staffComboBoxRole->currentText()],
        ui.staffNameLine->text().toStdString(),
        limit,
        offset,
        staffReverseFlags[0], // reverse_staff_ID
        staffReverseFlags[1], // reverse_name
        staffReverseFlags[2], // reverse_email
        staffReverseFlags[3], // reverse_password
        staffReverseFlags[4], // reverse_join_date
        staffReverseFlags[5]  // reverse_role
    );

    // 清空表格模型
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.staffTableView->model());
    model->removeRows(0, model->rowCount());

    // 填充表格数据
    for (const auto& record : records)
    {
        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(QString::fromStdString(record.staff_ID));
        rowItems << new QStandardItem(QString::fromStdString(record.name));
        rowItems << new QStandardItem(QString::fromStdString(record.email));
        rowItems << new QStandardItem(QString::fromStdString(record.password));
        rowItems << new QStandardItem(QString::fromStdString(record.join_date));
        rowItems << new QStandardItem(QString::fromStdString(record.role));
        model->appendRow(rowItems);
    }

    // 更新分页标签和按钮状态
    ui.staffPageLabel->setText(QString("第 %1/%2 页")
        .arg(staffPagination->getCurrentPage() + 1)
        .arg(staffPagination->pageCount(totalCount)));
    ui.preStaffPageBtn->setEnabled(staffPagination->getCurrentPage() > 0);
    ui.nextStaffPageBtn->setEnabled(staffPagination->getCurrentPage() < staffPagination->pageCount(totalCount) - 1);
}

void ManagerWindowClass::onGoodsClicked()
{
    GoodsWidget* goodsWidget = qobject_cast<GoodsWidget*>(sender());
    if (!goodsWidget) return;

    // 判断是否是新增商品按钮
    if (goodsWidget->getID() == "NULL") 
    {
        GoodsDetailDialog dialog(this);
        connect(&dialog, &GoodsDetailDialog::accepted, [&]() {
            // 获取输入的商品信息
            QString id = dialog.getUi().goodsIDEdit->text().trimmed();
            QString name = dialog.getUi().goodsNameEdit->text().trimmed();
            QString price = dialog.getUi().goodsPriceEdit->text().trimmed();
            QString category = dialog.getUi().goodsCategoryEdit->text().trimmed();
            QString count = dialog.getUi().goodsCountEdit->text().trimmed();
            QString description = dialog.getUi().goodsTextEdit->toPlainText().trimmed();
            QString path = ":/res/default.jpg"; // 默认图片路径
            if (dialog.getUi().goodsPicture)
            {
                path = QString::fromStdString(dialog.GetPath());
            }

            // 调用 SqlTools::Add_Goods 写入数据库
            bool success = SqlTools::Add_Goods
            (
                path.toStdString(),
                id.toStdString(),
                name.toStdString(),
                price.toStdString(),
                convert[category],
                count.toStdString(),
                description.toStdString()
            );

            if (success) 
            {
                QMessageBox::information(this, "成功", "商品已成功添加！");
                updateGoodsPage(); // 刷新商品显示
            }
            else 
            {
                QMessageBox::warning(this, "失败", "商品添加失败，请检查输入！");
            }
            });

        dialog.exec(); // 显示对话框
    }
    else {
        goodsWidget->showGoodsDetailDialog(); // 显示商品详情
    }
}

void ManagerWindowClass::onGoodsComboxCategoryClicked(int index)
{
    QString selectedCategory = ui.goodsComboBoxCategory->itemText(index);
    QString selectedPrice = ui.goodsComboBoxPrice->currentText();
    QString goodsName = ui.goodsNameLine->text();

    // 调用 SqlTools 获取筛选后的商品总数
    auto [records, totalCount] = SqlTools::SearchProductTable_Kind_Price_Name(
        convert[selectedCategory],
        convert[selectedPrice],
        goodsName.toStdString(),
        goodsWidgetPagination->getPageSize(),
        0 // 从偏移量 0 开始
    );

    // 更新商品分页器
    goodsWidgetPagination->updatePagination(totalCount, goodsWidgetPagination->getPageSize());

    // 加载第一页的数据
    updateGoodsPage();
}

void ManagerWindowClass::onGoodsComboxPriceClicked(int index)
{
    QString selectedCategory = ui.goodsComboBoxCategory->currentText();
    QString selectedPrice = ui.goodsComboBoxPrice->itemText(index);
    QString goodsName = ui.goodsNameLine->text();

    // 调用 SqlTools 获取筛选后的商品总数
    auto [records, totalCount] = SqlTools::SearchProductTable_Kind_Price_Name(
        convert[selectedCategory],
        convert[selectedPrice],
        goodsName.toStdString(),
        goodsWidgetPagination->getPageSize(),
        0 // 从偏移量 0 开始
    );

    // 更新商品分页器
    goodsWidgetPagination->updatePagination(totalCount, goodsWidgetPagination->getPageSize());

    // 加载第一页的数据
    updateGoodsPage();
}

void ManagerWindowClass::onGoodstNameLineSearchClicked(const QString& text)
{
    QString selectedCategory = ui.goodsComboBoxCategory->currentText();
    QString selectedPrice = ui.goodsComboBoxPrice->currentText();

    // 调用 SqlTools 获取筛选后的商品总数
    auto [records, totalCount] = SqlTools::SearchProductTable_Kind_Price_Name(
        convert[selectedCategory],
        convert[selectedPrice],
        text.toStdString(),
        goodsWidgetPagination->getPageSize(),
        0 // 从偏移量 0 开始
    );

    // 更新商品分页器
    goodsWidgetPagination->updatePagination(totalCount, goodsWidgetPagination->getPageSize());

    // 加载第一页的数据
    updateGoodsPage();
}

void ManagerWindowClass::onAddGoodsClicked(const QString& id, int count)
{
    if (id.isEmpty() || count <= 0)
    {
        QMessageBox::warning(this, "警告", "请输入有效的商品ID和数量！");
        return;
    }

    // 调用 SqlTools::Stock_Goods 增加库存
    bool success = SqlTools::Stock_Goods(id.toStdString(), QString::number(count).toStdString());

    if (success)
    {
        QMessageBox::information(this, "成功", "库存已成功增加！");
        updateGoodsPage(); // 刷新商品显示
    }
    else
    {
        QMessageBox::warning(this, "失败", "库存增加失败，请检查商品ID！");
    }
}

void ManagerWindowClass::onDeleteGoodsClicked(const QString& id)
{
    if (id.isEmpty())
    {
        QMessageBox::warning(this, "警告", "请输入商品ID！");
        return;
    }

    // 调用 SqlTools::Delete_Goods 删除商品
    bool success = SqlTools::Delete_Goods(id.toStdString());

    if (success)
    {
        QMessageBox::information(this, "成功", "商品已成功删除！");
        updateGoodsPage(); // 刷新商品显示
    }
    else
    {
        QMessageBox::warning(this, "失败", "商品删除失败，请检查商品ID！");
    }
}

void ManagerWindowClass::updateGoodsPage()
{
    // 获取当前页的起始偏移量
    int offset = goodsWidgetPagination->getCurrentPage() * goodsWidgetPagination->getPageSize();
    int returnCount = goodsWidgetPagination->getPageSize();

    // 调用 SqlTools 获取当前页的数据
    auto [records, totalCount] = SqlTools::SearchProductTable_Kind_Price_Name(
        convert[ui.goodsComboBoxCategory->currentText()],
        convert[ui.goodsComboBoxPrice->currentText()],
        ui.goodsNameLine->text().toStdString(),
        returnCount,
        offset
    );

    // 清空当前商品数据
    vec_current_goods_widget.clear();

    for (const auto& record : records)
    {
        auto widget = std::make_unique<GoodsWidget>(
            nullptr,
            QString::fromStdString(record.path),
            QString::fromStdString(record.ID),
            QString::fromStdString(record.name),
            QString::fromStdString(record.price),
            QString::fromStdString(record.category),
            QString::fromStdString(record.count),
            QString::fromStdString(record.description)
        );

        connect(widget.get(), &GoodsWidget::clicked, this, &ManagerWindowClass::onGoodsClicked);

        vec_current_goods_widget.emplace_back(std::move(widget));
    }

    if (vec_current_goods_widget.size()<=9)
    {
        auto addGoo = std::make_unique<GoodsWidget>(nullptr, QString(":/res/Add_Icon.svg"));
        addGoo->setStyleSheet("QWidget { background-color: rgba(255,255,255,0.8); }");

        connect(addGoo.get(), &GoodsWidget::clicked, this, &ManagerWindowClass::onGoodsClicked);

        vec_current_goods_widget.push_back(std::move(addGoo));
        totalCount++;
    }
   
    // 提供一个回调函数，获取当前页的商品数据
    auto fetchPageData = [this](int start, int count) -> std::vector<GoodsWidget*> {
        std::vector<GoodsWidget*> widgets;
        for (int i = start; i < start + count && i < vec_current_goods_widget.size(); ++i) {
            widgets.push_back(vec_current_goods_widget[i].get());
        }
        return widgets;
        };

    // 调用 applyToGridLayout，传递回调函数
    goodsWidgetPagination->applyToGridLayout(ui.gridLayout, fetchPageData);

    // 更新分页标签和按钮状态
    ui.goodsWidgetPageLabel->setText(QString("第 %1/%2 页")
        .arg(goodsWidgetPagination->getCurrentPage() + 1)
        .arg(goodsWidgetPagination->pageCount()));
    ui.preGoodsWidgetPageBtn->setEnabled(goodsWidgetPagination->getCurrentPage() > 0);
    ui.nextGoodsWidgetPageBtn->setEnabled(goodsWidgetPagination->getCurrentPage() < goodsWidgetPagination->pageCount() - 1);
}

void ManagerWindowClass::onLogoutClicked() {
    UnifiedLoginManager::instance().localLogout("Manager");
    emit logoutRequested();
    this->close();
}

void ManagerWindowClass::onStaffChangedAcceptClicked()
{
    std::vector<StaffDetail> changedData;

    // 遍历 vec_changed_Staff_data，收集需要提交的数据
    for (const auto& staff : vec_changed_Staff_data) {
        changedData.push_back(*staff);
    }

    // 调用 SqlTools 更新数据库
    if (!changedData.empty()) {
        bool success = SqlTools::Change_StaffTable(changedData);
        if (success) {
            QMessageBox::information(this, "成功", "员工数据已成功更新！");
            vec_changed_Staff_data.clear(); // 清空修改记录
            updateStaffPage(); // 刷新页面
        }
        else {
            QMessageBox::warning(this, "失败", "员工数据更新失败！");
        }
    }
    else {
        QMessageBox::information(this, "提示", "没有需要提交的修改！");
    }
}

void ManagerWindowClass::onStaffChangedCancelClicked()
{
    // 清空修改记录
    vec_changed_Staff_data.clear();

    // 重新加载当前页数据
    updateStaffPage();

    QMessageBox::information(this, "提示", "所有修改已取消！");
}

void ManagerWindowClass::onStaffDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    QAbstractItemModel* model = ui.staffTableView->model();
    for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
        StaffDetail* staff = new StaffDetail();
        staff->staff_ID = model->data(model->index(row, 0)).toString().toStdString();
        staff->name = model->data(model->index(row, 1)).toString().toStdString();
        staff->email = model->data(model->index(row, 2)).toString().toStdString();
        staff->password = model->data(model->index(row, 3)).toString().toStdString();
        staff->join_date = model->data(model->index(row, 4)).toString().toStdString();
        staff->role = model->data(model->index(row, 5)).toString().toStdString();

        // 将修改记录存储到 vec_changed_Staff_data
        vec_changed_Staff_data.push_back(std::unique_ptr<StaffDetail>(staff));

        // 将修改后的数据字体变为红色
        for (int col = topLeft.column(); col <= bottomRight.column(); ++col) {
            model->setData(model->index(row, col), QBrush(Qt::red), Qt::ForegroundRole);
        }
    }
}

void ManagerWindowClass::onStaffComboxCategoryClicked(int index)
{
    QString selectedCategory = ui.staffComboBoxRole->itemText(index);
    staffPagination->setPage(0, 0); // 重置到第一页
    updateStaffPage();
}

void ManagerWindowClass::onstaffNameLineSearchClicked(const QString& text)
{
    staffPagination->setPage(0, 0); // 重置到第一页
    updateStaffPage();
}

void ManagerWindowClass::onAddStaffClicked()
{
    // 创建并显示 StaffDetailDialog
    StaffDetailDialog dialog(this);

    // 连接确定按钮的信号槽
    connect(dialog.getUi()->confirmButton, &QPushButton::clicked, [&]() {
        QString staffID = dialog.getUi()->staffIDEdit->text().trimmed();
        QString name = dialog.getUi()->nameEdit->text().trimmed();
        QString email = dialog.getUi()->emailEdit->text().trimmed();
        QString password = dialog.getUi()->passwordEdit->text().trimmed();
        QString joinDate = dialog.getUi()->joinDateEdit->text().trimmed();
        QString role = dialog.getUi()->roleEdit->text().trimmed();

        // 检查输入是否有效
        if (staffID.isEmpty() || name.isEmpty() || email.isEmpty() || password.isEmpty() || joinDate.isEmpty() || role.isEmpty()) {
            QMessageBox::warning(&dialog, "警告", "所有字段均为必填项！");
            return;
        }

        // 调用 SqlTools 添加员工记录
        StaffDetail newStaff(staffID.toStdString(), name.toStdString(), email.toStdString(), password.toStdString(), joinDate.toStdString(), role.toStdString());
        bool success = SqlTools::Add_Staff(newStaff);

        if (success) {
            QMessageBox::information(&dialog, "成功", "员工记录已成功添加！");
            dialog.accept(); // 关闭对话框
            updateStaffPage(); // 刷新员工页面
        }
        else {
            QMessageBox::warning(&dialog, "失败", "员工记录添加失败，请检查输入！");
        }
        });

    // 连接取消按钮的信号槽
    connect(dialog.getUi()->cancelButton, &QPushButton::clicked, [&]() {
        dialog.reject(); // 关闭对话框
        });

    dialog.exec(); // 显示对话框
}

void ManagerWindowClass::onDeleteStaffClicked()
{
    QString staffID = ui.staffDeleteEdit->text().trimmed(); // 获取输入的员工编号
    if (staffID.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入员工编号！");
        return;
    }

    // 调用 SqlTools 删除数据库中的记录
    bool success = SqlTools::Delete_Staff(staffID.toStdString());
    if (success) {
        QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.staffTableView->model());
        if (!model) return;

        // 遍历表格，找到对应的员工编号
        for (int row = 0; row < model->rowCount(); ++row) {
            QModelIndex index = model->index(row, 0); // 假设员工编号在第0列
            if (model->data(index).toString() == staffID) {
                model->removeRow(row); // 删除该行
                break;
            }
        }

        QMessageBox::information(this, "成功", "员工记录已成功删除！");
        updateStaffPage(); // 刷新页面
    }
    else {
        QMessageBox::warning(this, "失败", "删除员工记录失败，请检查员工编号！");
    }
}

void ManagerWindowClass::onCustomerChangedAcceptClicked()
{
    std::vector<CustomerDetail> changedData;

    // 遍历 vec_changed_Customer_data，收集需要提交的数据
    for (const auto& customer : vec_changed_Customer_data) {
        changedData.push_back(*customer);
    }

    // 调用 SqlTools 更新数据库
    if (!changedData.empty()) {
        bool success = SqlTools::Change_CustomerTable(changedData);
        if (success) {
            QMessageBox::information(this, "成功", "顾客数据已成功更新！");
            vec_changed_Customer_data.clear(); // 清空修改记录
            updateCustomerPage(); // 刷新页面
        }
        else {
            QMessageBox::warning(this, "失败", "顾客数据更新失败！");
        }
    }
    else {
        QMessageBox::information(this, "提示", "没有需要提交的修改！");
    }
}




void ManagerWindowClass::onCustomerChangedCancelClicked()
{
    // 清空修改记录
    vec_changed_Customer_data.clear();

    // 重新加载当前页数据
    updateCustomerPage();

    QMessageBox::information(this, "提示", "所有修改已取消！");
}






void ManagerWindowClass::onDeleteCustomerClicked()
{
    QString customerId = ui.customerDeleteEdit->text().trimmed(); // 获取输入的顾客ID
    if (customerId.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入顾客ID！");
        return;
    }

    // 调用 SqlTools 删除数据库中的记录
    bool success = SqlTools::Delete_Customer(customerId.toStdString());
    if (success) {
        // 遍历表格，找到对应的顾客ID
        for (int row = 0; row < customerModel->rowCount(); ++row) {
            QModelIndex index = customerModel->index(row, 1); // 假设顾客ID在第1列
            if (customerModel->data(index).toString() == customerId) {
                customerModel->removeRow(row); // 删除该行
                break;
            }
        }

        QMessageBox::information(this, "成功", "顾客记录已成功删除！");
        updateCustomerPage(); // 刷新页面
    }
    else {
        QMessageBox::warning(this, "失败", "删除顾客记录失败，请检查顾客ID！");
    }
}

void ManagerWindowClass::onDateRangeChanged()
{
    // 获取日期范围
    QDate startDate = ui.dateEditStart->date();
    QDate endDate = ui.dateEditEnd->date();
    QString startDateStr = startDate.toString("yyyy-MM-dd");
    QString endDateStr = endDate.toString("yyyy-MM-dd");

    // 从 SqlTools 获取收入数据
    QVector<QPair<QDateTime, double>> incomeData = SqlTools::Get_Income_Date(startDateStr.toStdString(), endDateStr.toStdString());

    // 更新 FinanceChart 数据和坐标轴
    FinanceChart* financeChart = qobject_cast<FinanceChart*>(ui.chartLay->layout()->itemAt(0)->widget());
    if (financeChart) {
        financeChart->setIncomeData(incomeData);
        financeChart->setAxisRange(startDate, endDate);  // 添加的坐标轴范围设置函数
    }
}

void ManagerWindowClass::onSaleHeaderClicked(int column)
{
    // 重置其他列的排序状态
    for (int i = 0; i < 5; ++i) {
        if (i != column) {
            saleReverseFlags[i] = false;
        }
    }

    // 切换当前列的排序状态
    saleReverseFlags[column] = !saleReverseFlags[column];
    currentSaleSortColumn = column;

    // 更新销售表格数据
    updateSalePage();
}

void ManagerWindowClass::onStockHeaderClicked(int column)
{
    for (int i = 0; i < 4; ++i) {
        if (i != column) {
            stockReverseFlags[i] = false;
        }
    }

    stockReverseFlags[column] = !stockReverseFlags[column];
    currentStockSortColumn = column;

    updateStockPage();
}

void ManagerWindowClass::onStaffHeaderClicked(int column)
{
    for (int i = 0; i < 6; ++i) {
        if (i != column) {
            staffReverseFlags[i] = false;
        }
    }

    staffReverseFlags[column] = !staffReverseFlags[column];
    currentStaffSortColumn = column;

    updateStaffPage();
}

void ManagerWindowClass::onCustomerHeaderClicked(int column)
{
    for (int i = 0; i < 7; ++i) {
        if (i != column) {
            customerReverseFlags[i] = false;
        }
    }

    customerReverseFlags[column] = !customerReverseFlags[column];
    currentCustomerSortColumn = column;

    updateCustomerPage();
}



void ManagerWindowClass::onCustomerDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    QAbstractItemModel* model = ui.customerTableView->model();
    for (int row = topLeft.row(); row <= bottomRight.row(); ++row) 
    {
        CustomerDetail* customer = new CustomerDetail();
        customer->profile_picture = model->data(model->index(row, 0), Qt::DecorationRole).toString().toStdString();
        customer->customer_ID = model->data(model->index(row, 1)).toString().toStdString();
        customer->birth_date = model->data(model->index(row, 2)).toString().toStdString();
        customer->note = model->data(model->index(row, 3)).toString().toStdString();
        customer->register_date = model->data(model->index(row, 4)).toString().toStdString();
        customer->email = model->data(model->index(row, 5)).toString().toStdString();
        customer->password = model->data(model->index(row, 6)).toString().toStdString();

        // 将修改记录存储到 vec_changed_Customer_data
        vec_changed_Customer_data.push_back(std::unique_ptr<CustomerDetail>(customer));

        // 将修改后的数据字体变为红色
        for (int col = topLeft.column(); col <= bottomRight.column(); ++col) 
        {
            model->setData(model->index(row, col), QBrush(Qt::red), Qt::ForegroundRole);
        }
    }
}



void ManagerWindowClass::onCustomerNameLineSearchClicked(const QString& text)
{
    customerPagination->setPage(0, 0); // 重置到第一页
    updateCustomerPage();
}


void ManagerWindowClass::onAddCustomerClicked()
{
    // 创建并显示 CustomerDetailDialog
    CustomerDetailDialog dialog(this);

    // 连接确定按钮的信号槽
    connect(&dialog, &CustomerDetailDialog::dataAccepted, this,
        [=](const QString& id, const QString& birthDate,
            const QString& logDate, const QString& email,
            const QString& password, const QString& description,
            const QString& avatarPath)
        {
            // 检查输入是否有效
            if (id.isEmpty() || birthDate.isEmpty() || logDate.isEmpty() || email.isEmpty() || password.isEmpty()) {
                QMessageBox::warning(this, "警告", "所有字段均为必填项！");
                return;
            }

            // 创建新顾客数据
            QList<QStandardItem*> rowItems;

            // 头像
            QPixmap avatar(avatarPath);
            QStandardItem* avatarItem = new QStandardItem();
            avatarItem->setData(QVariant(avatar.scaled(50, 50, Qt::KeepAspectRatio)), Qt::DecorationRole);
            avatarItem->setEditable(false); // 头像不可直接编辑
            rowItems << avatarItem;

            // 其他列
            rowItems << new QStandardItem(id);
            rowItems << new QStandardItem(birthDate);
            rowItems << new QStandardItem(description);
            rowItems << new QStandardItem(logDate);
            rowItems << new QStandardItem(email);
            rowItems << new QStandardItem(password);

            // 将新行添加到模型中
            customerModel->appendRow(rowItems);

            // 更新分页
            updateCustomerPage();

            QMessageBox::information(this, "成功", "新顾客已成功添加！");
        });

    dialog.exec(); // 以模态方式显示对话框
}



void ManagerWindowClass::onCustomerAvatarDoubleClicked(const QModelIndex& index)
{
    if (index.column() == 0) { // 头像列
        QString filePath = QFileDialog::getOpenFileName(this, "选择头像", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!filePath.isEmpty()) {
            QPixmap avatar(filePath);
            customerModel->setData(index, QVariant(avatar.scaled(50, 50, Qt::KeepAspectRatio)), Qt::DecorationRole);

            // 记录头像修改到 vec_changed_Customer_data
            int row = index.row();
            CustomerDetail* customer = new CustomerDetail();
            customer->profile_picture = filePath.toStdString();
            customer->customer_ID = customerModel->data(customerModel->index(row, 1)).toString().toStdString();
            customer->birth_date = customerModel->data(customerModel->index(row, 2)).toString().toStdString();
            customer->note = customerModel->data(customerModel->index(row, 3)).toString().toStdString();
            customer->register_date = customerModel->data(customerModel->index(row, 4)).toString().toStdString();
            customer->email = customerModel->data(customerModel->index(row, 5)).toString().toStdString();
            customer->password = customerModel->data(customerModel->index(row, 6)).toString().toStdString();

            vec_changed_Customer_data.push_back(std::unique_ptr<CustomerDetail>(customer));
        }
    }
}



void ManagerWindowClass::updateCustomerPage()
{
    // 获取当前页的偏移量和返回数量
    int offset = customerPagination->getCurrentPage() * customerPagination->getPageSize();
    int limit = customerPagination->getPageSize();

    // 调用 SqlTools 获取当前页的顾客数据
    auto [records, totalCount] = SqlTools::Search_CustomerTable_Name(
        ui.customerNameLine->text().toStdString(),
        limit,
        offset,
        customerReverseFlags[2], // reverse_birth_date
        customerReverseFlags[4]  // reverse_register_date
    );

    // 清空表格模型
    customerModel->removeRows(0, customerModel->rowCount());

    // 填充表格数据
    for (const auto& record : records)
    {
        QList<QStandardItem*> rowItems;

        // 头像
        QPixmap avatar(QString::fromStdString(record.profile_picture));
        QStandardItem* avatarItem = new QStandardItem();
        avatarItem->setData(QVariant(avatar.scaled(50, 50, Qt::KeepAspectRatio)), Qt::DecorationRole);
        avatarItem->setEditable(false); // 头像不可直接编辑
        rowItems << avatarItem;

        // 其他列
        rowItems << new QStandardItem(QString::fromStdString(record.customer_ID));
        rowItems << new QStandardItem(QString::fromStdString(record.birth_date));
        rowItems << new QStandardItem(QString::fromStdString(record.note));
        rowItems << new QStandardItem(QString::fromStdString(record.register_date));
        rowItems << new QStandardItem(QString::fromStdString(record.email));
        rowItems << new QStandardItem(QString::fromStdString(record.password));

        customerModel->appendRow(rowItems);
    }

    // 更新分页标签和按钮状态
    ui.customerPageLabel->setText(QString("第 %1/%2 页")
        .arg(customerPagination->getCurrentPage() + 1)
        .arg(customerPagination->pageCount(totalCount)));
    ui.preCustomerPageBtn->setEnabled(customerPagination->getCurrentPage() > 0);
    ui.nextCustomerPageBtn->setEnabled(customerPagination->getCurrentPage() < customerPagination->pageCount(totalCount) - 1);
}





