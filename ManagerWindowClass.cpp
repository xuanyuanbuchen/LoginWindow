#include "ManagerWindowClass.h"
#include <qstandarditemmodel.h>
#include <qdatetime.h>
#include <qmessagebox.h>

ManagerWindowClass::ManagerWindowClass(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
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
        GoodsWidget* goo = new GoodsWidget(nullptr);
        goo->setStyleSheet("QWidget { background-color: rgba(255,255,255,0.8); }");
        vec_current_goods_widget.push_back(goo);
        //点击唤出详细页面
        connect(goo, &GoodsWidget::clicked, this, &ManagerWindowClass::onGoodsClicked);
    }
	//最后一个用来添加新的商品，添加完新的商品再添加一个的功能有了数据库后再实现
	GoodsWidget* addGoo = new GoodsWidget(QString(":/res/Add_Icon.svg"));
    addGoo->setStyleSheet("QWidget { background-color: rgba(255,255,255,0.8); }");
    vec_current_goods_widget.push_back(addGoo);
    //点击唤出详细页面
    connect(addGoo, &GoodsWidget::clicked, this, &ManagerWindowClass::onGoodsClicked);
    //设置滚动区域
    ui.scrollAreaWidgetContents_3->setMaximumSize(1600, 800);
    ui.scrollAreaWidgetContents_3->setMinimumSize(1600, 800);
    ui.gridLayout->setSpacing(5);
    ui.gridLayout->setContentsMargins(10, 10, 10, 10);
    ui.scrollArea->setWidgetResizable(true);
    ui.scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui.scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    //设置销售记录展示和处理表格
    QStandardItemModel* salemodel = new QStandardItemModel(this);
    salemodel->setHorizontalHeaderLabels({ "订单ID","顾客ID","订单时间","总金额","订单状态","操作" });

    ui.saleTableView->setModel(salemodel);
    ui.saleTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.saleTableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui.saleTableView->verticalHeader()->setVisible(false); // 隐藏行号
    ui.saleTableView->horizontalHeader()->setStretchLastSection(true); // 最后一列拉伸
    ui.saleTableView->setStyleSheet("QTableView { border: 1px solid #ddd; }");
    // 添加初始数据（连接数据库后可以注释掉）
    for (int i = 0; i < 50; ++i) {
        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(QString("ORD%1").arg(i + 1, 4, 10, QLatin1Char('0')));
        rowItems << new QStandardItem(QString("CUS%1").arg(1000 + i));
        rowItems << new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        rowItems << new QStandardItem(QString("%1元").arg((i + 1) * 100, 0, 'f', 2));
        rowItems << new QStandardItem(i % 3 == 0 ? QString("待处理") : (i % 3 == 1 ? QString("已发货") : QString("已完成")));

        // 添加操作按钮
        QPushButton* btn = new QPushButton("同意", this);
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

        salemodel->appendRow(rowItems);
        ui.saleTableView->setIndexWidget(salemodel->index(i, 5), widget);
    }
    //初始化订单分页管理器
    salePagination = new TableViewPagination(salemodel, 30);
    //更新数据
    updateSalePage();

    connect(ui.preSalePageBtn, &QPushButton::clicked, [=]() {
        salePagination->setPage(salePagination->getCurrentPage() - 1);
        updateSalePage();
        });
    connect(ui.nextSalePageBtn, &QPushButton::clicked, [=]() {
        salePagination->setPage(salePagination->getCurrentPage() + 1);
        updateSalePage();
        });
    connect(ui.addSaleBtn, &QPushButton::clicked, this, &ManagerWindowClass::onAddOrderClicked);








    //设置员工展示和处理表格
    QStandardItemModel* staffmodel = new QStandardItemModel(this);
    staffmodel->setHorizontalHeaderLabels({ "员工ID","名字","邮箱","密码","加入时间","职责" });

    ui.staffTableView->setModel(staffmodel);
    ui.staffTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.staffTableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui.staffTableView->verticalHeader()->setVisible(false); // 隐藏行号
    ui.staffTableView->horizontalHeader()->setStretchLastSection(true); // 最后一列拉伸
    ui.staffTableView->setStyleSheet("QTableView { border: 1px solid #ddd; }");
    // 添加初始数据（连接数据库后可以注释掉）
    for (int i = 0; i < 50; ++i) {
        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(QString("STF%1").arg(1000 + i));
        rowItems << new QStandardItem(QString("员工%1").arg(i + 1));
        rowItems << new QStandardItem(QString("employee%1@example.com").arg(i + 1));
        rowItems << new QStandardItem(QString("password%1").arg(i + 1));
        rowItems << new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        rowItems << new QStandardItem(i % 2 == 0 ? QString("店长") : QString("员工"));
        staffmodel->appendRow(rowItems);
       
    }
    //初始化订单分页管理器
    staffPagination = new TableViewPagination(staffmodel, 30);
    //更新数据
    updateStaffPage();

    connect(ui.preStaffPageBtn, &QPushButton::clicked, [=]() {
        staffPagination->setPage(staffPagination->getCurrentPage() - 1);
        updateStaffPage();
        });
    connect(ui.nextStaffPageBtn, &QPushButton::clicked, [=]() {
        staffPagination->setPage(staffPagination->getCurrentPage() + 1);
        updateStaffPage();
        });










	//设置页面跳转按钮
    connect(ui.manageGoodsBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(0);
        });
	connect(ui.processSalesInfoBtn, &QPushButton::clicked, [=]() {
		ui.stackedWidget->setCurrentIndex(1);
		});
    connect(ui.manageStaffBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(2);
        });
    connect(ui.managerCustomBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(3);
        });
    connect(ui.financialManagerBtn, &QPushButton::clicked, [=]() {
        ui.stackedWidget->setCurrentIndex(4);
        });
    //设置登出按钮
    connect(ui.logOutBtn, &QPushButton::clicked, this, &ManagerWindowClass::onLogoutClicked);

    // 设置商品分页按钮
    connect(ui.nextGoodsWidgetPageBtn, &QPushButton::clicked, this, &ManagerWindowClass::onNextPageClicked);
    connect(ui.preGoodsWidgetPageBtn, &QPushButton::clicked, this, &ManagerWindowClass::onPrevPageClicked);

    // 初始化商品分页管理器
    goodsWidgetPagination = new GoodsWidgetPagination(vec_current_goods_widget, 9);
    updateGoodsPage();

    //连接表格操作
    connect(ui.saleChangedAcceptBtn, &QPushButton::clicked, this, &ManagerWindowClass::onSaleChangedAcceptClicked);
    connect(ui.saleChangedCancelBtn, &QPushButton::clicked, this, &ManagerWindowClass::onSaleChangedCancelClicked);
    connect(ui.saleTableView->model(), &QAbstractItemModel::dataChanged, this, &ManagerWindowClass::onSaleDataChanged);
    connect(ui.staffChangedAcceptBtn, &QPushButton::clicked, this, &ManagerWindowClass::onStaffChangedAcceptClicked);
    connect(ui.staffChangedCancelBtn, &QPushButton::clicked, this, &ManagerWindowClass::onStaffChangedCancelClicked);
    connect(ui.addStaffBtn, &QPushButton::clicked, this, &ManagerWindowClass::onAddStaffClicked);
    connect(ui.staffTableView->model(), &QAbstractItemModel::dataChanged, this, &ManagerWindowClass::onStaffDataChanged);

    //删除按钮连接
    connect(ui.goodsDeleteBtn, &QPushButton::clicked, this, &ManagerWindowClass::onDeleteGoodsClicked);
    connect(ui.saleDeleteBtn, &QPushButton::clicked, this, &ManagerWindowClass::onDeleteSaleClicked);
    connect(ui.staffDeleteBtn, &QPushButton::clicked, this, &ManagerWindowClass::onDeleteStaffClicked);



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

