#include "EventLogger.h"
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QTime>

EventLogger::EventLogger(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
    , m_lastExtendedBolusStart()
    , m_extendedBolusAmount(0)
    , m_extendedBolusDuration(0)
    , extendedBolusTimer(new QTimer(this))
    , extendedBolusRemaining(0)
    , extendedBolusRate(0)
    , lastBGLevel(0.0)
{
    // Create history file if it doesn't exist
    QFile file("history.txt");
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.close();
        }
    }

    m_timer->setInterval(60000); // 1 minute intervals
    connect(m_timer, &QTimer::timeout, this, &EventLogger::onTimerTick);
    
    connect(extendedBolusTimer, &QTimer::timeout, this, [this]() {
        qDebug() << "Extended bolus timer tick - Remaining:" << extendedBolusRemaining;
        if (extendedBolusRemaining > 0) {
            double unitsToDeliver = qMin(extendedBolusRate, extendedBolusRemaining);
            extendedBolusRemaining -= unitsToDeliver;
            logExtendedBolus(unitsToDeliver);
            
            if (extendedBolusRemaining <= 0) {
                qDebug() << "Extended bolus timer detected completion";
                extendedBolusTimer->stop();
                logExtendedBolusComplete(m_extendedBolusAmount);
            }
        } else {
            qDebug() << "Extended bolus timer detected completion (remaining <= 0)";
            extendedBolusTimer->stop();
            logExtendedBolusComplete(m_extendedBolusAmount);
        }
    });
}

EventLogger::~EventLogger()
{
    stopLogging();
    extendedBolusTimer->stop();
    delete extendedBolusTimer;
    delete m_timer;
}

void EventLogger::startLogging()
{
    m_timer->start();
}

void EventLogger::stopLogging()
{
    m_timer->stop();
}

void EventLogger::logBatteryAlert(int level)
{
    QString message = QString("[BATTERY ALERT] Battery at %1%%. Please consider charging.")
                        .arg(level);
    qDebug() << message;
    emit newEventLogged(message, QDateTime::currentDateTime());
}

void EventLogger::logCartridgeAlert(int level)
{
    QString message = QString("[CARTRIDGE ALERT] Insulin cartridge is low (%1 units). Please refill soon.")
                        .arg(level);
    qDebug() << message;
    emit newEventLogged(message, QDateTime::currentDateTime());
}

void EventLogger::logImmediateBolus(double units)
{
    QString message = QString("[BOLUS DELIVERED] Immediate bolus of %1 units delivered at %2.")
                        .arg(units)
                        .arg(QDateTime::currentDateTime().toString("HH:mm:ss"));
    qDebug() << message;
    emit newEventLogged(message, QDateTime::currentDateTime());
}

void EventLogger::logExtendedBolus(double units)
{
    QString message = QString("[EXTENDED BOLUS] Delivered %1 units at %2.")
                        .arg(units)
                        .arg(QDateTime::currentDateTime().toString("HH:mm:ss"));
    qDebug() << message;
    qDebug() << "Remaining units: " << extendedBolusRemaining;
    emit newEventLogged(message, QDateTime::currentDateTime());
}

void EventLogger::logInsulinDelivery(double units)
{
    QString message = QString("[INSULIN DELIVERY] %1 units delivered at %2.")
                        .arg(units)
                        .arg(QDateTime::currentDateTime().toString("HH:mm:ss"));
    qDebug() << message;
    emit newEventLogged(message, QDateTime::currentDateTime());
}

void EventLogger::logExtendedBolusStart(double totalAmount, int durationMinutes)
{
    m_lastExtendedBolusStart = QDateTime::currentDateTime();
    m_extendedBolusAmount = totalAmount;
    m_extendedBolusDuration = durationMinutes;
    extendedBolusRemaining = totalAmount;
    extendedBolusRate = totalAmount / (durationMinutes * 60.0 / 5.0); // Rate per 5 seconds
    
    QString message = QString("[EXTENDED BOLUS] Started: %1 units over %2 minutes")
                        .arg(totalAmount)
                        .arg(durationMinutes);
    qDebug() << message;
    qDebug() << "Extended bolus rate: " << extendedBolusRate << " units per 5 seconds";
    emit newEventLogged(message, QDateTime::currentDateTime());
    
    // Ensure timer is stopped before starting
    if (extendedBolusTimer->isActive()) {
        extendedBolusTimer->stop();
    }
    
    // Start the timer with a shorter interval for more frequent updates
    extendedBolusTimer->start(1000); // Check every second
    qDebug() << "Extended bolus timer started with " << extendedBolusRemaining << " units remaining";
}

void EventLogger::logExtendedBolusComplete(double amountDelivered)
{
    QString message = QString("[EXTENDED BOLUS] Completed: %1 units delivered over %2 minutes")
                        .arg(amountDelivered)
                        .arg(m_extendedBolusDuration);
    
    // Print completion message to terminal
    qDebug() << "\n*****************************************";
    qDebug() << "EXTENDED BOLUS DELIVERY COMPLETE!";
    qDebug() << "Total delivered: " << amountDelivered << " units";
    qDebug() << "Duration: " << m_extendedBolusDuration << " minutes";
    qDebug() << "*****************************************\n";
    
    emit newEventLogged(message, QDateTime::currentDateTime());
    
    // Reset all extended bolus variables
    m_extendedBolusAmount = 0;
    m_extendedBolusDuration = 0;
    extendedBolusRemaining = 0;
    extendedBolusRate = 0;
    
    // Ensure timer is stopped
    if (extendedBolusTimer->isActive()) {
        extendedBolusTimer->stop();
    }
}

void EventLogger::logBatteryWarning(int level)
{
    logBatteryAlert(level);
}

void EventLogger::logLowInsulinWarning(int remainingUnits)
{
    logCartridgeAlert(remainingUnits);
}

void EventLogger::updateBGLevel(double bgLevel)
{
    lastBGLevel = bgLevel;
    qDebug() << "EventLogger: BG level updated to" << bgLevel;
}

void EventLogger::onTimerTick()
{
    qDebug() << "EventLogger: Timer tick - Current BG level:" << lastBGLevel;
    
    // Log BG reading every minute
    if (lastBGLevel > 0) {
        QString message = QString("[BG READING] Last recorded BG: %1 mmol/L at %2")
            .arg(lastBGLevel, 0, 'f', 1)
            .arg(QTime::currentTime().toString("HH:mm:ss"));
        qDebug() << "EventLogger: Logged BG reading:" << message;
        emit newEventLogged(message, QDateTime::currentDateTime());
    } else {
        qDebug() << "EventLogger: No BG reading available to log";
    }
} 