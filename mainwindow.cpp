#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , profiles()
    , activeProfile()
    , insulinLevel(300.0)
    , batteryLevel(100)
    , insulinDeliveryStopped(false)  // Initialize to false
    , batteryTimer(new QTimer(this))
    , glucoseTimer(new QTimer(this))
    , extendedBolusTimer(new QTimer(this))
    , cgmSimulationTimer(new QTimer(this))
    , glucoseChart(new QChart())
    , glucoseSeries(new QLineSeries())
    , glucoseAxisX(new QValueAxis())
    , glucoseAxisY(new QValueAxis())
    , profileLabel(new QLabel("No Active Profile", this))
    , battery(new Battery())
    , timeLabel(new QLabel(this))
    , dateLabel(new QLabel(this))
    , clockTimer(new QTimer(this))
    , remainingExtendedBolus(0.0)
    , totalExtendedBolus(0.0)
    , totalDurationMs(0)
    , deliveryIntervalMs(20000)  // Default 20-second intervals
    , intervalsRemaining(0)
    , lastGlucoseTime()
    , poweredOn(false)  // Initialize to false
    , isSleeping(false)  // Initialize to false
    , cgmModeEnabled(false)  // Initialize CGM mode to false
    , currentCgmValueIndex(0)
    , eventLogger(new EventLogger())
{
    // Initialize CGM simulation values
    cgmSimulationValues = {110, 115, 120, 130, 125, 118, 105, 100};
    
    // Set up CGM simulation timer
    connect(cgmSimulationTimer, &QTimer::timeout, this, &MainWindow::onCgmSimulationTimeout);
    
    ui->setupUi(this);
    
    // Create CGM mode toggle switch
    QCheckBox *cgmModeToggle = new QCheckBox("Enable CGM Mode", this);
    cgmModeToggle->setStyleSheet("color: white; font-size: 12px;");
    cgmModeToggle->setChecked(cgmModeEnabled);
    connect(cgmModeToggle, &QCheckBox::toggled, this, &MainWindow::onCgmModeToggled);
    
    // Add the toggle to the glucose frame layout
    ui->glucoseFrameLayout->addWidget(cgmModeToggle);
    
    // Load profiles from JSON file
    loadProfiles();
    
    // Setup UI elements
    setupGlucoseChart();
    
    // Create battery timer (but don't start it yet)
    connect(batteryTimer, &QTimer::timeout, this, &MainWindow::updateBatteryLevel);
    
    // Create glucose timer
    connect(glucoseTimer, &QTimer::timeout, this, &MainWindow::updateGlucoseLevel);
    glucoseTimer->start(300000); // 5 minutes
    
    // Setup extended bolus timer
    connect(extendedBolusTimer, &QTimer::timeout, this, &MainWindow::updateExtendedBolus);
    
    // Add profile label to status bar
    statusBar()->addPermanentWidget(profileLabel);
    
    // Add time and date labels to status bar
    timeLabel->setStyleSheet("color: white; font-size: 14px; padding-right: 10px;");
    dateLabel->setStyleSheet("color: white; font-size: 14px; padding-right: 10px;");
    statusBar()->addWidget(timeLabel);
    statusBar()->addWidget(dateLabel);
    
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
    
    // Initialize battery display
    updateBatteryDisplay();
    
    // Initialize insulin display
    updateInsulinDisplay();
    
    // Connect the buttons
    connect(ui->optionsButton, &QPushButton::clicked, this, &MainWindow::onOptionsClicked);
    connect(ui->bolusButton, &QPushButton::clicked, this, &MainWindow::onBolusClicked);
    connect(ui->rechargeButton, &QPushButton::clicked, this, &MainWindow::onRechargeClicked);
    connect(ui->refillButton, &QPushButton::clicked, this, &MainWindow::onRefillClicked);
    connect(ui->powerButton, &QPushButton::clicked, this, &MainWindow::onPowerButtonClicked);
    
    // Set up clock timer and initial time/date
    connect(clockTimer, &QTimer::timeout, this, &MainWindow::updateDateTime);
    clockTimer->start(1000); // Update every second
    updateDateTime(); // Initial update
    
    // Connect time and date labels to UI elements
    ui->timeLabel->setText(timeLabel->text());
    ui->dateLabel->setText(dateLabel->text());
    connect(clockTimer, &QTimer::timeout, this, [this]() {
        ui->timeLabel->setText(timeLabel->text());
        ui->dateLabel->setText(dateLabel->text());
    });
    
    // Initially disable buttons until power on
    ui->optionsButton->setEnabled(false);
    ui->bolusButton->setEnabled(false);
    ui->rechargeButton->setEnabled(false);
    ui->refillButton->setEnabled(false);
    ui->powerButton->setEnabled(true);
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
        
        // Log battery alerts at specific levels
        int currentLevel = battery->getLevel();
        if (currentLevel == 50 || currentLevel == 20 || currentLevel == 10) {
            eventLogger->logBatteryAlert(currentLevel);
        }
    } else {
        batteryTimer->stop();
        
        // Disable all functionality except recharge
        ui->optionsButton->setEnabled(false);
        ui->bolusButton->setEnabled(false);
        ui->refillButton->setEnabled(false);
        ui->powerButton->setEnabled(false);
        ui->rechargeButton->setEnabled(true);
        
        QMessageBox::critical(this, "Battery Dead", 
            "The pump has stopped due to dead battery! Please recharge to continue.");
    }
}

