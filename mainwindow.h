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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onOptionsClicked();
    void onBolusClicked();

private:
    Ui::MainWindow *ui;
    QChart *glucoseChart;
    QLineSeries *glucoseSeries;
    QChartView *chartView;
    
    void setupGlucoseChart();
};
#endif // MAINWINDOW_H
