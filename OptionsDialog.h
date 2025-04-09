#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include "Profile.h"

class OptionsDialog : public QDialog {
    Q_OBJECT
public:
    explicit OptionsDialog(QWidget *parent = nullptr);

signals:
    void profileAdded(Profile* profile);
    void profilesClicked();

private slots:
    void onStopInsulinClicked();
    void onAddProfileClicked();
    void onProfilesClicked();

private:
    QPushButton *stopInsulinButton;
    QPushButton *addProfileButton;
    QPushButton *profilesButton;
};

#endif 