void MainWindow::updateBatteryDisplay()
{
    // Update progress bar
    ui->batteryProgressBar->setValue(battery->getLevel());
    
    // Update label
    ui->batteryLabel->setText(QString("%1%").arg(battery->getLevel()));
    
    // Update color based on level
    QString color;
    if (battery->getLevel() > 50) {
        color = "#55cc55"; // Green
    } else if (battery->getLevel() > 20) {
        color = "#ffaa00"; // Orange
    } else {
        color = "#ff4444"; // Red
    }
    
    // Update styles
    ui->batteryLabel->setStyleSheet(QString("color: %1; font-size: 10px; font-weight: bold;").arg(color));
    ui->batteryProgressBar->setStyleSheet(QString(
        "QProgressBar {"
        "    border: 1px solid #333333;"
        "    border-radius: 0px;"
        "    background-color: #333333;"
        "    text-align: center;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: %1;"
        "}").arg(color));
}

/**
 * This function handles the recharge button click event. It checks if the battery level is below
 * 100% and if so, charges it to full capacity. The battery display is updated to reflect the
 * new charge level, and the battery timer is restarted if it was stopped.
 * 
 * If the battery is already at 100%, it shows a message indicating that no charging is needed.
 */
void MainWindow::onRechargeClicked()
{
    if (battery->getLevel() < 100) {
        battery->charge();
        updateBatteryDisplay();
        
        // If battery was at 0%, re-enable all functionality
        if (battery->getLevel() == 100) {
            ui->optionsButton->setEnabled(true);
            ui->bolusButton->setEnabled(true);
            ui->refillButton->setEnabled(true);
            ui->powerButton->setEnabled(true);
            
            // Only restart the battery timer if the pump is powered on
            if (poweredOn) {
                batteryTimer->start(5000); // 5 seconds
            }
        }
        
        QMessageBox::information(this, "Battery Charged", "Battery has been charged to 100%");
    } else {
        QMessageBox::information(this, "Battery Full", "Battery is already at 100%");
    }
}

/**
 * This function sets up the glucose chart visualization. It creates a line series to display
 * glucose data points over time, configures the chart appearance with a dark theme, and sets up
 * the axes with appropriate ranges and labels. The chart is then added to the CGM graph frame
 * in the main window.
 * 
 * The chart uses a QLineSeries to plot glucose values, with time on the x-axis and glucose
 * concentration in mmol/L on the y-axis. The axes are customized with white labels and titles
 * for better visibility against the dark background.
 */
void MainWindow::setupGlucoseChart()
{
    // Create series for glucose data
    glucoseSeries = new QLineSeries();
    
    // Create the chart and add the series
    glucoseChart = new QChart();
    glucoseChart->addSeries(glucoseSeries);
    glucoseChart->setTitle("Glucose Levels");
    glucoseChart->legend()->hide();
    
    // Customize the chart appearance
    glucoseChart->setBackgroundBrush(QBrush(QColor("#333333")));
    glucoseChart->setTitleBrush(QBrush(Qt::white));
    
    // Set up X axis (time) for 5-minute demo
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 5);  // 5 minutes range
    axisX->setLabelFormat("%d");
    axisX->setTitleText("Time (minutes)");
    axisX->setLabelsColor(Qt::white);
    axisX->setTitleBrush(QBrush(Qt::white));
    axisX->setTickCount(6);  // Show ticks at 0,1,2,3,4,5 minutes
    
    // Set up Y axis (glucose)
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

/**
 * This function handles the options button click event. It creates and shows the options dialog,
 * which allows the user to access various pump settings and features. The dialog is modal,
 * meaning it blocks interaction with the main window until closed.
 * 
 * When the options dialog is closed, it triggers an update of the profiles to ensure any changes
 * made in the options menu are reflected in the main window.
 */
void MainWindow::onOptionsClicked()
{
    OptionsDialog dialog(this);
    connect(&dialog, &OptionsDialog::finished, this, &MainWindow::updateProfiles);
    dialog.exec();
}

void MainWindow::onBolusClicked()
{
    // Check if a profile is selected
    if (activeProfile.isEmpty() || !profiles.contains(activeProfile)) {
        QMessageBox::warning(this, "No Profile Selected",
            "Please select a profile from Options > Manage Profiles before administering a bolus.");
        return;
    }

    // Get the selected profile
    Profile* profile = profiles[activeProfile];
    
    ManualBolusDialog *bolusDialog = new ManualBolusDialog(this, profile);
    
    // Connect both signals
    connect(bolusDialog, &ManualBolusDialog::bolusConfirmed, this, [this](float immediateAmount, float extendedAmount, int duration) {
        float totalAmount = immediateAmount + extendedAmount;
        if (totalAmount > insulinLevel) {
            QMessageBox::critical(this, "Insufficient Insulin",
                "Not enough insulin in cartridge for this bolus!");
            return;
        }
        
        // Deduct insulin and update display
        insulinLevel -= totalAmount;
        updateInsulinDisplay();
        
        // Update IOB label
        QString currentIOB = ui->iobLabel->text();
        float iobValue = currentIOB.split(" ")[3].toFloat();
        float newIOB = iobValue + totalAmount;
        ui->iobLabel->setText(QString("INSULIN ON BOARD    %1 u | 3:45 hrs").arg(newIOB));
        
        // Log the bolus delivery
        eventLogger->logImmediateBolus(immediateAmount);
        if (extendedAmount > 0) {
            eventLogger->logExtendedBolus(extendedAmount);
        }
        
        // If extended bolus is selected, start the timer
        if (extendedAmount > 0) {
            totalExtendedBolus = extendedAmount;
            remainingExtendedBolus = extendedAmount;
            totalDurationMs = duration * 3600000; // Convert hours to milliseconds
            intervalsRemaining = totalDurationMs / deliveryIntervalMs;
            extendedBolusTimer->start(deliveryIntervalMs);
            
            // Show extended bolus details
            QMessageBox::information(this, "Extended Bolus Started", 
                QString("Extended bolus delivery started:\n"
                       "Total Amount: %1 units\n"
                       "Duration: %2 hours\n"
                       "Delivery Interval: %3 seconds\n"
                       "Amount per Interval: %4 units\n"
                       "Total Intervals: %5")
                .arg(extendedAmount)
                .arg(duration)
                .arg(deliveryIntervalMs / 1000)
                .arg(extendedAmount / (duration * 3600.0 / deliveryIntervalMs))
                .arg(intervalsRemaining));
        } else {
            QMessageBox::information(this, "Bolus Delivered", 
                QString("Immediate bolus of %1 units delivered successfully.\n"
                       "Remaining insulin: %2 units")
                .arg(immediateAmount)
                .arg(insulinLevel));
        }
    });
    
    // Connect the new glucose reading signal
    connect(bolusDialog, &ManualBolusDialog::newGlucoseReading, this, &MainWindow::addGlucoseReading);
    
    bolusDialog->exec();
    delete bolusDialog;
}