void ManagerWindowClass::updateSalePage()
{
    salePagination->applyToTableView(ui.saleTableView);
    ui.salePageLabel->setText(QString("第 %1/%2 页").arg(salePagination->getCurrentPage() + 1).arg(salePagination->pageCount()));
    ui.preSalePageBtn->setEnabled(salePagination->getCurrentPage() > 0);
    ui.nextSalePageBtn->setEnabled(salePagination->getCurrentPage() < salePagination->pageCount() - 1);
}

void ManagerWindowClass::updateStaffPage()
{
    staffPagination->applyToTableView(ui.staffTableView);
    ui.staffPageLabel->setText(QString("第 %1/%2 页").arg(staffPagination->getCurrentPage() + 1).arg(staffPagination->pageCount()));
    ui.preStaffPageBtn->setEnabled(staffPagination->getCurrentPage() > 0);
    ui.nextStaffPageBtn->setEnabled(staffPagination->getCurrentPage() < staffPagination->pageCount() - 1);
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

void ManagerWindowClass::onSaleChangedAcceptClicked()
{
    QAbstractItemModel* model = ui.saleTableView->model();
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QModelIndex index = model->index(row, col);
            QVariant data = model->data(index, Qt::ForegroundRole);
            if (data.isValid() && data.value<QBrush>().color() == Qt::red) {
                // 生成SQL语句并执行
                QString orderId = model->data(model->index(row, 0)).toString();
				qDebug() << orderId;
                QString columnName = model->headerData(col, Qt::Horizontal).toString();
                QString newValue = model->data(index).toString();
               /* QSqlQuery query;
                query.prepare(QString("UPDATE orders SET %1 = :value WHERE order_id = :order_id").arg(columnName));
                query.bindValue(":value", newValue);
                query.bindValue(":order_id", orderId);*/
                // 以后要删除
                model->setData(index, QBrush(Qt::black), Qt::ForegroundRole);
				qDebug() << "UPDATE orders SET " << columnName << " = " << newValue << " WHERE order_id = " << orderId;
                if (false) {
                    //qWarning() << "Failed to update database:" << query.lastError();
					

                }
                else {
                    // 修改成功后将字体颜色恢复为黑色
                    model->setData(index, QBrush(Qt::black), Qt::ForegroundRole);
                }
            }
        }
    }
}

