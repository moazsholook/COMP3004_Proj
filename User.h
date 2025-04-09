#ifndef USER_H
#define USER_H

#include <string>
#include "Profile.h"
#include "BolusPlan.h"

class User {
public:
    bool enterPIN(const std::string& pin);
    void addProfile(Profile* profile);
    void selectProfile(const std::string& name);
    BolusPlan* requestManualBolus(float bg, float carbs);
    void confirmBolus(BolusPlan* plan);
    void stopInsulinDelivery();
    void powerOnPump();
    void powerOffPump();
    void chargeBattery();
    void refillCartridge();
};

#endif 