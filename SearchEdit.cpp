#include "SearchEdit.h"

SearchEdit::SearchEdit(QWidget *parent)
	: QLineEdit(parent)
{
	ui.setupUi(this);
	search_action = new QAction;
	search_action->setToolTip("点击搜索");
	search_action->setIcon(QIcon(":/res/Search_Icon.svg"));
	addAction(search_action, QLineEdit::TrailingPosition);
	setPlaceholderText("请输入内容");
}

SearchEdit::~SearchEdit()
{
}
