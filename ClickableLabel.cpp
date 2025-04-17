#include "ClickableLabel.h"

ClickableLabel::ClickableLabel(QWidget* parent)
    : QLabel(parent) 
{

}

ClickableLabel::~ClickableLabel() 
{

}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
}

void ClickableLabel::mouseDoubleClickEvent(QMouseEvent* event)
{
    emit doubleClicked(); // 发射双击信号
}