#include "OptionsDialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include "ProfileDialog.h"
#include "Profile.h"

OptionsDialog::OptionsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Options");
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // Stop Insulin button
    stopInsulinButton = new QPushButton("Stop Insulin", this);
    layout->addWidget(stopInsulinButton);
    
    // Add Profile button
    addProfileButton = new QPushButton("Add Profile", this);
    layout->addWidget(addProfileButton);
    
    // Profiles button
    profilesButton = new QPushButton("Manage Profiles", this);
    layout->addWidget(profilesButton);
    
    // Connect signals
    connect(stopInsulinButton, &QPushButton::clicked, this, &OptionsDialog::onStopInsulinClicked);
    connect(addProfileButton, &QPushButton::clicked, this, &OptionsDialog::onAddProfileClicked);
    connect(profilesButton, &QPushButton::clicked, this, &OptionsDialog::onProfilesClicked);
}

void OptionsDialog::onStopInsulinClicked()
{
    QMessageBox::information(this, "Stop Insulin", "Insulin delivery stopped.");
}

void OptionsDialog::onAddProfileClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Profile");
    
    QFormLayout *form = new QFormLayout(&dialog);
    QLineEdit *nameEdit = new QLineEdit(&dialog);
    QDoubleSpinBox *icrSpin = new QDoubleSpinBox(&dialog);
    QDoubleSpinBox *cfSpin = new QDoubleSpinBox(&dialog);
    QDoubleSpinBox *targetSpin = new QDoubleSpinBox(&dialog);
    QDoubleSpinBox *basalSpin = new QDoubleSpinBox(&dialog);
    
    // Setup spin boxes
    icrSpin->setRange(1, 100);
    icrSpin->setValue(15);
    cfSpin->setRange(1, 100);
    cfSpin->setValue(50);
    targetSpin->setRange(70, 200);
    targetSpin->setValue(100);
    basalSpin->setRange(0.1, 10);
    basalSpin->setValue(1.0);
    basalSpin->setSingleStep(0.1);
    
    form->addRow("Name:", nameEdit);
    form->addRow("Insulin to Carb Ratio:", icrSpin);
    form->addRow("Correction Factor:", cfSpin);
    form->addRow("Target BG:", targetSpin);
    form->addRow("Basal Rate:", basalSpin);
    
    QPushButton *okButton = new QPushButton("OK", &dialog);
    form->addRow(okButton);
    
    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    
    if (dialog.exec() == QDialog::Accepted) {
        Profile* newProfile = new Profile(
            nameEdit->text().toStdString(),
            icrSpin->value(),
            cfSpin->value(),
            targetSpin->value(),
            basalSpin->value()
        );
        
        emit profileAdded(newProfile);
        QMessageBox::information(this, "Success", "New profile added successfully!");
    }
}

void OptionsDialog::onProfilesClicked()
{
    emit profilesClicked();
} 