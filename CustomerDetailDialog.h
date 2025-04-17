#pragma once

#include <QDialog>
#include "ui_CustomerDetailDialog.h"

class CustomerDetailDialog : public QDialog
{
	Q_OBJECT

public:
	CustomerDetailDialog(QWidget* parent = nullptr);
	CustomerDetailDialog(QWidget* parent, const QString& id, const QString& birthDate, const QString& logDate,
		const QString& email, const QString& password, const QString& description,
		const QString& avatarPath); // 新增构造函数
	~CustomerDetailDialog();

signals:
	void dataAccepted(const QString& id, const QString& birthDate, const QString& logDate, const QString& email, const QString& password, const QString& description, const QString& avatarPath); // 新增头像路径参数

private slots:
	void onAcceptBtnClicked(); // 接受按钮槽函数
	void onCancelBtnClicked(); // 取消按钮槽函数
	void onCustomerPictureDoubleClicked(); // 双击头像槽函数

private:
	Ui::CustomerDetailDialog ui;
	QString avatarPath; // 保存头像路径
};