/**
 * This function handles the refill button click event. It checks if the insulin level is below
 * the maximum capacity (300 units) and if so, refills it to full capacity. The insulin display
 * is updated to reflect the new level.
 * 
 * If the insulin cartridge is already full, it shows a message indicating that no refill is needed.
 */
void MainWindow::onRefillClicked()
{
    if (insulinLevel < 300.0) {
        insulinLevel = 300.0;  // Reset to full
        updateInsulinDisplay();
        QMessageBox::information(this, "Cartridge Refilled", 
            "Insulin cartridge has been refilled to 300 units.");
    } else {
        QMessageBox::information(this, "Cartridge Full", 
            "Insulin cartridge is already full (300 units).");
    }
}

void MainWindow::updateInsulinDisplay()
{
    // Update progress bar
    int percentage = (insulinLevel / 300.0) * 100;
    ui->insulinProgressBar->setValue(percentage);
    
    // Update label
    ui->insulinLabel->setText(QString("%1 u").arg(insulinLevel, 0, 'f', 1));
    
    // Update color based on level
    QString color;
    if (insulinLevel > 150) {  // >50%
        color = "#66aaff"; // Blue
    } else if (insulinLevel > 60) {  // >20%
        color = "#ffaa00"; // Orange
    } else {
        color = "#ff4444"; // Red
    }
    
    // Update styles
    ui->insulinLabel->setStyleSheet(QString("color: %1; font-size: 10px; font-weight: bold;").arg(color));
    ui->insulinProgressBar->setStyleSheet(QString(
        "QProgressBar {"
        "    border: 1px solid #333333;"
        "    border-radius: 0px;"
        "    background-color: #333333;"
        "    text-align: center;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: %1;"
        "}").arg(color));
    
    // Log cartridge alerts
    if (insulinLevel <= 60 && insulinLevel > 30) {
        eventLogger->logCartridgeAlert(insulinLevel);
    } else if (insulinLevel <= 30) {
        eventLogger->logCartridgeAlert(insulinLevel);
    }
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
    sleepButton = new QPushButton("Sleep Mode", this);  // ✅ Add this

    layout->addWidget(stopInsulinButton);
    layout->addWidget(profilesButton);
    layout->addWidget(sleepButton);  // ✅ And this

    connect(stopInsulinButton, &QPushButton::clicked, this, &OptionsDialog::onStopInsulinClicked);
    connect(profilesButton, &QPushButton::clicked, this, &OptionsDialog::onProfilesClicked);
    connect(sleepButton, &QPushButton::clicked, this, &OptionsDialog::onSleepClicked);  // ✅ Connect it
}

void OptionsDialog::onStopInsulinClicked()
{
    MainWindow* mainWindow = qobject_cast<MainWindow*>(parent());
    if (mainWindow) {
        mainWindow->setInsulinDeliveryStopped(true);
        QMessageBox::warning(this, "Emergency Stop", "Insulin delivery has been stopped!");
    }
    close();
}

void OptionsDialog::onProfilesClicked()
{
    ProfilesDialog dialog(this);
    connect(&dialog, &ProfilesDialog::profileSelected, this, [this](const QString& profile) {
        MainWindow* mainWindow = qobject_cast<MainWindow*>(parent());
        if (mainWindow) {
            mainWindow->setActiveProfile(profile);
            QMessageBox::information(this, "Profile Selected", 
                QString("Profile '%1' has been selected and saved.").arg(profile));
        }
    });
    dialog.exec();
}

void OptionsDialog::onSleepClicked()
{
    MainWindow* mainWindow = qobject_cast<MainWindow*>(parent());
    if (mainWindow) {
        mainWindow->enterSleepMode();  // ✅ Call the real sleep function
    }
    close();
}

