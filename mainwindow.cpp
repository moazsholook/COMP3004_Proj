#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , profiles()
    , activeProfile()
    , insulinLevel(100.0f)  // Start with 100 units
    , batteryLevel(100)
    , batteryTimer(new QTimer(this))
    , glucoseTimer(new QTimer(this))
    , glucoseChart(new QChart())
    , glucoseSeries(new QLineSeries())
    , glucoseAxisX(new QValueAxis())
    , glucoseAxisY(new QValueAxis())
    , profileLabel(new QLabel("No Active Profile", this))
    , battery(new Battery())
{
    ui->setupUi(this);
    
    // Add profile label to status bar
    statusBar()->addPermanentWidget(profileLabel);
    
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
    updateBatteryDisplay();
    
    // Set up battery drain timer (drain 1% every 5 seconds)
    connect(batteryTimer, &QTimer::timeout, this, &MainWindow::updateBatteryLevel);
    batteryTimer->start(5000); // 5 seconds
    
    setupGlucoseChart();
    updateInsulinDisplay();  // Initial display update
    
    // Connect the buttons
    connect(ui->optionsButton, &QPushButton::clicked, this, &MainWindow::onOptionsClicked);
    connect(ui->bolusButton, &QPushButton::clicked, this, &MainWindow::onBolusClicked);
    connect(ui->rechargeButton, &QPushButton::clicked, this, &MainWindow::onRechargeClicked);
    connect(ui->refillButton, &QPushButton::clicked, this, &MainWindow::onRefillClicked);
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
        else if (battery->getLevel() == 20) {
            QMessageBox::warning(this, "Low Battery Warning", 
                "Battery is getting low at 20%! Please charge the device soon.");
        }
        else if (battery->getLevel() <= 10) {
            QMessageBox::critical(this, "Critical Battery Warning", 
                "Battery is critically low! The pump will stop soon if not charged.");
        }
    } else {
        batteryTimer->stop();
        QMessageBox::critical(this, "Battery Dead", "The pump has stopped due to dead battery!");
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

void MainWindow::onRechargeClicked()
{
    if (battery->getLevel() < 100) {
        battery->charge();
        updateBatteryDisplay();
        batteryTimer->start(); // Restart the timer if it was stopped
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
    ManualBolusDialog dialog(this, activeProfile);
    if (dialog.exec() == QDialog::Accepted) {
        // Get the calculated bolus amount from the dialog
        float bolusAmount = dialog.getCalculatedBolus();
        
        // Check if we have enough insulin in both the display and cartridge
        if (bolusAmount > insulinLevel) {
            QMessageBox::warning(this, "Insufficient Insulin", 
                "Not enough insulin remaining for this bolus!");
            return;
        }
        
        // Deduct the bolus amount from the display
        insulinLevel -= bolusAmount;
        
        // Update displays
        updateInsulinDisplay();
        
        QMessageBox::information(this, "Bolus Delivered", 
            QString("Bolus of %1 units delivered successfully.\nRemaining insulin: %2 units")
            .arg(bolusAmount)
            .arg(insulinLevel));
    }
}

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
    
    // Show warnings at appropriate levels
    if (insulinLevel <= 60 && insulinLevel > 30) {
        QMessageBox::warning(this, "Low Insulin Warning",
            "Insulin cartridge is getting low. Consider refilling soon.");
    } else if (insulinLevel <= 30) {
        QMessageBox::critical(this, "Critical Insulin Warning",
            "Insulin cartridge is critically low! Please refill immediately.");
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
    ProfilesDialog dialog(this);
    dialog.exec();
}

// Manual Bolus Dialog Implementation
ManualBolusDialog::ManualBolusDialog(QWidget *parent, const QString& profile) 
    : QDialog(parent), activeProfile(profile)
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

    // Load profile settings from profiles.json
    QFile file("profiles.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray profilesArray = doc.array();
        
        float icr = 10.0;  // Default values
        float cf = 2.0;
        float targetBG = 7.0;
        
        // Find the active profile settings
        for (const QJsonValue &value : profilesArray) {
            QJsonObject profileObj = value.toObject();
            if (profileObj["name"].toString() == activeProfile) {
                icr = profileObj["icr"].toDouble();
                cf = profileObj["cf"].toDouble();
                targetBG = profileObj["targetBG"].toDouble();
                break;
            }
        }
        
        // Calculate bolus using profile settings
        float carbBolus = carbs / icr;  // Use profile's ICR
        float correctionBolus = (bg > targetBG) ? (bg - targetBG) / cf : 0;  // Use profile's CF and target BG
        calculatedBolus = carbBolus + correctionBolus;
        
        bolusResultLabel->setText(QString("Calculated Bolus: %1 units").arg(calculatedBolus));
        confirmButton->setEnabled(true);
    }
}

void ManualBolusDialog::onConfirmClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Bolus",
        QString("Are you sure you want to deliver %1 units of insulin?").arg(calculatedBolus),
        QMessageBox::Yes | QMessageBox::No);
        
    if (reply == QMessageBox::Yes) {
        emit bolusConfirmed(calculatedBolus);
        accept();
    }
}

ProfilesDialog::ProfilesDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Profile Management");
    setMinimumWidth(400);
    setStyleSheet("background-color: #333333; color: white;");

    setupUI();
    loadProfiles();  // Load existing profiles
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

void ProfilesDialog::onProfileSelected(const QString& name)
{
    // Uncheck all buttons
    for (auto button : profileButtons) {
        button->setChecked(false);
    }
    
    // Check the selected button
    for (auto button : profileButtons) {
        if (button->text() == name) {
            button->setChecked(true);
            break;
        }
    }
    
    selectedProfile = name;
    updateActionButtons();
}

void ProfilesDialog::onSelectProfileClicked()
{
    if (profiles.isEmpty()) {
        QMessageBox::warning(this, "No Profiles", "No profiles available. Please create a profile first.");
        return;
    }

    bool ok;
    QString profile = QInputDialog::getItem(this, "Select Profile",
                                          "Choose a profile:", profiles.keys(), 0, false, &ok);
    
    if (ok && !profile.isEmpty()) {
        // Get the MainWindow instance and call its setActiveProfile method
        MainWindow* mainWindow = qobject_cast<MainWindow*>(parent());
        if (mainWindow) {
            mainWindow->setActiveProfile(profile);
            QMessageBox::information(this, "Profile Selected", 
                QString("Profile '%1' has been selected and is now active.").arg(profile));
        }
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

