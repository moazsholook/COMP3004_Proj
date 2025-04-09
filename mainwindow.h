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
#include "Profile.h"

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
    explicit ManualBolusDialog(QWidget *parent = nullptr, Profile* profile = nullptr);
    float getCalculatedBolus() const { return calculatedBolus; }

signals:
    void bolusConfirmed(float amount);

private slots:
    void onCalculateClicked();
    void onConfirmClicked();

private:
    Profile* activeProfile;
    float calculatedBolus;
    QLineEdit *bgInput;
    QLineEdit *carbsInput;
    QLabel *bolusResultLabel;
    QPushButton *calculateButton;
    QPushButton *confirmButton;
};

// Profiles Dialog
class ProfilesDialog : public QDialog
{
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
    
    // Action buttons
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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setActiveProfile(const QString& profile);

public slots:
    void updateProfiles();  

private slots:
    void onOptionsClicked();
    void onBolusClicked();
    void onRechargeClicked();
    void onRefillClicked();
    void updateBatteryLevel();

private:
    Ui::MainWindow *ui;
    QMap<QString, Profile*> profiles;
    QString activeProfile;
    float insulinLevel;
    int batteryLevel;
    QTimer *batteryTimer;
    QTimer *glucoseTimer;
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
    
    void setupGlucoseChart();
    void updateBatteryDisplay();
    void updateInsulinDisplay();
    void updateDateTime();
    void loadProfiles();
};
#endif 
