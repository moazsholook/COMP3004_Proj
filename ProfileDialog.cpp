#include "ProfileDialog.h"
#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QLabel>

ProfileDialog::ProfileDialog(QWidget *parent)
    : QDialog(parent)
    , selectedProfile(nullptr)
{
    setupUI();
}

void ProfileDialog::setupUI() {
    setWindowTitle("Profile Management");
    setModal(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Profile selection
    profileComboBox = new QComboBox(this);
    mainLayout->addWidget(new QLabel("Select Profile:"));
    mainLayout->addWidget(profileComboBox);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("Add Profile", this);
    editButton = new QPushButton("Edit Profile", this);
    deleteButton = new QPushButton("Delete Profile", this);
    okButton = new QPushButton("OK", this);
    cancelButton = new QPushButton("Cancel", this);
    
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Connect signals
    connect(profileComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ProfileDialog::onProfileSelected);
    connect(addButton, &QPushButton::clicked, this, &ProfileDialog::onAddProfileClicked);
    connect(editButton, &QPushButton::clicked, this, &ProfileDialog::onEditProfileClicked);
    connect(deleteButton, &QPushButton::clicked, this, &ProfileDialog::onDeleteProfileClicked);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void ProfileDialog::setProfiles(const std::vector<Profile*>& newProfiles) {
    profiles = newProfiles;
    updateProfileList();
}

Profile* ProfileDialog::getSelectedProfile() const {
    return selectedProfile;
}

void ProfileDialog::updateProfileList() {
    profileComboBox->clear();
    for (Profile* profile : profiles) {
        profileComboBox->addItem(QString::fromStdString(profile->getName()));
    }
}

void ProfileDialog::onProfileSelected(int index) {
    if (index >= 0 && index < static_cast<int>(profiles.size())) {
        selectedProfile = profiles[index];
        showProfileDetails(selectedProfile);
    }
}

void ProfileDialog::onAddProfileClicked() {
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
        
        profiles.push_back(newProfile);
        updateProfileList();
        selectedProfile = newProfile;
    }
}

void ProfileDialog::onEditProfileClicked() {
    if (!selectedProfile) {
        QMessageBox::warning(this, "Warning", "Please select a profile to edit.");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Profile");
    
    QFormLayout *form = new QFormLayout(&dialog);
    QLineEdit *nameEdit = new QLineEdit(QString::fromStdString(selectedProfile->getName()), &dialog);
    QDoubleSpinBox *icrSpin = new QDoubleSpinBox(&dialog);
    QDoubleSpinBox *cfSpin = new QDoubleSpinBox(&dialog);
    QDoubleSpinBox *targetSpin = new QDoubleSpinBox(&dialog);
    QDoubleSpinBox *basalSpin = new QDoubleSpinBox(&dialog);
    
    // Setup spin boxes with current values
    icrSpin->setRange(1, 100);
    icrSpin->setValue(selectedProfile->getInsulinToCarbRatio());
    cfSpin->setRange(1, 100);
    cfSpin->setValue(selectedProfile->getCorrectionFactor());
    targetSpin->setRange(70, 200);
    targetSpin->setValue(selectedProfile->getTargetBG());
    basalSpin->setRange(0.1, 10);
    basalSpin->setValue(selectedProfile->getBasalRate());
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
        selectedProfile->setName(nameEdit->text().toStdString());
        selectedProfile->setInsulinToCarbRatio(icrSpin->value());
        selectedProfile->setCorrectionFactor(cfSpin->value());
        selectedProfile->setTargetBG(targetSpin->value());
        selectedProfile->setBasalRate(basalSpin->value());
        
        updateProfileList();
    }
}

void ProfileDialog::onDeleteProfileClicked() {
    if (!selectedProfile) {
        QMessageBox::warning(this, "Warning", "Please select a profile to delete.");
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Delete",
        "Are you sure you want to delete this profile?",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        auto it = std::find(profiles.begin(), profiles.end(), selectedProfile);
        if (it != profiles.end()) {
            delete *it;
            profiles.erase(it);
            selectedProfile = nullptr;
            updateProfileList();
        }
    }
}

void ProfileDialog::showProfileDetails(Profile* profile) {
    if (!profile) return;
    
    QString details = QString("Profile Details:\n"
                            "Name: %1\n"
                            "Insulin to Carb Ratio: %2\n"
                            "Correction Factor: %3\n"
                            "Target BG: %4\n"
                            "Basal Rate: %5")
                            .arg(QString::fromStdString(profile->getName()))
                            .arg(profile->getInsulinToCarbRatio())
                            .arg(profile->getCorrectionFactor())
                            .arg(profile->getTargetBG())
                            .arg(profile->getBasalRate());
    
    QMessageBox::information(this, "Profile Details", details);
} 