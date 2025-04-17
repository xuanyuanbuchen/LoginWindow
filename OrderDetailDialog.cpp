#include "OrderDetailDialog.h"
#include "OrderDetailDialog.h"

OrderDetailDialog::OrderDetailDialog(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    setWindowTitle("添加订单");
}

OrderDetailDialog::~OrderDetailDialog() = default;

Ui::OrderDetailDialog* OrderDetailDialog::getUi()
{
    return &ui;
}
