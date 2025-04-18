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
    QList<QPushButton*> profileButtons;
    QMap<QString, Profile*> profiles;

    QPushButton *viewButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *selectButton;
    QString selectedProfile;

    void setupUI();
    void refreshProfilesList();
    void saveProfiles();
    void loadProfiles();
    void showProfileDetails(const QString& name);
    void editProfile(const QString& name);
    void updateActionButtons();
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setActiveProfile(const QString& profile);
    void setInsulinDeliveryStopped(bool stopped) { insulinDeliveryStopped = stopped; }
    void enterSleepMode();

public slots:
    void updateProfiles();

private slots:
    void onOptionsClicked();
    void onBolusClicked();
    void onRechargeClicked();
    void onRefillClicked();
    void updateBatteryLevel();
    void updateExtendedBolus();
    void onPowerButtonClicked();

private:
    Ui::MainWindow *ui;
    QMap<QString, Profile*> profiles;
    QString activeProfile;
    float insulinLevel;
    int batteryLevel;
    bool insulinDeliveryStopped;
    bool isSleeping;
    bool poweredOn;
    QTimer *batteryTimer;
    QTimer *glucoseTimer;
    QTimer *extendedBolusTimer;
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
    float remainingExtendedBolus;
    float totalExtendedBolus;
    int totalDurationMs;
    int deliveryIntervalMs;
    int intervalsRemaining;

    void setupGlucoseChart();
    void updateBatteryDisplay();
    void updateInsulinDisplay();
    void updateDateTime();
    void loadProfiles();
    float calculateIntervalDelivery(float totalBolus, int totalDurationMs, int intervalMs);
};

#endif
