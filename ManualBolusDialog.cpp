#include "ManualBolusDialog.h"
#include <QMessageBox>  // Used for error and confirmation dialogs

// Constructor: Sets up the dialog UI and connects signals to slots
// - parent: the parent QWidget
// - profile: the currently active Profile containing insulin settings
ManualBolusDialog::ManualBolusDialog(QWidget *parent, Profile* profile)
    : QDialog(parent)
    , activeProfile(profile)
    , calculatedBolus(0.0)  // Initial bolus value
{
    setWindowTitle("Manual Bolus");  // Set dialog title
    setModal(true);                  // Prevent interaction with other windows until closed

    // Layout for the overall dialog
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Form layout for inputs
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(10);

    // Create input fields and UI elements
    bgInput = new QLineEdit(this);  // Input for blood glucose
    carbsInput = new QLineEdit(this);  // Input for carbohydrates
    bolusResultLabel = new QLabel("Calculated Bolus: 0.0 units", this);  // Output label
    calculateButton = new QPushButton("Calculate", this);  // Triggers calculation
    confirmButton = new QPushButton("Confirm", this);  // Triggers confirmation

    // Add inputs to form layout
    formLayout->addRow("Blood Glucose (mmol/L):", bgInput);
    formLayout->addRow("Carbohydrates (g):", carbsInput);

    // Add everything to the main layout
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(bolusResultLabel);
    mainLayout->addWidget(calculateButton);
    mainLayout->addWidget(confirmButton);

    // Connect UI signals to event handlers (slots)
    connect(calculateButton, &QPushButton::clicked, this, &ManualBolusDialog::onCalculateClicked);
    connect(confirmButton, &QPushButton::clicked, this, &ManualBolusDialog::onConfirmClicked);

    // Disable confirm button until bolus is calculated
    confirmButton->setEnabled(false);
}

// Slot: Handles the "Calculate" button click
// Reads user inputs and calculates bolus based on profile's ICR, CF, and target BG
void ManualBolusDialog::onCalculateClicked()
{
    if (!activeProfile) {
        QMessageBox::warning(this, "Error", "No profile selected!");
        return;
    }

    bool bgOk, carbsOk;
    float bg = bgInput->text().toFloat(&bgOk);         // Try to parse BG input
    float carbs = carbsInput->text().toFloat(&carbsOk); // Try to parse carb input

    // Input validation: Ensure both inputs are valid numbers
    if (!bgOk || !carbsOk) {
        QMessageBox::warning(this, "Input Error", "Please enter valid numbers for BG and carbs");
        return;
    }

    // Fetch profile settings
    float icr = activeProfile->getICR();               // Insulin-to-carb ratio
    float cf = activeProfile->getCorrectionFactor();   // Correction factor (mg/dL per unit)
    float targetBG = activeProfile->getTargetBG();     // Target blood glucose

    // Bolus = (carbs / ICR) + correction (if BG above target)
    float carbBolus = carbs / icr;
    float correctionBolus = (bg > targetBG) ? (bg - targetBG) / cf : 0;

    calculatedBolus = carbBolus + correctionBolus;

    // Show result and enable confirmation
    bolusResultLabel->setText(QString("Calculated Bolus: %1 units").arg(calculatedBolus));
    confirmButton->setEnabled(true);
}

// Slot: Handles the "Confirm" button click
// Displays a confirmation message and closes the dialog
void ManualBolusDialog::onConfirmClicked()
{
    QMessageBox::information(this, "Bolus Confirmed", 
        QString("Delivering %1 units of insulin").arg(calculatedBolus, 0, 'f', 2));

    // Close the dialog and return QDialog::Accepted
    accept();
}
