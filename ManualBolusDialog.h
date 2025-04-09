#ifndef MANUALBOLUSDIALOG_H
#define MANUALBOLUSDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include "Profile.h"

class ManualBolusDialog : public QDialog {
    Q_OBJECT
public:
    explicit ManualBolusDialog(QWidget *parent = nullptr, Profile* profile = nullptr);
    float getCalculatedBolus() const { return calculatedBolus; }

private slots:
    void onCalculateClicked();
    void onConfirmClicked();

private:
    QLineEdit *bgInput;
    QLineEdit *carbsInput;
    QLabel *bolusResultLabel;
    QPushButton *calculateButton;
    QPushButton *confirmButton;
    float calculatedBolus;
    Profile* activeProfile;
};

#endif 