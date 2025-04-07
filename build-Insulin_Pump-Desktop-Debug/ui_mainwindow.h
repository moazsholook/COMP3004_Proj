/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QFrame *statusBarFrame;
    QHBoxLayout *statusBarLayout;
    QVBoxLayout *batteryLayout;
    QProgressBar *batteryProgressBar;
    QLabel *batteryLabel;
<<<<<<< HEAD
    QPushButton *rechargeButton;
=======
>>>>>>> eb6053f3cc6bf5eb42807707cf41208950aed492
    QLabel *filterIcon;
    QSpacerItem *horizontalSpacer1;
    QVBoxLayout *timeLayout;
    QLabel *timeLabel;
    QLabel *dateLabel;
    QSpacerItem *horizontalSpacer2;
    QLabel *bolusIcon;
    QVBoxLayout *insulinLayout;
    QProgressBar *insulinProgressBar;
    QLabel *insulinLabel;
<<<<<<< HEAD
    QPushButton *refillButton;
=======
>>>>>>> eb6053f3cc6bf5eb42807707cf41208950aed492
    QFrame *mainContentFrame;
    QHBoxLayout *mainContentLayout;
    QFrame *cgmGraphFrame;
    QHBoxLayout *cgmGraphLayout;
    QSpacerItem *graphSpacer;
    QVBoxLayout *cgmMarkersLayout;
    QLabel *marker22;
    QLabel *marker18;
    QLabel *marker14;
    QLabel *marker10;
    QLabel *marker6;
    QLabel *marker2;
    QVBoxLayout *rightSideLayout;
    QFrame *glucoseFrame;
    QVBoxLayout *glucoseFrameLayout;
    QLabel *glucoseValueLabel;
    QLabel *glucoseUnitLabel;
    QLabel *glucoseTrendLabel;
    QFrame *hoursFrame;
    QVBoxLayout *hoursFrameLayout;
    QLabel *hoursValueLabel;
    QLabel *hoursTextLabel;
    QPushButton *tandemLogoButton;
    QSpacerItem *rightSideSpacer;
    QFrame *bottomFrame;
    QVBoxLayout *bottomFrameLayout;
    QLabel *iobLabel;
    QHBoxLayout *actionButtonsLayout;
    QPushButton *optionsButton;
    QPushButton *bolusButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(724, 381);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: #222222;"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        statusBarFrame = new QFrame(centralWidget);
        statusBarFrame->setObjectName(QString::fromUtf8("statusBarFrame"));
        statusBarFrame->setMinimumSize(QSize(0, 40));
        statusBarFrame->setMaximumSize(QSize(16777215, 40));
        statusBarFrame->setStyleSheet(QString::fromUtf8("background-color: #444444;"));
        statusBarFrame->setFrameShape(QFrame::NoFrame);
        statusBarFrame->setFrameShadow(QFrame::Plain);
        statusBarLayout = new QHBoxLayout(statusBarFrame);
        statusBarLayout->setSpacing(0);
        statusBarLayout->setObjectName(QString::fromUtf8("statusBarLayout"));
        statusBarLayout->setContentsMargins(5, 2, 5, 2);
        batteryLayout = new QVBoxLayout();
        batteryLayout->setSpacing(0);
        batteryLayout->setObjectName(QString::fromUtf8("batteryLayout"));
        batteryProgressBar = new QProgressBar(statusBarFrame);
        batteryProgressBar->setObjectName(QString::fromUtf8("batteryProgressBar"));
        batteryProgressBar->setMinimumSize(QSize(50, 20));
        batteryProgressBar->setMaximumSize(QSize(50, 20));
        batteryProgressBar->setStyleSheet(QString::fromUtf8("QProgressBar {\n"
"    border: 1px solid #333333;\n"
"    border-radius: 0px;\n"
"    background-color: #333333;\n"
"    text-align: center;\n"
"}\n"
"QProgressBar::chunk {\n"
"    background-color: #55cc55;\n"
"}"));
        batteryProgressBar->setValue(100);
        batteryProgressBar->setTextVisible(false);

        batteryLayout->addWidget(batteryProgressBar);

        batteryLabel = new QLabel(statusBarFrame);
        batteryLabel->setObjectName(QString::fromUtf8("batteryLabel"));
        batteryLabel->setStyleSheet(QString::fromUtf8("color: #55cc55; font-size: 10px; font-weight: bold;"));
        batteryLabel->setAlignment(Qt::AlignCenter);

        batteryLayout->addWidget(batteryLabel);

