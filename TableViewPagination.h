#pragma once
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <SearchEdit.h>
class TableViewPagination 
{
public:

    // ���캯��
    TableViewPagination(QAbstractItemModel* model, int pageSize = 15);
    //��ȡҳ����
    int pageCount() const;
    // ���õ�ǰҳ(�Զ�����߽�)
    void setPage(int page);
    // ��ȡ��ǰҳ
    int getCurrentPage() const;
    // Ӧ�õ������ͼ
    void applyToTableView(QTableView* tableView);
    // ��ȡ��ǰҳ����ʼ�к�
    int startRow() const;
    // ��ȡ��ǰҳ�Ľ����к�(������)
    int endRow() const;

private:
    QAbstractItemModel* model;
    int pageSize;
    int currentPage;

};