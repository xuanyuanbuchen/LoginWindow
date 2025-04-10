#include "FinanceChart.h"
#include <QtCharts/QChart>
#include <QRandomGenerator>

FinanceChart::FinanceChart(QWidget* parent)
    : QChartView(parent)
{
    incomeSeries = new QLineSeries();
    incomeSeries->setName("Income");
    incomeSeries->setColor(Qt::green);

    expenseSeries = new QLineSeries();
    expenseSeries->setName("Expense");
    expenseSeries->setColor(Qt::red);

    initChart();
    createDefaultData();
    updateAxesRange();
}

void FinanceChart::initChart()
{
    QChart* chart = new QChart();
    chart->addSeries(incomeSeries);
    chart->addSeries(expenseSeries);
    chart->setTitle("Income & Expense Chart");

    // ��ʼ��������
    axisX = new QDateTimeAxis();
    axisX->setTitleText("Date");
    axisX->setFormat("yyyy-MM-dd");
    chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis();
    axisY->setTitleText("Amount");
    chart->addAxis(axisY, Qt::AlignLeft);

    incomeSeries->attachAxis(axisX);
    incomeSeries->attachAxis(axisY);
    expenseSeries->attachAxis(axisX);
    expenseSeries->attachAxis(axisY);

    setChart(chart);

    setRenderHints(QPainter::Antialiasing |
        QPainter::SmoothPixmapTransform |
        QPainter::TextAntialiasing);

    // �� FinanceChart ���캯�������
    incomeSeries->setPointsVisible(true);
    expenseSeries->setPointsVisible(true);

    // �Զ�����ʾ��ʽ
    incomeSeries->setPointLabelsFormat("����: @yPoint Ԫ");
    expenseSeries->setPointLabelsFormat("֧��: @yPoint Ԫ");

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
}

void FinanceChart::createDefaultData()
{
    QDateTime baseDate = QDateTime::currentDateTime().addDays(-7);

    // ����7���Ĭ������
    for (int i = 0; i < 7; ++i) {
        QDateTime date = baseDate.addDays(i);
        incomeData.append({ date, QRandomGenerator::global()->bounded(5000, 10000) });
        expenseData.append({ date, QRandomGenerator::global()->bounded(3000, 8000) });
    }

    // ����Ĭ������
    setIncomeData(incomeData);
    setExpenseData(expenseData);
}

void FinanceChart::setIncomeData(const QVector<QPair<QDateTime, double>>& data)
{
    incomeData = data;
    incomeSeries->clear();

    for (const auto& point : data) {
        incomeSeries->append(point.first.toMSecsSinceEpoch(), point.second);
    }
    updateAxesRange();
}

void FinanceChart::setExpenseData(const QVector<QPair<QDateTime, double>>& data)
{
    expenseData = data;
    expenseSeries->clear();

    for (const auto& point : data) {
        expenseSeries->append(point.first.toMSecsSinceEpoch(), point.second);
    }
    updateAxesRange();
}

void FinanceChart::updateAxesRange()
{
    if (incomeData.isEmpty() && expenseData.isEmpty()) return;

    QDateTime minDate = QDateTime::currentDateTime();
    QDateTime maxDate = QDateTime::currentDateTime().addDays(-365);
    double minValue = 0;
    double maxValue = 0;

    auto updateRange = [&](const auto& data) {
        for (const auto& point : data) {
            if (point.first < minDate) minDate = point.first;
            if (point.first > maxDate) maxDate = point.first;
            if (point.second < minValue) minValue = point.second;
            if (point.second > maxValue) maxValue = point.second;
        }
        };

    updateRange(incomeData);
    updateRange(expenseData);

    axisX->setRange(minDate, maxDate.addDays(1));
    axisY->setRange(minValue * 0.9, maxValue * 1.1);
}