<<<<<<< HEAD
        rechargeButton = new QPushButton(statusBarFrame);
        rechargeButton->setObjectName(QString::fromUtf8("rechargeButton"));
        rechargeButton->setMinimumSize(QSize(50, 15));
        rechargeButton->setMaximumSize(QSize(50, 15));
        rechargeButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #333333;\n"
"    color: #55cc55;\n"
"    border: none;\n"
"    font-size: 8px;\n"
"    font-weight: bold;\n"
"    border-radius: 2px;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #444444;\n"
"}"));

        batteryLayout->addWidget(rechargeButton);

=======
>>>>>>> eb6053f3cc6bf5eb42807707cf41208950aed492

        statusBarLayout->addLayout(batteryLayout);

        filterIcon = new QLabel(statusBarFrame);
        filterIcon->setObjectName(QString::fromUtf8("filterIcon"));
        filterIcon->setMinimumSize(QSize(25, 25));
        filterIcon->setStyleSheet(QString::fromUtf8("color: white;"));
        filterIcon->setAlignment(Qt::AlignCenter);

        statusBarLayout->addWidget(filterIcon);

        horizontalSpacer1 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        statusBarLayout->addItem(horizontalSpacer1);

        timeLayout = new QVBoxLayout();
        timeLayout->setObjectName(QString::fromUtf8("timeLayout"));
        timeLabel = new QLabel(statusBarFrame);
        timeLabel->setObjectName(QString::fromUtf8("timeLabel"));
        timeLabel->setStyleSheet(QString::fromUtf8("color: white; font-size: 15px; font-weight: bold;"));
        timeLabel->setAlignment(Qt::AlignCenter);

        timeLayout->addWidget(timeLabel);

        dateLabel = new QLabel(statusBarFrame);
        dateLabel->setObjectName(QString::fromUtf8("dateLabel"));
        dateLabel->setStyleSheet(QString::fromUtf8("color: white; font-size: 12px;"));
        dateLabel->setAlignment(Qt::AlignCenter);

        timeLayout->addWidget(dateLabel);


        statusBarLayout->addLayout(timeLayout);

        horizontalSpacer2 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        statusBarLayout->addItem(horizontalSpacer2);

        bolusIcon = new QLabel(statusBarFrame);
        bolusIcon->setObjectName(QString::fromUtf8("bolusIcon"));
        bolusIcon->setMinimumSize(QSize(25, 25));
        bolusIcon->setStyleSheet(QString::fromUtf8("color: #66aaff; font-weight: bold; font-size: 14px;"));
        bolusIcon->setAlignment(Qt::AlignCenter);

        statusBarLayout->addWidget(bolusIcon);

        insulinLayout = new QVBoxLayout();
        insulinLayout->setSpacing(0);
        insulinLayout->setObjectName(QString::fromUtf8("insulinLayout"));
        insulinProgressBar = new QProgressBar(statusBarFrame);
        insulinProgressBar->setObjectName(QString::fromUtf8("insulinProgressBar"));
        insulinProgressBar->setMinimumSize(QSize(50, 20));
        insulinProgressBar->setMaximumSize(QSize(50, 20));
        insulinProgressBar->setStyleSheet(QString::fromUtf8("QProgressBar {\n"
"    border: 1px solid #333333;\n"
"    border-radius: 0px;\n"
"    background-color: #333333;\n"
"    text-align: center;\n"
"}\n"
"QProgressBar::chunk {\n"
"    background-color: #66aaff;\n"
"}"));
        insulinProgressBar->setValue(78);
        insulinProgressBar->setTextVisible(false);

        insulinLayout->addWidget(insulinProgressBar);

        insulinLabel = new QLabel(statusBarFrame);
        insulinLabel->setObjectName(QString::fromUtf8("insulinLabel"));
        insulinLabel->setStyleSheet(QString::fromUtf8("color: #66aaff; font-size: 10px; font-weight: bold;"));
        insulinLabel->setAlignment(Qt::AlignCenter);

        insulinLayout->addWidget(insulinLabel);

