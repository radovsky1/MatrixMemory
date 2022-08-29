#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QValueAxis>
#include <QListWidget>

QT_CHARTS_USE_NAMESPACE

class PlotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlotWidget(QWidget *parent = nullptr);
    void setData(const QVector<int> &newData);
    void makePlot();
    QChartView *chartView;
    QListWidget *listWidget;

signals:

private:
    QChart *chart;
    QVector<int> data;

};

#endif // PLOTWIDGET_H