// Manual Bolus Dialog Implementation
ManualBolusDialog::ManualBolusDialog(QWidget *parent, Profile* profile)
    : QDialog(parent)
    , activeProfile(profile)
    , calculatedBolus(0.0)
    , extendedBolus(0.0)
    , duration(3)  // Default 3 hours
    , immediatePercentage(60)  // Default 60%
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
    extendedBolusCheck = new QCheckBox("Use Extended Bolus", this);
    extendedBolusCheck->setStyleSheet("color: white;");
    
    // Add immediate percentage input
    immediatePercentageInput = new QSpinBox(this);
    immediatePercentageInput->setRange(0, 100);
    immediatePercentageInput->setValue(60);
    immediatePercentageInput->setSuffix("%");
    
    durationInput = new QSpinBox(this);
    durationInput->setRange(1, 8);  // 1-8 hours
    durationInput->setValue(3);
    
    bolusResultLabel = new QLabel("Calculated Bolus: 0.0 units", this);
    extendedBolusLabel = new QLabel("Extended Bolus: 0.0 units", this);
    bolusPerHourLabel = new QLabel("Bolus Rate: 0.0 units/hour", this);
    calculateButton = new QPushButton("Calculate", this);
    confirmButton = new QPushButton("Confirm", this);
    
    formLayout->addRow("Blood Glucose (mmol/L):", bgInput);
    formLayout->addRow("Carbohydrates (g):", carbsInput);
    formLayout->addRow("", extendedBolusCheck);
    formLayout->addRow("Immediate Percentage:", immediatePercentageInput);
    formLayout->addRow("Duration (hours):", durationInput);
    
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(bolusResultLabel);
    mainLayout->addWidget(extendedBolusLabel);
    mainLayout->addWidget(bolusPerHourLabel);
    mainLayout->addWidget(calculateButton);
    mainLayout->addWidget(confirmButton);
    
    connect(calculateButton, &QPushButton::clicked, this, &ManualBolusDialog::onCalculateClicked);
    connect(confirmButton, &QPushButton::clicked, this, &ManualBolusDialog::onConfirmClicked);
    connect(extendedBolusCheck, &QCheckBox::toggled, this, &ManualBolusDialog::onExtendedBolusToggled);
    connect(durationInput, QOverload<int>::of(&QSpinBox::valueChanged), this, &ManualBolusDialog::onDurationChanged);
    connect(immediatePercentageInput, QOverload<int>::of(&QSpinBox::valueChanged), this, &ManualBolusDialog::onImmediatePercentageChanged);
    
    // Initially disable confirm button until calculation is done
    confirmButton->setEnabled(false);
    durationInput->setEnabled(false);
    immediatePercentageInput->setEnabled(false);
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

    // Emit the new glucose reading
    emit newGlucoseReading(bg);

    // Calculate bolus using profile settings
    float carbBolus = carbs / activeProfile->getICR(); // Use profile's insulin-to-carb ratio
    float correctionBolus = 0.0;
    
    // Calculate correction if BG is above target
    if (bg > activeProfile->getTargetBG()) {
        correctionBolus = (bg - activeProfile->getTargetBG()) / activeProfile->getCorrectionFactor();
    }
    
    calculatedBolus = carbBolus + correctionBolus;

    // Update display with detailed calculation
    bolusResultLabel->setText(QString("Calculated Bolus: %1 units\n"
                                     "Carb Bolus: %2 units\n"
                                     "Correction Bolus: %3 units")
                             .arg(calculatedBolus, 0, 'f', 1)
                             .arg(carbBolus, 0, 'f', 1)
                             .arg(correctionBolus, 0, 'f', 1));
    
    // If extended bolus is enabled, calculate the split
    if (extendedBolusCheck->isChecked()) {
        float immediateAmount = calculatedBolus * (immediatePercentage / 100.0);
        extendedBolus = calculatedBolus - immediateAmount;
        calculatedBolus = immediateAmount;
        
        extendedBolusLabel->setText(QString("Extended Bolus: %1 units over %2 hours")
                                  .arg(extendedBolus, 0, 'f', 1)
                                  .arg(duration));
        bolusPerHourLabel->setText(QString("Bolus Rate: %1 units/hour")
                                 .arg(extendedBolus / duration, 0, 'f', 1));
    }
    
    confirmButton->setEnabled(true);
}

void ManualBolusDialog::onConfirmClicked()
{
    QString message = QString("Are you sure you want to deliver:\n"
                            "Immediate Bolus: %1 units\n").arg(calculatedBolus);
    
    if (extendedBolusCheck->isChecked()) {
        message += QString("Extended Bolus: %1 units over %2 hours\n"
                         "Bolus Rate: %3 units/hour")
                         .arg(extendedBolus)
                         .arg(duration)
                         .arg(extendedBolus / duration);
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Bolus",
        message, QMessageBox::Yes | QMessageBox::No);
        
    if (reply == QMessageBox::Yes) {
        emit bolusConfirmed(calculatedBolus, extendedBolus, duration);
        accept();
    }
}

void ManualBolusDialog::onExtendedBolusToggled(bool checked)
{
    durationInput->setEnabled(checked);
    immediatePercentageInput->setEnabled(checked);
    updateExtendedBolusDisplay();
}

void ManualBolusDialog::onDurationChanged(int value)
{
    duration = value;
    updateExtendedBolusDisplay();
}

void ManualBolusDialog::onImmediatePercentageChanged(int value)
{
    immediatePercentage = value;
    updateExtendedBolusDisplay();
}

void ManualBolusDialog::updateExtendedBolusDisplay()
{
    bolusResultLabel->setText(QString("Immediate Bolus: %1 units").arg(calculatedBolus));
    
    if (extendedBolusCheck->isChecked()) {
        extendedBolusLabel->setText(QString("Extended Bolus: %1 units over %2 hours")
                                  .arg(extendedBolus)
                                  .arg(duration));
        bolusPerHourLabel->setText(QString("Bolus Rate: %1 units/hour")
                                 .arg(extendedBolus / duration));
        extendedBolusLabel->setVisible(true);
        bolusPerHourLabel->setVisible(true);
    } else {
        extendedBolusLabel->setVisible(false);
        bolusPerHourLabel->setVisible(false);
    }
}

ProfilesDialog::ProfilesDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Profile Management");
    setMinimumWidth(400);
    setStyleSheet("background-color: #333333; color: white;");

    setupUI();
    loadProfiles();  
    refreshProfilesList();
}

