#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Set the application-wide stylesheet for better text visibility
    this->setStyleSheet(R"(
        QMainWindow {
            background-color: #222222;
        }
        QLabel {
            color: #ffffff;
        }
        QPushButton {
            background-color: #444444;
            color: #ffffff;
            border: 1px solid #666666;
            padding: 5px;
            border-radius: 3px;
        }
        QPushButton:hover {
            background-color: #555555;
        }
        QPushButton:pressed {
            background-color: #666666;
        }
        QLineEdit {
            background-color: #333333;
            color: #ffffff;
            border: 1px solid #666666;
            padding: 5px;
            border-radius: 3px;
        }
        QDialog {
            background-color: #222222;
        }
    )");
    
    // Initialize battery
    battery = new Battery();
    updateBatteryDisplay();
    
    // Set up battery drain timer (drain 1% every 5 seconds)
    batteryTimer = new QTimer(this);
    connect(batteryTimer, &QTimer::timeout, this, &MainWindow::updateBatteryLevel);
    batteryTimer->start(5000); // 5 seconds
    
    setupGlucoseChart();
    
    // Connect the buttons
    connect(ui->optionsButton, &QPushButton::clicked, this, &MainWindow::onOptionsClicked);
    connect(ui->bolusButton, &QPushButton::clicked, this, &MainWindow::onBolusClicked);
    connect(ui->rechargeButton, &QPushButton::clicked, this, &MainWindow::onRechargeClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete battery;
}

void MainWindow::updateBatteryLevel()
{
    if (battery->getLevel() > 0) {
        battery->drain(1); // Drain 1% every 5 seconds
        updateBatteryDisplay();
        
        // Show different warning messages based on battery level
        if (battery->getLevel() == 50) {
            QMessageBox::warning(this, "Battery Warning", 
                "Battery is at 50%. Consider charging soon.");
        }
        else if (battery->getLevel() == 10) {
            QMessageBox::warning(this, "Low Battery Warning", 
                "Battery is critically low at 10%! Please charge the device immediately.");
        }
        else if (battery->getLevel() <= 5) {
            QMessageBox::critical(this, "Critical Battery Warning", 
                "Battery is extremely low! The pump will stop soon if not charged.");
        }
    } else {
        batteryTimer->stop();
        QMessageBox::critical(this, "Battery Dead", "The pump has stopped due to dead battery!");
    }
}

void MainWindow::updateBatteryDisplay()
{
    ui->batteryLabel->setText(QString("Battery: %1%").arg(battery->getLevel()));
    
    // Update battery color based on level
    QString color;
    if (battery->getLevel() > 50) {
        color = "#00ff00"; // Green
    } else if (battery->getLevel() > 10) {
        color = "#ffff00"; // Yellow
    } else {
        color = "#ff0000"; // Red
    }
    ui->batteryLabel->setStyleSheet(QString("color: %1; font-weight: bold;").arg(color));
}

void MainWindow::onRechargeClicked()
{
    if (battery->getLevel() < 100) {
        battery->charge();
        updateBatteryDisplay();
        QMessageBox::information(this, "Battery Charged", "Battery has been charged to 100%");
    } else {
        QMessageBox::information(this, "Battery Full", "Battery is already at 100%");
    }
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

void MainWindow::onOptionsClicked()
{
    OptionsDialog dialog(this);
    dialog.exec();
}

void MainWindow::onBolusClicked()
{
    ManualBolusDialog dialog(this);
    dialog.exec();
}

// Options Dialog Implementation
OptionsDialog::OptionsDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Options");
    setMinimumWidth(300);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(20, 20, 20, 20);
    
    stopInsulinButton = new QPushButton("Stop Insulin Delivery", this);
    profilesButton = new QPushButton("Manage Profiles", this);
    
    layout->addWidget(stopInsulinButton);
    layout->addWidget(profilesButton);
    
    connect(stopInsulinButton, &QPushButton::clicked, this, &OptionsDialog::onStopInsulinClicked);
    connect(profilesButton, &QPushButton::clicked, this, &OptionsDialog::onProfilesClicked);
}

void OptionsDialog::onStopInsulinClicked()
{
    QMessageBox::warning(this, "Emergency Stop", "Insulin delivery has been stopped!");
    // Here you would implement the actual stopping of insulin delivery
    close();
}

void OptionsDialog::onProfilesClicked()
{
    // This would open the profiles management window
    QMessageBox::information(this, "Profiles", "Profile management will be implemented here");
}

// Manual Bolus Dialog Implementation
ManualBolusDialog::ManualBolusDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Manual Bolus");
    setMinimumWidth(300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(10);
    
    bgInput = new QLineEdit(this);
    carbsInput = new QLineEdit(this);
    bolusResultLabel = new QLabel("Calculated Bolus: 0.0 units", this);
    calculateButton = new QPushButton("Calculate", this);
    confirmButton = new QPushButton("Confirm", this);
    
    formLayout->addRow("Blood Glucose (mmol/L):", bgInput);
    formLayout->addRow("Carbohydrates (g):", carbsInput);
    
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(bolusResultLabel);
    mainLayout->addWidget(calculateButton);
    mainLayout->addWidget(confirmButton);
    
    connect(calculateButton, &QPushButton::clicked, this, &ManualBolusDialog::onCalculateClicked);
    connect(confirmButton, &QPushButton::clicked, this, &ManualBolusDialog::onConfirmClicked);
}

void ManualBolusDialog::onCalculateClicked()
{
    bool bgOk, carbsOk;
    float bg = bgInput->text().toFloat(&bgOk);
    float carbs = carbsInput->text().toFloat(&carbsOk);

    if (!bgOk || !carbsOk) {
        QMessageBox::warning(this, "Input Error", "Please enter valid numbers for BG and carbs");
        return;
    }

    // Simple bolus calculation (you would implement your actual calculation logic here)
    float carbBolus = carbs / 10.0; // Example: 1 unit per 10g carbs
    float correctionBolus = (bg > 7.0) ? (bg - 7.0) / 2.0 : 0; // Example: correct if BG > 7.0
    calculatedBolus = carbBolus + correctionBolus;

    bolusResultLabel->setText(QString("Calculated Bolus: %1 units").arg(calculatedBolus));
    confirmButton->setEnabled(true);
}

void ManualBolusDialog::onConfirmClicked()
{
    QMessageBox::information(this, "Bolus Confirmed", 
        QString("Delivering %1 units of insulin").arg(calculatedBolus));
    close();
}

