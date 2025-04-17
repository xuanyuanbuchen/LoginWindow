#ifndef FINANCECHART_H
#define FINANCECHART_H

#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QDateTime>

QT_BEGIN_NAMESPACE
class QChart;
class QLineSeries;
class QDateTimeAxis;
class QValueAxis;
QT_END_NAMESPACE

class FinanceChart : public QChartView
{
    Q_OBJECT
public:
    explicit FinanceChart(QWidget* parent = nullptr);

    // 数据更新接口
    void setIncomeData(const QVector<QPair<QDateTime, double>>& data);
    void setExpenseData(const QVector<QPair<QDateTime, double>>& data);

private:
    void initChart();
    void createDefaultData();
    void updateAxesRange();

    QLineSeries* incomeSeries;
    QLineSeries* expenseSeries;
    QDateTimeAxis* axisX;
    QValueAxis* axisY;
    QVector<QPair<QDateTime, double>> incomeData;
    QVector<QPair<QDateTime, double>> expenseData;
};

#endif // FINANCECHART_H