void ProfilesDialog::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    
    // Add Profile button at the top
    addProfileButton = new QPushButton("ADD NEW PROFILE", this);
    addProfileButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #444444;"
        "  color: #F0B000;"
        "  border: none;"
        "  padding: 10px;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #333333;"
        "}"
    );
    connect(addProfileButton, &QPushButton::clicked, this, &ProfilesDialog::onAddProfileClicked);
    
    mainLayout->addWidget(addProfileButton);
    
    // Container for profile buttons
    QWidget *profilesContainer = new QWidget(this);
    profilesLayout = new QVBoxLayout(profilesContainer);
    mainLayout->addWidget(profilesContainer);
    
    // Action buttons container
    QWidget *actionButtonsContainer = new QWidget(this);
    QHBoxLayout *actionButtonsLayout = new QHBoxLayout(actionButtonsContainer);
    actionButtonsLayout->setContentsMargins(0, 10, 0, 0);
    
    // Create action buttons
    viewButton = new QPushButton("View Profile", this);
    editButton = new QPushButton("Edit Profile", this);
    deleteButton = new QPushButton("Delete Profile", this);
    selectButton = new QPushButton("Select Profile", this);
    
    // Style action buttons
    QString actionButtonStyle = 
        "QPushButton {"
        "  background-color: #444444;"
        "  color: white;"
        "  border: none;"
        "  padding: 8px 15px;"
        "  font-size: 14px;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #333333;"
        "}";
    
    viewButton->setStyleSheet(actionButtonStyle.replace("color: white;", "color: #55cc55;"));
    editButton->setStyleSheet(actionButtonStyle.replace("color: white;", "color: #F0B000;"));
    deleteButton->setStyleSheet(actionButtonStyle.replace("color: white;", "color: #ff4444;"));
    selectButton->setStyleSheet(actionButtonStyle.replace("color: white;", "color: #66aaff;"));
    
    // Add action buttons to layout
    actionButtonsLayout->addWidget(viewButton);
    actionButtonsLayout->addWidget(editButton);
    actionButtonsLayout->addWidget(deleteButton);
    actionButtonsLayout->addWidget(selectButton);
    
    // Add action buttons container to main layout
    mainLayout->addWidget(actionButtonsContainer);
    
    // Connect action buttons
    connect(viewButton, &QPushButton::clicked, this, &ProfilesDialog::onViewProfileClicked);
    connect(editButton, &QPushButton::clicked, this, &ProfilesDialog::onEditProfileClicked);
    connect(deleteButton, &QPushButton::clicked, this, &ProfilesDialog::onDeleteProfileClicked);
    connect(selectButton, &QPushButton::clicked, this, &ProfilesDialog::onSelectProfileClicked);
    
    // Initially disable action buttons
    updateActionButtons();
}

void ProfilesDialog::refreshProfilesList()
{
    // Clear existing profile buttons
    for (auto button : profileButtons) {
        profilesLayout->removeWidget(button);
        delete button;
    }
    profileButtons.clear();

    // Create buttons for each profile
    for (auto it = profiles.begin(); it != profiles.end(); ++it) {
        const QString& name = it.key();
        QPushButton* nameButton = new QPushButton(name, this);
        nameButton->setStyleSheet(
            "QPushButton {"
            "  background-color: #444444;"
            "  color: white;"
            "  border: none;"
            "  padding: 10px;"
            "  font-size: 14px;"
            "  text-align: left;"
            "}"
            "QPushButton:pressed {"
            "  background-color: #333333;"
            "}"
            "QPushButton:checked {"
            "  background-color: #555555;"
            "  border-left: 4px solid #F0B000;"
            "}"
        );
        nameButton->setCheckable(true);
        connect(nameButton, &QPushButton::clicked, this, [this, name]() {
            onProfileSelected(name);
        });
        
        profilesLayout->addWidget(nameButton);
        profileButtons.append(nameButton);
    }
}

void ProfilesDialog::onProfileSelected(const QString& profile)
{
    // Uncheck all other buttons
    for (auto button : profileButtons) {
        button->setChecked(button->text() == profile);
    }
    
    selectedProfile = profile;
    updateActionButtons();
}

void ProfilesDialog::onSelectProfileClicked() {
    if (!selectedProfile.isEmpty()) {
        emit profileSelected(selectedProfile);
        accept();
    }
}

void ProfilesDialog::updateActionButtons()
{
    bool hasSelection = !selectedProfile.isEmpty();
    viewButton->setEnabled(hasSelection);
    editButton->setEnabled(hasSelection);
    deleteButton->setEnabled(hasSelection);
    selectButton->setEnabled(hasSelection);
}

void ProfilesDialog::onViewProfileClicked()
{
    if (!selectedProfile.isEmpty()) {
        showProfileDetails(selectedProfile);
    }
}

void ProfilesDialog::onEditProfileClicked()
{
    if (!selectedProfile.isEmpty()) {
        editProfile(selectedProfile);
    }
}

void ProfilesDialog::onDeleteProfileClicked()
{
    if (selectedProfile.isEmpty()) return;
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete Profile",
        QString("Are you sure you want to delete profile '%1'?").arg(selectedProfile),
        QMessageBox::Yes | QMessageBox::No);
        
    if (reply == QMessageBox::Yes) {
        delete profiles[selectedProfile];
        profiles.remove(selectedProfile);
        selectedProfile.clear();
        updateActionButtons();
        saveProfiles();
        refreshProfilesList();
    }
}

void ProfilesDialog::showProfileDetails(const QString& name)
{
    if (!profiles.contains(name)) return;
    
    Profile* profile = profiles[name];
    QString details = QString(
        "Profile: %1\n\n"
        "Basal Rate: %2 U/hr\n"
        "Insulin-to-Carb Ratio: %3\n"
        "Correction Factor: %4\n"
        "Target BG: %5 mmol/L"
    ).arg(name)
     .arg(profile->getBasalRate())
     .arg(profile->getICR())
     .arg(profile->getCorrectionFactor())
     .arg(profile->getTargetBG());
    
    QMessageBox::information(this, "Profile Details", details);
}

