#include "ManagerWindowClass.h"

ManagerWindowClass::ManagerWindowClass(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.stackedWidget->setCurrentIndex(0);
    // ���ô��ڱ���ͼƬ
    QPixmap background(":/res/manager.png");  // ����Դ�ļ��м���ͼƬ
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // ����ͼƬ����Ӧ����
    QPalette palette;
    palette.setBrush(QPalette::Window, background);  // ���ñ���ͼƬ
    this->setPalette(palette);
    //������Ʒչʾ����
    for (int i = 0; i < 19; i++)
    {
        GoodsWidget* goo = new GoodsWidget;
        goo->setStyleSheet("QWidget { background-color: rgba(255,255,255,0.8); }");
        vec_current_goods_widget.push_back(goo);
        //���������ϸҳ��
        connect(goo, &GoodsWidget::clicked, this, &ManagerWindowClass::onGoodsClicked);
    }
    //���ù�������
    ui.scrollAreaWidgetContents_3->setMaximumSize(1600, 800);
    ui.scrollAreaWidgetContents_3->setMinimumSize(1600, 800);
    ui.gridLayout->setSpacing(5);
    ui.gridLayout->setContentsMargins(10, 10, 10, 10);
    ui.scrollArea->setWidgetResizable(true);
    ui.scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui.scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    //���õǳ���ť
    connect(ui.logOutBtn, &QPushButton::clicked, this, &ManagerWindowClass::onLogoutClicked);

    // ������Ʒ��ҳ��ť
    connect(ui.nextGoodsWidgetPageBtn, &QPushButton::clicked, this, &ManagerWindowClass::onNextPageClicked);
    connect(ui.preGoodsWidgetPageBtn, &QPushButton::clicked, this, &ManagerWindowClass::onPrevPageClicked);

    // ��ʼ����Ʒ��ҳ������
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
    ui.goodsWidgetPageLabel->setText(QString("�� %1/%2 ҳ").arg(goodsWidgetPagination->getCurrentPage() + 1).arg(goodsWidgetPagination->pageCount()));
    ui.preGoodsWidgetPageBtn->setEnabled(goodsWidgetPagination->getCurrentPage() > 0);
    ui.nextGoodsWidgetPageBtn->setEnabled(goodsWidgetPagination->getCurrentPage() < goodsWidgetPagination->pageCount() - 1);
}

void ManagerWindowClass::onLogoutClicked()
{
    emit logoutRequested();
    this->close();
}
