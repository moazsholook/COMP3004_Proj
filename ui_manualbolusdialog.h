/********************************************************************************
** Form generated from reading UI file 'manualbolusdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANUALBOLUSDIALOG_H
#define UI_MANUALBOLUSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ManualBolusDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *inputGroup;
    QFormLayout *formLayout;
    QLabel *bgLabel;
    QDoubleSpinBox *bgSpin;
    QLabel *carbsLabel;
    QDoubleSpinBox *carbsSpin;
    QLabel *iobLabel;
    QDoubleSpinBox *iobSpin;
    QCheckBox *extendedBolusCheck;
    QGroupBox *extendedBolusGroup;
    QFormLayout *formLayout_2;
    QLabel *durationLabel;
    QSpinBox *durationSpin;
    QLabel *extendedBolusLabel;
    QLabel *bolusPerHourLabel;
    QLabel *bolusLabel;
    QPushButton *calculateButton;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ManualBolusDialog)
    {
        if (ManualBolusDialog->objectName().isEmpty())
            ManualBolusDialog->setObjectName(QString::fromUtf8("ManualBolusDialog"));
        ManualBolusDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(ManualBolusDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        inputGroup = new QGroupBox(ManualBolusDialog);
        inputGroup->setObjectName(QString::fromUtf8("inputGroup"));
        formLayout = new QFormLayout(inputGroup);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        bgLabel = new QLabel(inputGroup);
        bgLabel->setObjectName(QString::fromUtf8("bgLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, bgLabel);

        bgSpin = new QDoubleSpinBox(inputGroup);
        bgSpin->setObjectName(QString::fromUtf8("bgSpin"));
        bgSpin->setMinimum(2.000000000000000);
        bgSpin->setMaximum(22.000000000000000);
        bgSpin->setSingleStep(0.100000000000000);
        bgSpin->setValue(5.500000000000000);

        formLayout->setWidget(0, QFormLayout::FieldRole, bgSpin);

        carbsLabel = new QLabel(inputGroup);
        carbsLabel->setObjectName(QString::fromUtf8("carbsLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, carbsLabel);

        carbsSpin = new QDoubleSpinBox(inputGroup);
        carbsSpin->setObjectName(QString::fromUtf8("carbsSpin"));
        carbsSpin->setMinimum(0.000000000000000);
        carbsSpin->setMaximum(200.000000000000000);
        carbsSpin->setSingleStep(1.000000000000000);

        formLayout->setWidget(1, QFormLayout::FieldRole, carbsSpin);

        iobLabel = new QLabel(inputGroup);
        iobLabel->setObjectName(QString::fromUtf8("iobLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, iobLabel);

        iobSpin = new QDoubleSpinBox(inputGroup);
        iobSpin->setObjectName(QString::fromUtf8("iobSpin"));
        iobSpin->setMinimum(0.000000000000000);
        iobSpin->setMaximum(20.000000000000000);
        iobSpin->setSingleStep(0.100000000000000);

        formLayout->setWidget(2, QFormLayout::FieldRole, iobSpin);


        verticalLayout->addWidget(inputGroup);

        extendedBolusCheck = new QCheckBox(ManualBolusDialog);
        extendedBolusCheck->setObjectName(QString::fromUtf8("extendedBolusCheck"));

        verticalLayout->addWidget(extendedBolusCheck);

        extendedBolusGroup = new QGroupBox(ManualBolusDialog);
        extendedBolusGroup->setObjectName(QString::fromUtf8("extendedBolusGroup"));
        formLayout_2 = new QFormLayout(extendedBolusGroup);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        durationLabel = new QLabel(extendedBolusGroup);
        durationLabel->setObjectName(QString::fromUtf8("durationLabel"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, durationLabel);

        durationSpin = new QSpinBox(extendedBolusGroup);
        durationSpin->setObjectName(QString::fromUtf8("durationSpin"));
        durationSpin->setMinimum(1);
        durationSpin->setMaximum(8);
        durationSpin->setValue(2);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, durationSpin);

        extendedBolusLabel = new QLabel(extendedBolusGroup);
        extendedBolusLabel->setObjectName(QString::fromUtf8("extendedBolusLabel"));

        formLayout_2->setWidget(1, QFormLayout::SpanningRole, extendedBolusLabel);

        bolusPerHourLabel = new QLabel(extendedBolusGroup);
        bolusPerHourLabel->setObjectName(QString::fromUtf8("bolusPerHourLabel"));

        formLayout_2->setWidget(2, QFormLayout::SpanningRole, bolusPerHourLabel);


        verticalLayout->addWidget(extendedBolusGroup);

        bolusLabel = new QLabel(ManualBolusDialog);
        bolusLabel->setObjectName(QString::fromUtf8("bolusLabel"));
        bolusLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(bolusLabel);

        calculateButton = new QPushButton(ManualBolusDialog);
        calculateButton->setObjectName(QString::fromUtf8("calculateButton"));

        verticalLayout->addWidget(calculateButton);

        buttonBox = new QDialogButtonBox(ManualBolusDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ManualBolusDialog);

        QMetaObject::connectSlotsByName(ManualBolusDialog);
    } // setupUi

    void retranslateUi(QDialog *ManualBolusDialog)
    {
        ManualBolusDialog->setWindowTitle(QCoreApplication::translate("ManualBolusDialog", "Manual Bolus", nullptr));
        inputGroup->setTitle(QCoreApplication::translate("ManualBolusDialog", "Input Parameters", nullptr));
        bgLabel->setText(QCoreApplication::translate("ManualBolusDialog", "Blood Glucose (mmol/L):", nullptr));
        carbsLabel->setText(QCoreApplication::translate("ManualBolusDialog", "Carbohydrates (g):", nullptr));
        iobLabel->setText(QCoreApplication::translate("ManualBolusDialog", "Insulin On Board (units):", nullptr));
        extendedBolusCheck->setText(QCoreApplication::translate("ManualBolusDialog", "Use Extended Bolus", nullptr));
        extendedBolusGroup->setTitle(QCoreApplication::translate("ManualBolusDialog", "Extended Bolus Settings", nullptr));
        durationLabel->setText(QCoreApplication::translate("ManualBolusDialog", "Duration (hours):", nullptr));
        extendedBolusLabel->setText(QCoreApplication::translate("ManualBolusDialog", "Extended Bolus: 0.0 units over 2 hours", nullptr));
        bolusPerHourLabel->setText(QCoreApplication::translate("ManualBolusDialog", "Bolus Rate: 0.0 units/hour", nullptr));
        bolusLabel->setText(QCoreApplication::translate("ManualBolusDialog", "Calculated Bolus: 0.0 units", nullptr));
        calculateButton->setText(QCoreApplication::translate("ManualBolusDialog", "Calculate", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ManualBolusDialog: public Ui_ManualBolusDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANUALBOLUSDIALOG_H
