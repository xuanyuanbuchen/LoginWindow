#include "ManagerWindowClass.h"

ManagerWindowClass::ManagerWindowClass(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.stackedWidget->setCurrentIndex(0);
    // 设置窗口背景图片
    QPixmap background(":/res/manager.png");  // 从资源文件中加载图片
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 缩放图片以适应窗口
    QPalette palette;
    palette.setBrush(QPalette::Window, background);  // 设置背景图片
    this->setPalette(palette);
    //设置商品展示网格
    for (int i = 0; i < 19; i++)
    {
        GoodsWidget* goo = new GoodsWidget;
        goo->setStyleSheet("QWidget { background-color: rgba(255,255,255,0.8); }");
        vec_current_goods_widget.push_back(goo);
        //点击唤出详细页面
        connect(goo, &GoodsWidget::clicked, this, &ManagerWindowClass::onGoodsClicked);
    }
    //设置滚动区域
    ui.scrollAreaWidgetContents_3->setMaximumSize(1600, 800);
    ui.scrollAreaWidgetContents_3->setMinimumSize(1600, 800);
    ui.gridLayout->setSpacing(5);
    ui.gridLayout->setContentsMargins(10, 10, 10, 10);
    ui.scrollArea->setWidgetResizable(true);
    ui.scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui.scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    //设置登出按钮
    connect(ui.logOutBtn, &QPushButton::clicked, this, &ManagerWindowClass::onLogoutClicked);

    // 设置商品分页按钮
    connect(ui.nextGoodsWidgetPageBtn, &QPushButton::clicked, this, &ManagerWindowClass::onNextPageClicked);
    connect(ui.preGoodsWidgetPageBtn, &QPushButton::clicked, this, &ManagerWindowClass::onPrevPageClicked);

    // 初始化商品分页管理器
    goodsWidgetPagination = new GoodsWidgetPagination(vec_current_goods_widget, 9);
    updateGoodsPage();
}

ManagerWindowClass::~ManagerWindowClass()
{
    vec_current_goods_widget.clear();
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

void ManagerWindowClass::onGoodsClicked()
{
    GoodsWidget* goodsWidget = qobject_cast<GoodsWidget*>(sender());
    if (goodsWidget) {
        goodsWidget->showGoodsDetailDialog();
    }
}

void ManagerWindowClass::updateGoodsPage()
{
    goodsWidgetPagination->applyToGridLayout(ui.gridLayout);
    ui.goodsWidgetPageLabel->setText(QString("第 %1/%2 页").arg(goodsWidgetPagination->getCurrentPage() + 1).arg(goodsWidgetPagination->pageCount()));
    ui.preGoodsWidgetPageBtn->setEnabled(goodsWidgetPagination->getCurrentPage() > 0);
    ui.nextGoodsWidgetPageBtn->setEnabled(goodsWidgetPagination->getCurrentPage() < goodsWidgetPagination->pageCount() - 1);
}

void ManagerWindowClass::onLogoutClicked()
{
    emit logoutRequested();
    this->close();
}
