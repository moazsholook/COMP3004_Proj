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
#include "Battery.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Options Dialog
class OptionsDialog : public QDialog {
    Q_OBJECT
public:
    explicit OptionsDialog(QWidget *parent = nullptr);
private slots:
    void onStopInsulinClicked();
    void onProfilesClicked();
private:
    QPushButton *stopInsulinButton;
    QPushButton *profilesButton;
};

// Manual Bolus Dialog
class ManualBolusDialog : public QDialog {
    Q_OBJECT
public:
    explicit ManualBolusDialog(QWidget *parent = nullptr);
    float getCalculatedBolus() const { return calculatedBolus; }

signals:
    void bolusConfirmed(float amount);

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
};

// Profiles Dialog
class ProfilesDialog : public QDialog {
    Q_OBJECT
public:
    explicit ProfilesDialog(QWidget *parent = nullptr);

private slots:
    void onAddProfileClicked();
    void onProfileSelected(const QString& name);

private:
    QVBoxLayout *mainLayout;
    QVBoxLayout *profilesLayout;
    QPushButton *addProfileButton;
    std::vector<QPushButton*> profileButtons;
    
    void createNewProfile();
    void setupUI();
    void refreshProfilesList();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onOptionsClicked();
    void onBolusClicked();
    void onRechargeClicked();
    void onRefillClicked();
    void updateBatteryLevel();

private:
    Ui::MainWindow *ui;
    QChart *glucoseChart;
    QLineSeries *glucoseSeries;
    QChartView *chartView;
    QTimer *batteryTimer;
    Battery *battery;
    float insulinLevel = 300.0; // Maximum insulin level (300 units)
    
    void setupGlucoseChart();
    void updateBatteryDisplay();
    void updateInsulinDisplay();
};
#endif // MAINWINDOW_H
