#pragma once

#include <QLineEdit>
#include "ui_SearchEdit.h"

class SearchEdit : public QLineEdit
{
	Q_OBJECT

public:
	SearchEdit(QWidget *parent = nullptr);
	~SearchEdit();

private:
	Ui::SearchEdit ui;
	QAction* search_action;
};
