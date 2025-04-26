#ifndef EVENTLOGGER_H
#define EVENTLOGGER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QString>

class EventLogger : public QObject
{
    Q_OBJECT

public:
    explicit EventLogger(QObject *parent = nullptr);
    ~EventLogger();

    void startLogging();
    void stopLogging();

    // Event logging methods
    void logBatteryAlert(int level);
    void logCartridgeAlert(int level);
    void logImmediateBolus(double units);
    void logExtendedBolus(double units);
    void logInsulinDelivery(double units);
    void logExtendedBolusStart(double totalAmount, int durationMinutes);
    void logExtendedBolusComplete(double amountDelivered);
    void logBatteryWarning(int level);
    void logLowInsulinWarning(int remainingUnits);
    void updateBGLevel(double bgLevel);
    void logEvent(const QString &event);

signals:
    void newEventLogged(const QString &event, const QDateTime &timestamp);

private slots:
    void onTimerTick();

private:
    QTimer *m_timer;
    QDateTime m_lastExtendedBolusStart;
    double m_extendedBolusAmount;
    int m_extendedBolusDuration;
    QTimer *extendedBolusTimer;
    double extendedBolusRemaining;
    double extendedBolusRate;
    double lastBGLevel;
};

#endif // EVENTLOGGER_H 