void ManagerWindowClass::onSaleChangedCancelClicked()
{
    QAbstractItemModel* model = ui.saleTableView->model();
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QModelIndex index = model->index(row, col);
            QVariant data = model->data(index, Qt::ForegroundRole);
            if (data.isValid() && data.value<QBrush>().color() == Qt::red) {
                // 恢复原始数据
                QString orderId = model->data(model->index(row, 0)).toString();
                QString columnName = model->headerData(col, Qt::Horizontal).toString();
                /*QSqlQuery query;
                query.prepare(QString("SELECT %1 FROM orders WHERE order_id = :order_id").arg(columnName));
                query.bindValue(":order_id", orderId);*/
                if (true) {
                    //QString originalValue = query.value(0).toString();
                    //model->setData(index, originalValue);
                    // 恢复字体颜色为黑色
                    model->setData(index, QBrush(Qt::black), Qt::ForegroundRole);
                }
            }
        }
    }
}

void ManagerWindowClass::onSaleDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    QAbstractItemModel* model = ui.saleTableView->model();
    for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
        for (int col = topLeft.column(); col <= bottomRight.column(); ++col) {
            QModelIndex index = model->index(row, col);
            // 将修改后的数据字体变为红色
            model->setData(index, QBrush(Qt::red), Qt::ForegroundRole);
        }
    }
}

void ManagerWindowClass::onAddOrderClicked()
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.saleTableView->model());
    if (!model) return;

    // 创建新的订单数据
    QList<QStandardItem*> rowItems;
    int newRow = model->rowCount();
    rowItems << new QStandardItem(QString("ORD%1").arg(newRow + 1, 4, 10, QLatin1Char('0')));
    rowItems << new QStandardItem(QString("CUS%1").arg(1000 + newRow));
    rowItems << new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    rowItems << new QStandardItem(QString("%1元").arg((newRow + 1) * 100, 0, 'f', 2));
    rowItems << new QStandardItem(QString("待处理"));

    // 添加操作按钮
    QPushButton* btn = new QPushButton("同意", this);
    btn->setProperty("row", newRow); // 存储行索引

    // 执行的操作（有数据库里再写）
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

    // 将新行添加到模型中
    model->appendRow(rowItems);
    ui.saleTableView->setIndexWidget(model->index(newRow, 5), widget);

    // 更新分页
    updateSalePage();
}

void ManagerWindowClass::onStaffChangedAcceptClicked()
{
    QAbstractItemModel* model = ui.staffTableView->model();
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QModelIndex index = model->index(row, col);
            QVariant data = model->data(index, Qt::ForegroundRole);
            if (data.isValid() && data.value<QBrush>().color() == Qt::red) {
                // 生成SQL语句并执行
                QString staffId = model->data(model->index(row, 0)).toString();
                QString columnName = model->headerData(col, Qt::Horizontal).toString();
                QString newValue = model->data(index).toString();
                /*QSqlQuery query;
                query.prepare(QString("UPDATE staff SET %1 = :value WHERE staff_id = :staff_id").arg(columnName));
                query.bindValue(":value", newValue);
                query.bindValue(":staff_id", staffId);*/
                if (true) {
                    //qWarning() << "Failed to update database:" << query.lastError();
                }
                else {
                    // 修改成功后将字体颜色恢复为黑色
                    model->setData(index, QBrush(Qt::black), Qt::ForegroundRole);
                }
            }
        }
    }
}

