#pragma once

#include <QDialog>
#include "ui_CustomerDetailDialog.h"

class CustomerDetailDialog : public QDialog
{
    Q_OBJECT

public:
    CustomerDetailDialog(QWidget* parent = nullptr);
    ~CustomerDetailDialog();

signals:
    void dataAccepted(const QString& id, const QString& birthDate, const QString& logDate, const QString& email, const QString& password, const QString& description, const QString& avatarPath); // ����ͷ��·������

private slots:
    void onAcceptBtnClicked(); // ���ܰ�ť�ۺ���
    void onCancelBtnClicked(); // ȡ����ť�ۺ���
    void onCustomerPictureDoubleClicked(); // ˫��ͷ��ۺ���

private:
    Ui::CustomerDetailDialog ui;
    QString avatarPath; // ����ͷ��·��
};
