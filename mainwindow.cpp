#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupGlucoseChart();
    
    // Connect the buttons
    connect(ui->optionsButton, &QPushButton::clicked, this, &MainWindow::onOptionsClicked);
    connect(ui->bolusButton, &QPushButton::clicked, this, &MainWindow::onBolusClicked);
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
    setStyleSheet("background-color: #222222;");
    setMinimumWidth(300);

    QVBoxLayout *layout = new QVBoxLayout(this);
    
    stopInsulinButton = new QPushButton("STOP INSULIN", this);
    stopInsulinButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #cc3333;"
        "  color: white;"
        "  border: none;"
        "  padding: 10px;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #aa2222;"
        "}"
    );
    
    profilesButton = new QPushButton("PERSONAL PROFILES", this);
    profilesButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #444444;"
        "  color: white;"
        "  border: none;"
        "  padding: 10px;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #333333;"
        "}"
    );

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
    setStyleSheet("background-color: #222222; color: white;");
    setMinimumWidth(300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    bgInput = new QLineEdit(this);
    bgInput->setStyleSheet(
        "QLineEdit {"
        "  background-color: #333333;"
        "  color: white;"
        "  border: 1px solid #444444;"
        "  padding: 5px;"
        "}"
    );
    
    carbsInput = new QLineEdit(this);
    carbsInput->setStyleSheet(bgInput->styleSheet());

    formLayout->addRow("Blood Glucose (mmol/L):", bgInput);
    formLayout->addRow("Carbs (g):", carbsInput);

    calculateButton = new QPushButton("Calculate", this);
    calculateButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #444444;"
        "  color: white;"
        "  border: none;"
        "  padding: 10px;"
        "  font-size: 14px;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #333333;"
        "}"
    );

    confirmButton = new QPushButton("Confirm Bolus", this);
    confirmButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #66aaff;"
        "  color: white;"
        "  border: none;"
        "  padding: 10px;"
        "  font-size: 14px;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #5599ee;"
        "}"
    );
    confirmButton->setEnabled(false);

    bolusResultLabel = new QLabel(this);
    bolusResultLabel->setStyleSheet("color: white; font-size: 16px;");
    bolusResultLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(calculateButton);
    mainLayout->addWidget(bolusResultLabel);
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