void ProfilesDialog::editProfile(const QString& name)
{
    if (!profiles.contains(name)) return;
    
    Profile* profile = profiles[name];
    
    QDialog editDialog(this);
    editDialog.setWindowTitle("Edit Profile: " + name);
    editDialog.setStyleSheet("background-color: #333333; color: white;");
    
    QFormLayout *form = new QFormLayout(&editDialog);
    
    // Create input fields with current values
    QLineEdit *basalInput = new QLineEdit(QString::number(profile->getBasalRate()), &editDialog);
    QLineEdit *icrInput = new QLineEdit(QString::number(profile->getICR()), &editDialog);
    QLineEdit *cfInput = new QLineEdit(QString::number(profile->getCorrectionFactor()), &editDialog);
    QLineEdit *targetBGInput = new QLineEdit(QString::number(profile->getTargetBG()), &editDialog);
    
    // Style input fields
    QString inputStyle = "background-color: #444444; color: white; padding: 5px;";
    basalInput->setStyleSheet(inputStyle);
    icrInput->setStyleSheet(inputStyle);
    cfInput->setStyleSheet(inputStyle);
    targetBGInput->setStyleSheet(inputStyle);
    
    // Add fields to form
    form->addRow("Basal Rate (U/hr):", basalInput);
    form->addRow("Insulin-to-Carb Ratio:", icrInput);
    form->addRow("Correction Factor:", cfInput);
    form->addRow("Target BG (mmol/L):", targetBGInput);
    
    // Add save button
    QPushButton *saveButton = new QPushButton("SAVE CHANGES", &editDialog);
    saveButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #55cc55;"
        "  color: white;"
        "  border: none;"
        "  padding: 10px;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #44bb44;"
        "}"
    );
    form->addRow(saveButton);
    
    connect(saveButton, &QPushButton::clicked, &editDialog, [&]() {
        // Update profile values
        profile->setBasalRate(basalInput->text().toFloat());
        profile->setICR(icrInput->text().toFloat());
        profile->setCorrectionFactor(cfInput->text().toFloat());
        profile->setTargetBG(targetBGInput->text().toFloat());
        
        saveProfiles();
        refreshProfilesList();
        editDialog.accept();
    });
    
    editDialog.exec();
}

void ProfilesDialog::onAddProfileClicked()
{
    QDialog newProfileDialog(this);
    newProfileDialog.setWindowTitle("New Profile");
    newProfileDialog.setStyleSheet("background-color: #333333; color: white;");
    
    QFormLayout *form = new QFormLayout(&newProfileDialog);
    
    // Create input fields
    QLineEdit *nameInput = new QLineEdit(&newProfileDialog);
    QLineEdit *basalInput = new QLineEdit(&newProfileDialog);
    QLineEdit *icrInput = new QLineEdit(&newProfileDialog);
    QLineEdit *cfInput = new QLineEdit(&newProfileDialog);
    QLineEdit *targetBGInput = new QLineEdit(&newProfileDialog);
    
    // Style input fields
    QString inputStyle = "background-color: #444444; color: white; padding: 5px;";
    nameInput->setStyleSheet(inputStyle);
    basalInput->setStyleSheet(inputStyle);
    icrInput->setStyleSheet(inputStyle);
    cfInput->setStyleSheet(inputStyle);
    targetBGInput->setStyleSheet(inputStyle);
    
    // Add fields to form
    form->addRow("Profile Name:", nameInput);
    form->addRow("Basal Rate (U/hr):", basalInput);
    form->addRow("Insulin-to-Carb Ratio:", icrInput);
    form->addRow("Correction Factor:", cfInput);
    form->addRow("Target BG (mmol/L):", targetBGInput);
    
    // Add save button
    QPushButton *saveButton = new QPushButton("SAVE PROFILE", &newProfileDialog);
    saveButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #55cc55;"
        "  color: white;"
        "  border: none;"
        "  padding: 10px;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #44bb44;"
        "}"
    );
    form->addRow(saveButton);
    
    connect(saveButton, &QPushButton::clicked, &newProfileDialog, [&]() {
        // Validate inputs
        if (nameInput->text().isEmpty() || basalInput->text().isEmpty() ||
            icrInput->text().isEmpty() || cfInput->text().isEmpty() ||
            targetBGInput->text().isEmpty()) {
            QMessageBox::warning(&newProfileDialog, "Incomplete Form", 
                               "Please fill in all fields.");
            return;
        }
        
        // Create new profile
        QString name = nameInput->text();
        float basal = basalInput->text().toFloat();
        float icr = icrInput->text().toFloat();
        float cf = cfInput->text().toFloat();
        float targetBG = targetBGInput->text().toFloat();
        
        Profile* newProfile = new Profile(name, basal, icr, cf, targetBG);
        profiles[name] = newProfile;
        
        saveProfiles();
        refreshProfilesList();
        newProfileDialog.accept();
    });
    
    newProfileDialog.exec();
}

void ProfilesDialog::saveProfiles()
{
    QFile file("profiles.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonArray profilesArray;
        for (auto it = profiles.begin(); it != profiles.end(); ++it) {
            Profile* profile = it.value();
            QJsonObject profileObj;
            profileObj["name"] = it.key();
            profileObj["basalRate"] = profile->getBasalRate();
            profileObj["icr"] = profile->getICR();
            profileObj["cf"] = profile->getCorrectionFactor();
            profileObj["targetBG"] = profile->getTargetBG();
            profilesArray.append(profileObj);
        }
        
        QJsonDocument doc(profilesArray);
        file.write(doc.toJson());
        file.close();

        // Update MainWindow's profiles
        MainWindow* mainWindow = qobject_cast<MainWindow*>(parent());
        if (mainWindow) {
            mainWindow->updateProfiles();
        }
    }
}

