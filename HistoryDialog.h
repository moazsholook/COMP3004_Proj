#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDateTime>

class EventLogger;

class HistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryDialog(QWidget *parent = nullptr);
    ~HistoryDialog();

    void setEventLogger(EventLogger *logger);

private slots:
    void onNewEventLogged(const QString &event, const QDateTime &timestamp);

private:
    QTextEdit *historyText;
    EventLogger *m_eventLogger;
    void setupUI();
};

#endif // HISTORYDIALOG_H 