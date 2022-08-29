#include "plotwidget.h"
#include <QDebug>

PlotWidget::PlotWidget(QWidget *parent)
    : QWidget(parent)
{
}

void PlotWidget::setData(const QVector<int> &newData) {
    data = newData;
}

void PlotWidget::makePlot() {
    QLineSeries *series = new QLineSeries();
    for (int i = 0; i < (int)data.size(); i++) {
        series->append(QPointF(i, data[i]));
        qDebug() << data[i] << '\n';
    }
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%d");
    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Passed levels dynamic");
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->hide();
    chartView = new QChartView(chart);
    chartView->chart()->setAxisY(axisY, series);
    chartView->setRenderHint(QPainter::Antialiasing);
}