void ProfilesDialog::loadProfiles()
{
    QFile file("profiles.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray profilesArray = doc.array();
        
        for (const QJsonValue &value : profilesArray) {
            QJsonObject profileObj = value.toObject();
            QString name = profileObj["name"].toString();
            float basalRate = profileObj["basalRate"].toDouble();
            float icr = profileObj["icr"].toDouble();
            float cf = profileObj["cf"].toDouble();
            float targetBG = profileObj["targetBG"].toDouble();
            
            Profile* profile = new Profile(name, basalRate, icr, cf, targetBG);
            profiles[name] = profile;
        }
        file.close();
    }
}

/**
 * This function sets the active profile for the pump. It updates the internal activeProfile
 * variable and the UI to reflect the selected profile. The bolus button is enabled only if
 * a profile is selected, and the profile label in the status bar is updated to show the
 * currently active profile.
 * 
 * @param profile The name of the profile to set as active
 */
void MainWindow::setActiveProfile(const QString& profile)
{
    activeProfile = profile;
    // Update the bolus button state
    ui->bolusButton->setEnabled(!profile.isEmpty());
    // Update the profile label if it exists
    if (profileLabel) {
        profileLabel->setText(QString("Active Profile: %1").arg(profile));
    }
}

void MainWindow::updateDateTime()
{
    QDateTime current = QDateTime::currentDateTime();
    
    // Update time in 24-hour format with seconds
    timeLabel->setText(current.toString("hh:mm:ss"));
    
    // Update date
    dateLabel->setText(current.toString("yyyy-MM-dd"));
}

void MainWindow::loadProfiles()
{
    QFile file("profiles.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray profilesArray = doc.array();
        
        for (const QJsonValue &value : profilesArray) {
            QJsonObject profileObj = value.toObject();
            QString name = profileObj["name"].toString();
            float basalRate = profileObj["basalRate"].toDouble();
            float icr = profileObj["icr"].toDouble();
            float cf = profileObj["cf"].toDouble();
            float targetBG = profileObj["targetBG"].toDouble();
            
            Profile* profile = new Profile(name, basalRate, icr, cf, targetBG);
            profiles[name] = profile;
        }
        file.close();
    }
}

void MainWindow::updateProfiles()
{
    // Clear existing profiles
    qDeleteAll(profiles);
    profiles.clear();

    // Reload profiles from file
    loadProfiles();
}

float MainWindow::calculateIntervalDelivery(float totalBolus, int totalDurationMs, int intervalMs)
{
    // Calculate how many intervals there will be
    int totalIntervals = totalDurationMs / intervalMs;
    
    // Calculate the amount to deliver per interval
    // This ensures the total amount is delivered by the end
    return totalBolus / totalIntervals;
}

void MainWindow::updateExtendedBolus()
{
    if (insulinDeliveryStopped) {
        return;  // Don't deliver insulin if delivery is stopped
    }

    if (remainingExtendedBolus > 0 && intervalsRemaining > 0) {
        // Calculate the amount to deliver this interval
        float deliveryAmount = calculateIntervalDelivery(totalExtendedBolus, totalDurationMs, deliveryIntervalMs);
        
        // Ensure we don't deliver more than remaining
        if (deliveryAmount > remainingExtendedBolus) {
            deliveryAmount = remainingExtendedBolus;
        }
        
        // Update insulin level
        insulinLevel -= deliveryAmount;
        remainingExtendedBolus -= deliveryAmount;
        intervalsRemaining--;
        
        // Update display
        updateInsulinDisplay();
        
        // If extended bolus is complete, stop the timer
        if (remainingExtendedBolus <= 0 || intervalsRemaining <= 0) {
            extendedBolusTimer->stop();
            QMessageBox::information(this, "Extended Bolus Complete", 
                "Extended bolus delivery has been completed.");
        }
    }
}

void MainWindow::onPowerButtonClicked() {
    if (!poweredOn) {
        poweredOn = true;

        // Enable full UI (initial power on)
        ui->optionsButton->setEnabled(true);
        ui->bolusButton->setEnabled(true);
        ui->rechargeButton->setEnabled(true);
        ui->refillButton->setEnabled(true);

        // Start battery drain timer
        batteryTimer->start(5000); // 5 seconds

        QMessageBox::information(this, "Power On", "Pump has powered on and is ready for use.");
        ui->powerButton->setText("Power");

    } else if (isSleeping) {
        isSleeping = false;

        // Resume battery drain timer
        batteryTimer->start(5000); // 5 seconds

        // Wake from sleep and re-enable buttons
        ui->optionsButton->setEnabled(true);
        ui->bolusButton->setEnabled(true);
        ui->rechargeButton->setEnabled(true);
        ui->refillButton->setEnabled(true);

        QMessageBox::information(this, "Wake", "Pump has woken from sleep.");
    } else {
        // Power off the pump
        poweredOn = false;
        
        // Stop battery drain timer
        batteryTimer->stop();
        
        // Disable all buttons except power
        ui->optionsButton->setEnabled(false);
        ui->bolusButton->setEnabled(false);
        ui->rechargeButton->setEnabled(false);
        ui->refillButton->setEnabled(false);
        
        QMessageBox::information(this, "Power Off", "Pump has been powered off.");
        ui->powerButton->setText("Power On");
    }
}

