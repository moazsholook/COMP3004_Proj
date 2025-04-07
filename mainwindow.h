#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QLineSeries>
#include <QChart>
#include <QChartView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QChart *glucoseChart;
    QLineSeries *glucoseSeries;
    QChartView *chartView;
    
    void setupGlucoseChart();

private slots:
    void onRechargeClicked();
    void onRefillClicked();
};
#endif // MAINWINDOW_H
