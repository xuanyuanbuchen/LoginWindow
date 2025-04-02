#include "CashierWindowClass.h"
#include <qstandarditemmodel.h>
#include <qtimezone.h>

CashierWindowClass::CashierWindowClass(QWidget *parent)
    : QWidget(parent), currentPage(0), totalPages(0)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui.stackedWidget->setCurrentIndex(0);
    // 设置窗口背景图片
    QPixmap background(":/res/supermarket.jpg");  // 从资源文件中加载图片
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 缩放图片以适应窗口
    QPalette palette;
    palette.setBrush(QPalette::Window, background);  // 设置背景图片
    this->setPalette(palette);
    //设置商品展示网格
    for (int i = 0; i < 19; i++)
    {
        ProductWidget* pro = new ProductWidget;
        pro->setStyleSheet("QWidget { background-color: rgba(255,255,255,0.8); }");
        vec_current_produc_widget.push_back(pro);
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
    //设置订单展示和处理表格
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({ "订单ID","顾客ID","订单时间","总金额","订单状态","操作" });

    ui.tableView->setModel(model);
    ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableView->verticalHeader()->setVisible(false); // 隐藏行号
    ui.tableView->horizontalHeader()->setStretchLastSection(true); // 最后一列拉伸
    ui.tableView->setStyleSheet("QTableView { border: 1px solid #ddd; }");
    // 添加初始数据（连接数据库后可以注释掉）
    for (int i = 0; i < 50; ++i) {
        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(QString("ORD%1").arg(i + 1, 4, 10, QLatin1Char('0')));
        rowItems << new QStandardItem(QString("CUS%1").arg(1000 + i));
        rowItems << new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        rowItems << new QStandardItem(QString("%1元").arg((i + 1) * 100, 0, 'f', 2));
        rowItems << new QStandardItem(i % 3 == 0 ? QString("待处理") : (i % 3 == 1 ? QString("已发货") : QString("已完成")));

        // 添加操作按钮
        QPushButton* btn = new QPushButton("同意",this);
        btn->setProperty("row", i); // 存储行索引

        //执行的操作（有数据库里再写）
        connect(btn, &QPushButton::clicked, [=]() {
            btn->setText("操作完成");
        });

        // 将按钮放入表格
        QWidget* widget = new QWidget;
        QHBoxLayout* layout = new QHBoxLayout(widget);
        layout->addWidget(btn);
        layout->setAlignment(Qt::AlignCenter);
        layout->setContentsMargins(0, 0, 0, 0);
        widget->setLayout(layout);

        model->appendRow(rowItems);
        ui.tableView->setIndexWidget(model->index(i, 5), widget);
    }
    //初始化订单分页管理器
    orderPagination = new TableViewPagination(model, 30);
    //更新数据
    updateOrderPage();

    connect(ui.preOrderPageBtn, &QPushButton::clicked, [=]() {
        orderPagination->setPage(orderPagination->getCurrentPage() - 1);
        updateOrderPage();
    });
    connect(ui.nextOrderPageBtn, &QPushButton::clicked, [=]() {
        orderPagination->setPage(orderPagination->getCurrentPage() + 1);
        updateOrderPage();
    });

	//设置页面跳转按钮
    connect(ui.checkGoodsBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(0);
    });

    connect(ui.processOrderBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(1);
    });
    connect(ui.checkSalesInformationBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(2);
    });
    //设置登出按钮
    connect(ui.logOutBtn, &QPushButton::clicked, this, &CashierWindowClass::onLogoutClicked);

    // 设置商品分页按钮
    connect(ui.nextProductWidgetPageBtn, &QPushButton::clicked, this, &CashierWindowClass::onNextPageClicked);
    connect(ui.preProductWidgetPageBtn, &QPushButton::clicked, this, &CashierWindowClass::onPrevPageClicked);

    // 初始化商品分页管理器
    productWidgetPagination = new ProductWidgetPagination(vec_current_produc_widget, 9);
    updateProductPage();
}

CashierWindowClass::~CashierWindowClass()
{
    vec_current_produc_widget.clear();
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
    orderPagination->applyToTableView(ui.tableView);
    ui.orderPageLabel->setText(QString("第 %1/%2 页").arg(orderPagination->getCurrentPage() + 1).arg(orderPagination->pageCount()));
    ui.preOrderPageBtn->setEnabled(orderPagination->getCurrentPage() > 0);
    ui.nextOrderPageBtn->setEnabled(orderPagination->getCurrentPage() < orderPagination->pageCount() - 1);
}

void CashierWindowClass::loadPage(int page)
{
}

void CashierWindowClass::onLogoutClicked()
{
    emit logoutRequested();
    this->close();
}

void CashierWindowClass::updateProductPage()
{
    productWidgetPagination->applyToGridLayout(ui.gridLayout);
    ui.productWidgetPageLabel->setText(QString("第 %1/%2 页").arg(productWidgetPagination->getCurrentPage() + 1).arg(productWidgetPagination->pageCount()));
    ui.preProductWidgetPageBtn->setEnabled(productWidgetPagination->getCurrentPage() > 0);
    ui.nextProductWidgetPageBtn->setEnabled(productWidgetPagination->getCurrentPage() < productWidgetPagination->pageCount() - 1);
}