void MainWindow::enterSleepMode() {
    if (poweredOn && !isSleeping) {
        isSleeping = true;

        // Stop battery drain timer
        batteryTimer->stop();

        // Disable buttons except power
        ui->optionsButton->setEnabled(false);
        ui->bolusButton->setEnabled(false);
        ui->rechargeButton->setEnabled(false);
        ui->refillButton->setEnabled(false);

        QMessageBox::information(this, "Sleep Mode", "The pump is now asleep. Press Power to wake.");
    }
}

void MainWindow::addGlucoseReading(float glucoseValue)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    
    if (glucoseSeries->count() == 0) {
        lastGlucoseTime = currentTime;
        glucoseSeries->clear();
        glucoseSeries->append(0, glucoseValue);
    } else {
        // Calculate minutes since last reading
        int minutesSinceStart = lastGlucoseTime.secsTo(currentTime) / 60;
        glucoseSeries->append(minutesSinceStart, glucoseValue);
        
        // Update X axis if needed
        QValueAxis *axisX = qobject_cast<QValueAxis*>(glucoseChart->axes(Qt::Horizontal).first());
        if (minutesSinceStart > axisX->max()) {
            axisX->setRange(0, minutesSinceStart + 1);  // Add some padding
        }
    }
    
    // Update Y axis if needed
    QValueAxis *axisY = qobject_cast<QValueAxis*>(glucoseChart->axes(Qt::Vertical).first());
    if (glucoseValue > axisY->max()) {
        axisY->setRange(2, glucoseValue + 2);  // Add some padding
    } else if (glucoseValue < axisY->min()) {
        axisY->setRange(glucoseValue - 2, 22);  // Add some padding
    }
    
    // Update chart
    glucoseChart->update();
    
    // Log the manual BG reading if not in CGM mode
    if (!cgmModeEnabled) {
        eventLogger->logEvent(QString("[MANUAL BG READING] BG: %1 mmol/L at %2")
                            .arg(glucoseValue, 0, 'f', 1)
                            .arg(QTime::currentTime().toString("HH:mm:ss")));
    }
}

void MainWindow::updateGlucoseLevel()
{
    // Generate a random glucose value between 4.0 and 10.0 mmol/L
    double glucoseValue = 4.0 + (QRandomGenerator::global()->generateDouble() * 6.0);
    
    // Update the EventLogger with the new BG level
    eventLogger->updateBGLevel(glucoseValue);
    
    // Add the new data point to the series
    static QTime timeAxis;
    timeAxis = timeAxis.addSecs(300); // 5 minutes
    glucoseSeries->append(timeAxis.msecsSinceStartOfDay(), glucoseValue);
    
    // Keep only the last 24 hours of data (288 points at 5-minute intervals)
    if (glucoseSeries->count() > 288) {
        glucoseSeries->remove(0);
    }
    
    // Update the chart
    glucoseChart->update();
}

void MainWindow::onCgmModeToggled(bool enabled)
{
    cgmModeEnabled = enabled;
    
    // Disable bolus button when CGM mode is enabled
    ui->bolusButton->setEnabled(!enabled);
    
    if (enabled) {
        // Start CGM simulation
        cgmSimulationTimer->start(30000);  // 1 minute intervals
        QMessageBox::information(this, "CGM Mode Enabled", 
            "CGM mode is now active. Simulated readings will be added every minute.");
    } else {
        // Stop CGM simulation
        cgmSimulationTimer->stop();
        QMessageBox::information(this, "CGM Mode Disabled", 
            "CGM mode has been disabled. Manual bolus calculations are now available.");
    }
}

void MainWindow::onCgmSimulationTimeout()
{
    if (currentCgmValueIndex >= cgmSimulationValues.size()) {
        currentCgmValueIndex = 0;  // Reset to start of list
    }
    
    float bgValue = cgmSimulationValues[currentCgmValueIndex++];
    addGlucoseReading(bgValue);
    
    // Update the glucose display
    ui->glucoseValueLabel->setText(QString::number(bgValue, 'f', 1));
    
    // Log the CGM reading
    eventLogger->logEvent(QString("[CGM READING] BG: %1 mmol/L at %2")
                         .arg(bgValue, 0, 'f', 1)
                         .arg(QTime::currentTime().toString("HH:mm:ss")));
    
    // Check if automatic correction bolus is needed
    if (bgValue >= 120.0) {
        // Get the active profile for correction factor
        if (!activeProfile.isEmpty() && profiles.contains(activeProfile)) {
            Profile* profile = profiles[activeProfile];
            float targetBG = profile->getTargetBG();
            float correctionFactor = profile->getCorrectionFactor();
            
            // Calculate correction bolus
            float correctionBolus = (bgValue - targetBG) / correctionFactor;
            
            // Safety check: don't deliver more than 5 units at once
            if (correctionBolus > 5.0) {
                correctionBolus = 5.0;
            }
            
            // Check if we have enough insulin
            if (insulinLevel >= correctionBolus) {
                // Deliver the correction bolus
                insulinLevel -= correctionBolus;
                updateInsulinDisplay();
                
                // Log the automatic correction
                eventLogger->logEvent(QString("Automatic correction bolus: %1 units (BG: %2)")
                                    .arg(correctionBolus, 0, 'f', 1)
                                    .arg(bgValue, 0, 'f', 1));
                
                // Show notification to user
                QMessageBox::information(this, "Automatic Correction", 
                    QString("Delivered %1 units correction bolus for high BG (%2)")
                    .arg(correctionBolus, 0, 'f', 1)
                    .arg(bgValue, 0, 'f', 1));
            } else {
                QMessageBox::warning(this, "Low Insulin", 
                    "Not enough insulin available for automatic correction.");
            }
        }
    }
}





