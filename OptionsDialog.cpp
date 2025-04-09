#include "OptionsDialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include "ProfileDialog.h"
#include "Profile.h"

// Constructor: sets up the Options dialog UI and connects signals to slots
OptionsDialog::OptionsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Options");  // Title of the dialog
    setModal(true);             // Makes dialog modal (blocks interaction with parent window)

    QVBoxLayout *layout = new QVBoxLayout(this);  // Vertical layout for stacking buttons
    
    // Stop Insulin button: Simulates halting insulin delivery
    stopInsulinButton = new QPushButton("Stop Insulin", this);
    layout->addWidget(stopInsulinButton);
    
    // Add Profile button: Opens a form to create a new profile
    addProfileButton = new QPushButton("Add Profile", this);
    layout->addWidget(addProfileButton);
    
    // Manage Profiles button: Could open a profile list or profile manager
    profilesButton = new QPushButton("Manage Profiles", this);
    layout->addWidget(profilesButton);
    
    // Connect button clicks to respective handler methods (slots)
    connect(stopInsulinButton, &QPushButton::clicked, this, &OptionsDialog::onStopInsulinClicked);
    connect(addProfileButton, &QPushButton::clicked, this, &OptionsDialog::onAddProfileClicked);
    connect(profilesButton, &QPushButton::clicked, this, &OptionsDialog::onProfilesClicked);
}

/**
 * Slot: Called when "Stop Insulin" button is clicked.
 * Displays a message box confirming that insulin delivery has stopped.
 */
void OptionsDialog::onStopInsulinClicked()
{
    QMessageBox::information(this, "Stop Insulin", "Insulin delivery stopped.");
}

/**
 * Slot: Called when "Add Profile" button is clicked.
 * Dynamically creates a temporary dialog for user input, allowing creation of a new Profile.
 * On confirmation, emits the `profileAdded` signal so other components (e.g., pump) can store it.
 */
void OptionsDialog::onAddProfileClicked()
{
    QDialog dialog(this);  // Create a temporary modal dialog
    dialog.setWindowTitle("Add New Profile");

    QFormLayout *form = new QFormLayout(&dialog);  // Layout for input fields

    // Input widgets for the profile parameters
    QLineEdit *nameEdit = new QLineEdit(&dialog);
    QDoubleSpinBox *icrSpin = new QDoubleSpinBox(&dialog);     // Insulin-to-Carb Ratio
    QDoubleSpinBox *cfSpin = new QDoubleSpinBox(&dialog);      // Correction Factor
    QDoubleSpinBox *targetSpin = new QDoubleSpinBox(&dialog);  // Target Blood Glucose
    QDoubleSpinBox *basalSpin = new QDoubleSpinBox(&dialog);   // Basal Rate

    // Configure value ranges and defaults
    icrSpin->setRange(1, 100);
    icrSpin->setValue(15);  // Typical default

    cfSpin->setRange(1, 100);
    cfSpin->setValue(50);

    targetSpin->setRange(70, 200);
    targetSpin->setValue(100);  // Standard target BG

    basalSpin->setRange(0.1, 10);
    basalSpin->setValue(1.0);
    basalSpin->setSingleStep(0.1);  // Allow fine-grained changes

    // Add all fields to the form layout
    form->addRow("Name:", nameEdit);
    form->addRow("Insulin to Carb Ratio:", icrSpin);
    form->addRow("Correction Factor:", cfSpin);
    form->addRow("Target BG:", targetSpin);
    form->addRow("Basal Rate:", basalSpin);

    // OK button to accept the dialog
    QPushButton *okButton = new QPushButton("OK", &dialog);
    form->addRow(okButton);

    // Connect OK button to dialog's accept method
    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    // If user presses OK and inputs are valid, create the profile
    if (dialog.exec() == QDialog::Accepted) {
        Profile* newProfile = new Profile(
            nameEdit->text().toStdString(),
            icrSpin->value(),
            cfSpin->value(),
            targetSpin->value(),
            basalSpin->value()
        );

        // Notify listeners that a new profile has been created
        emit profileAdded(newProfile);

        // Confirmation dialog
        QMessageBox::information(this, "Success", "New profile added successfully!");
    }
}

/**
 * Slot: Called when "Manage Profiles" button is clicked.
 * Emits a signal that can be used to open a profile management UI elsewhere.
 */
void OptionsDialog::onProfilesClicked()
{
    emit profilesClicked();
}
