#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
<<<<<<< HEAD
#include <QLineSeries>
#include <QChart>
#include <QChartView>
=======
>>>>>>> eb6053f3cc6bf5eb42807707cf41208950aed492

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
