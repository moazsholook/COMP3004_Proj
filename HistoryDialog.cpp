#include "HistoryDialog.h"
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QFile>
#include <QTextStream>

HistoryDialog::HistoryDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("History Log");
    setMinimumSize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // Create text area for displaying history
    historyText = new QTextEdit(this);
    historyText->setReadOnly(true);
    layout->addWidget(historyText);

    // Create clear button
    QPushButton *clearButton = new QPushButton("Clear History", this);
    connect(clearButton, &QPushButton::clicked, this, &HistoryDialog::clearHistory);
    layout->addWidget(clearButton);

    // Load existing history
    loadHistory();
}

HistoryDialog::~HistoryDialog()
{
    // Save history before closing
    saveHistory();
}

void HistoryDialog::addEvent(const QString& event)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString formattedEvent = QString("[%1] %2\n")
        .arg(currentTime.toString("yyyy-MM-dd hh:mm:ss"))
        .arg(event);
    
    historyText->append(formattedEvent);
    saveHistory();
}

void HistoryDialog::loadHistory()
{
    QFile file("history.log");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        historyText->setPlainText(in.readAll());
        file.close();
    }
}

void HistoryDialog::saveHistory()
{
    QFile file("history.log");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << historyText->toPlainText();
        file.close();
    }
}

void HistoryDialog::clearHistory()
{
    historyText->clear();
    saveHistory();
} 