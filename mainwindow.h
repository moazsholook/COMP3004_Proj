#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QMessageBox>
#include <QTimer>
#include <QCheckBox>
#include <QSpinBox>
#include "Battery.h"
#include "Profile.h"
#include "EventLogger.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class OptionsDialog : public QDialog {
    Q_OBJECT
public:
    explicit OptionsDialog(QWidget *parent = nullptr);
private slots:
    void onStopInsulinClicked();
    void onProfilesClicked();
    void onSleepClicked();
private:
    QPushButton *stopInsulinButton;
    QPushButton *profilesButton;
    QPushButton *sleepButton;
};

class ManualBolusDialog : public QDialog {
    Q_OBJECT
public:
    explicit ManualBolusDialog(QWidget *parent = nullptr, Profile* profile = nullptr);
    float getCalculatedBolus() const { return calculatedBolus; }
    float getExtendedBolus() const { return extendedBolus; }
    int getDuration() const { return duration; }

signals:
    void bolusConfirmed(float immediateAmount, float extendedAmount, int duration);
    void newGlucoseReading(float glucose);

private slots:
    void onCalculateClicked();
    void onConfirmClicked();
    void onExtendedBolusToggled(bool checked);
    void onDurationChanged(int value);
    void onImmediatePercentageChanged(int value);

private:
    Profile* activeProfile;
    float calculatedBolus;
    float extendedBolus;
    int duration;
    int immediatePercentage;
    QLineEdit *bgInput;
    QLineEdit *carbsInput;
    QCheckBox *extendedBolusCheck;
    QSpinBox *durationInput;
    QSpinBox *immediatePercentageInput;
    QLabel *bolusResultLabel;
    QLabel *extendedBolusLabel;
    QLabel *bolusPerHourLabel;
    QPushButton *calculateButton;
    QPushButton *confirmButton;
    void updateExtendedBolusDisplay();
};

class ProfilesDialog : public QDialog {
    Q_OBJECT
public:
    explicit ProfilesDialog(QWidget *parent = nullptr);

signals:
    void profileSelected(const QString& profile);

private slots:
    void onAddProfileClicked();
    void onProfileSelected(const QString& name);
    void onViewProfileClicked();
    void onEditProfileClicked();
    void onDeleteProfileClicked();
    void onSelectProfileClicked();

private:
    QVBoxLayout *mainLayout;
    QVBoxLayout *profilesLayout;
    QPushButton *addProfileButton;
    QPushButton *viewButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *selectButton;
    QList<QPushButton*> profileButtons;
    QString selectedProfile;
    QMap<QString, Profile*> profiles;

    void setupUI();
    void refreshProfilesList();
    void updateActionButtons();
    void showProfileDetails(const QString& name);
    void editProfile(const QString& name);
    void saveProfiles();
    void loadProfiles();
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setActiveProfile(const QString& profile);
    void setInsulinDeliveryStopped(bool stopped) { insulinDeliveryStopped = stopped; }
    void enterSleepMode();
    void updateProfiles();
    void updateGlucoseLevel();

private slots:
    void onOptionsClicked();
    void onBolusClicked();
    void onRechargeClicked();
    void onRefillClicked();
    void onPowerButtonClicked();
    void updateBatteryLevel();
    void updateBatteryDisplay();
    void updateInsulinDisplay();
    void updateDateTime();
    void updateExtendedBolus();
    void onCgmModeToggled(bool enabled);
    void onCgmSimulationTimeout();

private:
    Ui::MainWindow *ui;
    QMap<QString, Profile*> profiles;
    QString activeProfile;
    double insulinLevel;
    int batteryLevel;
    bool insulinDeliveryStopped;
    bool isSleeping;
    bool poweredOn;
    bool cgmModeEnabled;
    QTimer *batteryTimer;
    QTimer *glucoseTimer;
    QTimer *extendedBolusTimer;
    QTimer *cgmSimulationTimer;  // Timer for CGM simulation
    QList<float> cgmSimulationValues;  // List of simulated BG values
    int currentCgmValueIndex;  // Current index in the simulation list
    QChart *glucoseChart;
    QLineSeries *glucoseSeries;
    QValueAxis *glucoseAxisX;
    QValueAxis *glucoseAxisY;
    QLabel *profileLabel;
    Battery *battery;
    QChartView *chartView;
    QLabel *timeLabel;
    QLabel *dateLabel;
    QTimer *clockTimer;
    double remainingExtendedBolus;
    double totalExtendedBolus;
    int totalDurationMs;
    int deliveryIntervalMs;
    int intervalsRemaining;
    QDateTime lastGlucoseTime;
    EventLogger *eventLogger;

    void setupGlucoseChart();
    void loadProfiles();
    float calculateIntervalDelivery(float totalBolus, int totalDurationMs, int intervalMs);
    void addGlucoseReading(float glucoseValue);
};

#endif
