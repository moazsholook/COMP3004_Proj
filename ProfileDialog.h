#ifndef PROFILEDIALOG_H
#define PROFILEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QMessageBox>
#include <vector>
#include "Profile.h"

class ProfileDialog : public QDialog {
    Q_OBJECT
public:
    explicit ProfileDialog(QWidget *parent = nullptr);
    void setProfiles(const std::vector<Profile*>& profiles);
    Profile* getSelectedProfile() const;

private slots:
    void onAddProfileClicked();
    void onEditProfileClicked();
    void onDeleteProfileClicked();
    void onProfileSelected(int index);

private:
    QComboBox *profileComboBox;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *okButton;
    QPushButton *cancelButton;
    
    std::vector<Profile*> profiles;
    Profile* selectedProfile;

    void setupUI();
    void updateProfileList();
    void showProfileDetails(Profile* profile);
};

#endif // PROFILEDIALOG_H 