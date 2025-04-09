#include "ManualBolusDialog.h"
#include <QMessageBox>

ManualBolusDialog::ManualBolusDialog(QWidget *parent, Profile* profile)
    : QDialog(parent)
    , activeProfile(profile)
    , calculatedBolus(0.0)
{
    setWindowTitle("Manual Bolus");
    setModal(true);

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
    
    // Initially disable confirm button until calculation is done
    confirmButton->setEnabled(false);
}

void ManualBolusDialog::onCalculateClicked()
{
    if (!activeProfile) {
        QMessageBox::warning(this, "Error", "No profile selected!");
        return;
    }

    bool bgOk, carbsOk;
    float bg = bgInput->text().toFloat(&bgOk);
    float carbs = carbsInput->text().toFloat(&carbsOk);

    if (!bgOk || !carbsOk) {
        QMessageBox::warning(this, "Input Error", "Please enter valid numbers for BG and carbs");
        return;
    }

    // Use the active profile's settings
    float icr = activeProfile->getICR();
    float cf = activeProfile->getCorrectionFactor();
    float targetBG = activeProfile->getTargetBG();
    
    // Calculate bolus using profile settings
    float carbBolus = carbs / icr;  // Use profile's ICR
    float correctionBolus = (bg > targetBG) ? (bg - targetBG) / cf : 0;  // Use profile's CF and target BG
    calculatedBolus = carbBolus + correctionBolus;
    
    bolusResultLabel->setText(QString("Calculated Bolus: %1 units").arg(calculatedBolus));
    confirmButton->setEnabled(true);
}

void ManualBolusDialog::onConfirmClicked()
{
    QMessageBox::information(this, "Bolus Confirmed", 
        QString("Delivering %1 units of insulin").arg(calculatedBolus, 0, 'f', 2));
    accept();
} 