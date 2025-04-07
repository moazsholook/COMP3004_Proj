#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupGlucoseChart();
    
    // Connect the buttons
    connect(ui->rechargeButton, &QPushButton::clicked, this, &MainWindow::onRechargeClicked);
    connect(ui->refillButton, &QPushButton::clicked, this, &MainWindow::onRefillClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupGlucoseChart()
{
    // Create series for glucose data
    glucoseSeries = new QLineSeries();
    
    // Add some sample data points (you can replace this with real data)
    // X axis represents time in minutes, Y axis represents glucose level
    glucoseSeries->append(0, 5.5);
    glucoseSeries->append(30, 6.2);
    glucoseSeries->append(60, 7.0);
    glucoseSeries->append(90, 6.8);
    glucoseSeries->append(120, 6.5);
    glucoseSeries->append(150, 5.8);
    
    // Create the chart and add the series
    glucoseChart = new QChart();
    glucoseChart->addSeries(glucoseSeries);
    glucoseChart->setTitle("Glucose Levels");
    glucoseChart->legend()->hide();
    
    // Customize the chart appearance
    glucoseChart->setBackgroundBrush(QBrush(QColor("#333333")));
    glucoseChart->setTitleBrush(QBrush(Qt::white));
    
    // Create axes
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 180);
    axisX->setLabelFormat("%d");
    axisX->setTitleText("Time (minutes)");
    axisX->setLabelsColor(Qt::white);
    axisX->setTitleBrush(QBrush(Qt::white));
    
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(2, 22);  // Range from 2 to 22 mmol/L
    axisY->setLabelFormat("%.1f");
    axisY->setTitleText("Glucose (mmol/L)");
    axisY->setLabelsColor(Qt::white);
    axisY->setTitleBrush(QBrush(Qt::white));
    
    glucoseChart->addAxis(axisX, Qt::AlignBottom);
    glucoseChart->addAxis(axisY, Qt::AlignLeft);
    glucoseSeries->attachAxis(axisX);
    glucoseSeries->attachAxis(axisY);
    
    // Set up the chart view
    chartView = new QChartView(glucoseChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    
    // Set the chart view as the central widget of the cgmGraphFrame
    QLayout* layout = ui->cgmGraphFrame->layout();
    if (layout) {
        delete layout;
    }
    QHBoxLayout* newLayout = new QHBoxLayout(ui->cgmGraphFrame);
    newLayout->setContentsMargins(0, 0, 0, 0);
    newLayout->addWidget(chartView);
    ui->cgmGraphFrame->setLayout(newLayout);
}

void MainWindow::onRechargeClicked()
{
    // Set battery to 100%
    ui->batteryProgressBar->setValue(100);
    ui->batteryLabel->setText("100%");
    ui->batteryLabel->setStyleSheet("color: #55cc55; font-size: 10px; font-weight: bold;");
    ui->batteryProgressBar->setStyleSheet("QProgressBar { border: 1px solid #333333; border-radius: 0px; background-color: #333333; text-align: center; } QProgressBar::chunk { background-color: #55cc55; }");
}

void MainWindow::onRefillClicked()
{
    // Set insulin to 100%
    ui->insulinProgressBar->setValue(100);
    ui->insulinLabel->setText("300 u");
    ui->insulinLabel->setStyleSheet("color: #66aaff; font-size: 10px; font-weight: bold;");
    ui->insulinProgressBar->setStyleSheet("QProgressBar { border: 1px solid #333333; border-radius: 0px; background-color: #333333; text-align: center; } QProgressBar::chunk { background-color: #66aaff; }");
}