<<<<<<< HEAD
        refillButton = new QPushButton(statusBarFrame);
        refillButton->setObjectName(QString::fromUtf8("refillButton"));
        refillButton->setMinimumSize(QSize(50, 15));
        refillButton->setMaximumSize(QSize(50, 15));
        refillButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #333333;\n"
"    color: #66aaff;\n"
"    border: none;\n"
"    font-size: 8px;\n"
"    font-weight: bold;\n"
"    border-radius: 2px;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #444444;\n"
"}"));

        insulinLayout->addWidget(refillButton);

=======
>>>>>>> eb6053f3cc6bf5eb42807707cf41208950aed492

        statusBarLayout->addLayout(insulinLayout);


        verticalLayout->addWidget(statusBarFrame);

        mainContentFrame = new QFrame(centralWidget);
        mainContentFrame->setObjectName(QString::fromUtf8("mainContentFrame"));
        mainContentFrame->setStyleSheet(QString::fromUtf8("background-color: #222222;"));
        mainContentFrame->setFrameShape(QFrame::NoFrame);
        mainContentFrame->setFrameShadow(QFrame::Plain);
        mainContentLayout = new QHBoxLayout(mainContentFrame);
        mainContentLayout->setSpacing(2);
        mainContentLayout->setObjectName(QString::fromUtf8("mainContentLayout"));
        mainContentLayout->setContentsMargins(0, 0, 0, 0);
        cgmGraphFrame = new QFrame(mainContentFrame);
        cgmGraphFrame->setObjectName(QString::fromUtf8("cgmGraphFrame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(2);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cgmGraphFrame->sizePolicy().hasHeightForWidth());
        cgmGraphFrame->setSizePolicy(sizePolicy);
        cgmGraphFrame->setStyleSheet(QString::fromUtf8("background-color: #333333;"));
        cgmGraphFrame->setFrameShape(QFrame::NoFrame);
        cgmGraphFrame->setFrameShadow(QFrame::Plain);
        cgmGraphLayout = new QHBoxLayout(cgmGraphFrame);
        cgmGraphLayout->setSpacing(0);
        cgmGraphLayout->setObjectName(QString::fromUtf8("cgmGraphLayout"));
        cgmGraphLayout->setContentsMargins(0, 0, 0, 0);
        graphSpacer = new QSpacerItem(180, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        cgmGraphLayout->addItem(graphSpacer);

        cgmMarkersLayout = new QVBoxLayout();
        cgmMarkersLayout->setSpacing(0);
        cgmMarkersLayout->setObjectName(QString::fromUtf8("cgmMarkersLayout"));
        cgmMarkersLayout->setContentsMargins(-1, -1, 5, -1);
        marker22 = new QLabel(cgmGraphFrame);
        marker22->setObjectName(QString::fromUtf8("marker22"));
        marker22->setStyleSheet(QString::fromUtf8("color: white; font-size: 10px;"));
        marker22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        cgmMarkersLayout->addWidget(marker22);

        marker18 = new QLabel(cgmGraphFrame);
        marker18->setObjectName(QString::fromUtf8("marker18"));
        marker18->setStyleSheet(QString::fromUtf8("color: white; font-size: 10px;"));
        marker18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        cgmMarkersLayout->addWidget(marker18);

        marker14 = new QLabel(cgmGraphFrame);
        marker14->setObjectName(QString::fromUtf8("marker14"));
        marker14->setStyleSheet(QString::fromUtf8("color: white; font-size: 10px;"));
        marker14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        cgmMarkersLayout->addWidget(marker14);

        marker10 = new QLabel(cgmGraphFrame);
        marker10->setObjectName(QString::fromUtf8("marker10"));
        marker10->setStyleSheet(QString::fromUtf8("color: white; font-size: 10px;"));
        marker10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        cgmMarkersLayout->addWidget(marker10);

        marker6 = new QLabel(cgmGraphFrame);
        marker6->setObjectName(QString::fromUtf8("marker6"));
        marker6->setStyleSheet(QString::fromUtf8("color: white; font-size: 10px;"));
        marker6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        cgmMarkersLayout->addWidget(marker6);

        marker2 = new QLabel(cgmGraphFrame);
        marker2->setObjectName(QString::fromUtf8("marker2"));
        marker2->setStyleSheet(QString::fromUtf8("color: white; font-size: 10px;"));
        marker2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        cgmMarkersLayout->addWidget(marker2);


        cgmGraphLayout->addLayout(cgmMarkersLayout);


        mainContentLayout->addWidget(cgmGraphFrame);

        rightSideLayout = new QVBoxLayout();
        rightSideLayout->setSpacing(5);
        rightSideLayout->setObjectName(QString::fromUtf8("rightSideLayout"));
        glucoseFrame = new QFrame(mainContentFrame);
        glucoseFrame->setObjectName(QString::fromUtf8("glucoseFrame"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(glucoseFrame->sizePolicy().hasHeightForWidth());
        glucoseFrame->setSizePolicy(sizePolicy1);
        glucoseFrame->setMinimumSize(QSize(100, 80));
        glucoseFrame->setStyleSheet(QString::fromUtf8("background-color: #333333;"));
        glucoseFrame->setFrameShape(QFrame::NoFrame);
        glucoseFrame->setFrameShadow(QFrame::Plain);
        glucoseFrameLayout = new QVBoxLayout(glucoseFrame);
        glucoseFrameLayout->setObjectName(QString::fromUtf8("glucoseFrameLayout"));
        glucoseFrameLayout->setContentsMargins(5, 5, 5, 5);
        glucoseValueLabel = new QLabel(glucoseFrame);
        glucoseValueLabel->setObjectName(QString::fromUtf8("glucoseValueLabel"));
        glucoseValueLabel->setStyleSheet(QString::fromUtf8("color: white; font-size: 32px; font-weight: bold;"));
        glucoseValueLabel->setAlignment(Qt::AlignCenter);

        glucoseFrameLayout->addWidget(glucoseValueLabel);

        glucoseUnitLabel = new QLabel(glucoseFrame);
        glucoseUnitLabel->setObjectName(QString::fromUtf8("glucoseUnitLabel"));
        glucoseUnitLabel->setStyleSheet(QString::fromUtf8("color: white; font-size: 14px;"));
        glucoseUnitLabel->setAlignment(Qt::AlignCenter);

        glucoseFrameLayout->addWidget(glucoseUnitLabel);

        glucoseTrendLabel = new QLabel(glucoseFrame);
        glucoseTrendLabel->setObjectName(QString::fromUtf8("glucoseTrendLabel"));
        glucoseTrendLabel->setStyleSheet(QString::fromUtf8("color: white; font-size: 26px; font-weight: bold;"));
        glucoseTrendLabel->setAlignment(Qt::AlignCenter);

        glucoseFrameLayout->addWidget(glucoseTrendLabel);


        rightSideLayout->addWidget(glucoseFrame);

        hoursFrame = new QFrame(mainContentFrame);
        hoursFrame->setObjectName(QString::fromUtf8("hoursFrame"));
        hoursFrame->setMinimumSize(QSize(100, 60));
        hoursFrame->setStyleSheet(QString::fromUtf8("background-color: #444444;"));
        hoursFrame->setFrameShape(QFrame::NoFrame);
        hoursFrame->setFrameShadow(QFrame::Plain);
        hoursFrameLayout = new QVBoxLayout(hoursFrame);
        hoursFrameLayout->setSpacing(0);
        hoursFrameLayout->setObjectName(QString::fromUtf8("hoursFrameLayout"));
        hoursFrameLayout->setContentsMargins(5, 5, 5, 5);
        hoursValueLabel = new QLabel(hoursFrame);
        hoursValueLabel->setObjectName(QString::fromUtf8("hoursValueLabel"));
        hoursValueLabel->setStyleSheet(QString::fromUtf8("color: white; font-size: 28px; font-weight: bold;"));
        hoursValueLabel->setAlignment(Qt::AlignCenter);

        hoursFrameLayout->addWidget(hoursValueLabel);

        hoursTextLabel = new QLabel(hoursFrame);
        hoursTextLabel->setObjectName(QString::fromUtf8("hoursTextLabel"));
        hoursTextLabel->setStyleSheet(QString::fromUtf8("color: white; font-size: 16px;"));
        hoursTextLabel->setAlignment(Qt::AlignCenter);

        hoursFrameLayout->addWidget(hoursTextLabel);


        rightSideLayout->addWidget(hoursFrame);

        tandemLogoButton = new QPushButton(mainContentFrame);
        tandemLogoButton->setObjectName(QString::fromUtf8("tandemLogoButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tandemLogoButton->sizePolicy().hasHeightForWidth());
        tandemLogoButton->setSizePolicy(sizePolicy2);
        tandemLogoButton->setMinimumSize(QSize(60, 35));
        tandemLogoButton->setMaximumSize(QSize(60, 35));
        tandemLogoButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"  background-color: #666666;\n"
"  border: none;\n"
"  border-radius: 3px;\n"
"}\n"
"QPushButton:pressed {\n"
"  background-color: #555555;\n"
"}"));

        rightSideLayout->addWidget(tandemLogoButton);

        rightSideSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        rightSideLayout->addItem(rightSideSpacer);


        mainContentLayout->addLayout(rightSideLayout);


        verticalLayout->addWidget(mainContentFrame);

        bottomFrame = new QFrame(centralWidget);
        bottomFrame->setObjectName(QString::fromUtf8("bottomFrame"));
        bottomFrame->setMinimumSize(QSize(0, 60));
        bottomFrame->setMaximumSize(QSize(16777215, 60));
        bottomFrame->setStyleSheet(QString::fromUtf8("background-color: #333333;"));
        bottomFrame->setFrameShape(QFrame::NoFrame);
        bottomFrame->setFrameShadow(QFrame::Plain);
        bottomFrameLayout = new QVBoxLayout(bottomFrame);
        bottomFrameLayout->setSpacing(0);
        bottomFrameLayout->setObjectName(QString::fromUtf8("bottomFrameLayout"));
        bottomFrameLayout->setContentsMargins(0, 0, 0, 0);
        iobLabel = new QLabel(bottomFrame);
        iobLabel->setObjectName(QString::fromUtf8("iobLabel"));
        iobLabel->setMinimumSize(QSize(0, 20));
        iobLabel->setMaximumSize(QSize(16777215, 20));
        iobLabel->setStyleSheet(QString::fromUtf8("color: white; font-size: 12px; background-color: #444444;"));
        iobLabel->setAlignment(Qt::AlignCenter);

        bottomFrameLayout->addWidget(iobLabel);

        actionButtonsLayout = new QHBoxLayout();
        actionButtonsLayout->setSpacing(0);
        actionButtonsLayout->setObjectName(QString::fromUtf8("actionButtonsLayout"));
        optionsButton = new QPushButton(bottomFrame);
        optionsButton->setObjectName(QString::fromUtf8("optionsButton"));
        optionsButton->setMinimumSize(QSize(0, 40));
        optionsButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"  background-color: #444444;\n"
"  color: #F0B000;\n"
"  border: none;\n"
"  font-size: 16px;\n"
"  font-weight: bold;\n"
"}\n"
"QPushButton:pressed {\n"
"  background-color: #333333;\n"
"}"));
        QIcon icon;
        icon.addFile(QString::fromUtf8("icons/gear.png"), QSize(), QIcon::Normal, QIcon::Off);
        optionsButton->setIcon(icon);
        optionsButton->setIconSize(QSize(20, 20));

        actionButtonsLayout->addWidget(optionsButton);

        bolusButton = new QPushButton(bottomFrame);
        bolusButton->setObjectName(QString::fromUtf8("bolusButton"));
        bolusButton->setMinimumSize(QSize(0, 40));
        bolusButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"  background-color: #555555;\n"
"  color: #66AAFF;\n"
"  border: none;\n"
"  font-size: 16px;\n"
"  font-weight: bold;\n"
"}\n"
"QPushButton:pressed {\n"
"  background-color: #444444;\n"
"}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("icons/drop.png"), QSize(), QIcon::Normal, QIcon::Off);
        bolusButton->setIcon(icon1);
        bolusButton->setIconSize(QSize(20, 20));

        actionButtonsLayout->addWidget(bolusButton);


        bottomFrameLayout->addLayout(actionButtonsLayout);


        verticalLayout->addWidget(bottomFrame);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "T:SLIM X2 Insulin Pump", nullptr));
        batteryLabel->setText(QCoreApplication::translate("MainWindow", "100%", nullptr));
<<<<<<< HEAD
        rechargeButton->setText(QCoreApplication::translate("MainWindow", "RECHARGE", nullptr));
=======
>>>>>>> eb6053f3cc6bf5eb42807707cf41208950aed492
        filterIcon->setText(QCoreApplication::translate("MainWindow", "\342\226\274", nullptr));
        timeLabel->setText(QCoreApplication::translate("MainWindow", "10:20", nullptr));
        dateLabel->setText(QCoreApplication::translate("MainWindow", "14 Nov", nullptr));
        bolusIcon->setText(QCoreApplication::translate("MainWindow", "B", nullptr));
        insulinLabel->setText(QCoreApplication::translate("MainWindow", "235 u", nullptr));
<<<<<<< HEAD
        refillButton->setText(QCoreApplication::translate("MainWindow", "REFILL", nullptr));
=======
>>>>>>> eb6053f3cc6bf5eb42807707cf41208950aed492
        marker22->setText(QCoreApplication::translate("MainWindow", "\342\227\200 22", nullptr));
        marker18->setText(QCoreApplication::translate("MainWindow", "\342\227\200 18", nullptr));
        marker14->setText(QCoreApplication::translate("MainWindow", "\342\227\200 14", nullptr));
        marker10->setText(QCoreApplication::translate("MainWindow", "\342\227\200 10", nullptr));
        marker6->setText(QCoreApplication::translate("MainWindow", "\342\227\200 6", nullptr));
        marker2->setText(QCoreApplication::translate("MainWindow", "\342\227\200 2", nullptr));
        glucoseValueLabel->setText(QCoreApplication::translate("MainWindow", "4.9", nullptr));
        glucoseUnitLabel->setText(QCoreApplication::translate("MainWindow", "mmol/L", nullptr));
        glucoseTrendLabel->setText(QCoreApplication::translate("MainWindow", "\342\236\241", nullptr));
        hoursValueLabel->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        hoursTextLabel->setText(QCoreApplication::translate("MainWindow", "HRS", nullptr));
        tandemLogoButton->setText(QString());
        iobLabel->setText(QCoreApplication::translate("MainWindow", "INSULIN ON BOARD    2.5 u | 3:45 hrs", nullptr));
        optionsButton->setText(QCoreApplication::translate("MainWindow", "OPTIONS", nullptr));
        bolusButton->setText(QCoreApplication::translate("MainWindow", "BOLUS", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