void ManagerWindowClass::onStaffChangedCancelClicked()
{
    QAbstractItemModel* model = ui.staffTableView->model();
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QModelIndex index = model->index(row, col);
            QVariant data = model->data(index, Qt::ForegroundRole);
            if (data.isValid() && data.value<QBrush>().color() == Qt::red) {
                // 恢复原始数据
                QString staffId = model->data(model->index(row, 0)).toString();
                QString columnName = model->headerData(col, Qt::Horizontal).toString();
               /* QSqlQuery query;
                query.prepare(QString("SELECT %1 FROM staff WHERE staff_id = :staff_id").arg(columnName));
                query.bindValue(":staff_id", staffId);*/
                if (true) {
                    //QString originalValue = query.value(0).toString();
                    //model->setData(index, originalValue);
                    // 恢复字体颜色为黑色
                    model->setData(index, QBrush(Qt::black), Qt::ForegroundRole);
                }
            }
        }
    }
}

void ManagerWindowClass::onStaffDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    QAbstractItemModel* model = ui.staffTableView->model();
    for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
        for (int col = topLeft.column(); col <= bottomRight.column(); ++col) {
            QModelIndex index = model->index(row, col);
            // 将修改后的数据字体变为红色
            model->setData(index, QBrush(Qt::red), Qt::ForegroundRole);
        }
    }
}

void ManagerWindowClass::onAddStaffClicked()
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.staffTableView->model());
    if (!model) return;

    // 创建新的员工数据
    QList<QStandardItem*> rowItems;
    int newRow = model->rowCount();
    rowItems << new QStandardItem(QString("STF%1").arg(1000 + newRow));
    rowItems << new QStandardItem(QString("员工%1").arg(newRow + 1));
    rowItems << new QStandardItem(QString("employee%1@example.com").arg(newRow + 1));
    rowItems << new QStandardItem(QString("password%1").arg(newRow + 1));
    rowItems << new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    rowItems << new QStandardItem(newRow % 2 == 0 ? QString("店长") : QString("员工"));

    // 将新行添加到模型中
    model->appendRow(rowItems);

    // 更新分页
    updateStaffPage();
}

void ManagerWindowClass::onDeleteGoodsClicked()
{
    QString goodsId = ui.goodsDeleteEdit->text().trimmed(); // 获取输入的商品编号
    if (goodsId.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入商品编号！");
        return;
    }

    // 遍历商品列表，找到对应的商品
    auto it = std::find_if(vec_current_goods_widget.begin(), vec_current_goods_widget.end(),
        [&goodsId](GoodsWidget* widget) {
            return widget->getID() == goodsId;
        });

    if (it != vec_current_goods_widget.end()) {
        GoodsWidget* widgetToDelete = *it;

        // 从布局中移除并删除商品
        ui.gridLayout->removeWidget(widgetToDelete);
        vec_current_goods_widget.erase(it);
        delete widgetToDelete;

        // 更新商品分页
        updateGoodsPage();

        QMessageBox::information(this, "成功", "商品已成功删除！");
    }
    else {
        QMessageBox::warning(this, "警告", "未找到对应的商品编号！");
    }
}

void ManagerWindowClass::onDeleteSaleClicked()
{
    QString saleId = ui.saleDeleteEdit->text().trimmed(); // 获取输入的订单编号
    if (saleId.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入订单编号！");
        return;
    }

    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.saleTableView->model());
    if (!model) {
        QMessageBox::warning(this, "错误", "无法获取销售记录表格模型！");
        return;
    }

    // 遍历表格，找到对应的订单编号
    bool recordFound = false;
    for (int row = 0; row < model->rowCount(); ++row) {
        QModelIndex index = model->index(row, 0); // 假设订单编号在第0列
        if (model->data(index).toString() == saleId) {
            model->removeRow(row); // 删除该行
            recordFound = true;
            break;
        }
    }

    if (recordFound) {
        // 更新分页
        updateSalePage();
        QMessageBox::information(this, "成功", "销售记录已成功删除！");
    }
    else {
        QMessageBox::warning(this, "警告", "未找到对应的订单编号！");
    }
}

void ManagerWindowClass::onDeleteStaffClicked()
{
    QString staffId = ui.staffDeleteEdit->text().trimmed(); // 获取输入的员工编号
    if (staffId.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入员工编号！");
        return;
    }

    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.staffTableView->model());
    if (!model) {
        QMessageBox::warning(this, "错误", "无法获取员工表格模型！");
        return;
    }

    // 遍历表格，找到对应的员工编号
    bool recordFound = false;
    for (int row = 0; row < model->rowCount(); ++row) {
        QModelIndex index = model->index(row, 0); // 假设员工编号在第0列
        if (model->data(index).toString() == staffId) {
            model->removeRow(row); // 删除该行
            recordFound = true;
            break;
        }
    }

    if (recordFound) {
        // 更新分页
        updateStaffPage();
        QMessageBox::information(this, "成功", "员工已成功删除！");
    }
    else {
        QMessageBox::warning(this, "警告", "未找到对应的员工编号！");
    }
}
