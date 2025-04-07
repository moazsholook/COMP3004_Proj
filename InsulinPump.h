#ifndef INSULINPUMP_H
#define INSULINPUMP_H

#include <vector>
#include <string>
#include "Profile.h"
#include "BolusPlan.h"
#include "Battery.h"
#include "Cartridge.h"
#include "Sensor.h"
#include "SystemLog.h"

class InsulinPump {
private:
    bool poweredOn;
    bool locked;
    Battery* battery;
    Cartridge* cartridge;
    std::vector<Profile*> profiles;
    Profile* activeProfile;
    float iob;
    Sensor* sensor;
    SystemLog* systemLog;

public:
    InsulinPump();
    ~InsulinPump();

    void powerOn();
    void powerOff();
    void lock();
    bool unlock(const std::string& pin);
    void addProfile(Profile* profile);
    void selectProfile(const std::string& name);
    BolusPlan* createManualBolus(float bg, float carbs);
    BolusPlan* createAutomaticBolus();
    void deliverBolus(BolusPlan* plan);
    void stopDelivery();
    void logEvent(const std::string& event);

    // Getters
    bool isPoweredOn() const { return poweredOn; }
    bool isLocked() const { return locked; }
    Profile* getActiveProfile() const { return activeProfile; }
    float getIOB() const { return iob; }
};

#endif // INSULINPUMP_H
