#include "InsulinPump.h"

InsulinPump::InsulinPump()
    : poweredOn(false), locked(true), battery(new Battery()),
      cartridge(new Cartridge()), activeProfile(nullptr), iob(0.0f),
      sensor(new Sensor()), systemLog(new SystemLog()) {}

InsulinPump::~InsulinPump() {
    delete battery;
    delete cartridge;
    delete sensor;
    delete systemLog;
    for (Profile* profile : profiles) {
        delete profile;
    }
}

void InsulinPump::powerOn() {
    poweredOn = true;
    systemLog->log("Pump powered on");
}

void InsulinPump::powerOff() {
    poweredOn = false;
    systemLog->log("Pump powered off");
}

void InsulinPump::lock() {
    locked = true;
    systemLog->log("Pump locked");
}

bool InsulinPump::unlock(const std::string& pin) {
    // In a real implementation, this would verify the PIN
    if (pin == "1234") {  // Placeholder PIN
        locked = false;
        systemLog->log("Pump unlocked");
        return true;
    }
    return false;
}

void InsulinPump::addProfile(Profile* profile) {
    profiles.push_back(profile);
    systemLog->log("New profile added: " + profile->getName());
}

void InsulinPump::selectProfile(const std::string& name) {
    for (Profile* profile : profiles) {
        if (profile->getName() == name) {
            activeProfile = profile;
            systemLog->log("Profile selected: " + name);
            return;
        }
    }
}

BolusPlan* InsulinPump::createManualBolus(float bg, float carbs) {
    if (!activeProfile) {
        systemLog->log("Error: No active profile selected");
        return nullptr;
    }
    
    BolusPlan* plan = new BolusPlan(0, 0, 0, 0, 0);
    plan->recalculate(bg, carbs, iob, activeProfile->getICR());
    return plan;
}

BolusPlan* InsulinPump::createAutomaticBolus() {
    if (!activeProfile) {
        systemLog->log("Error: No active profile selected");
        return nullptr;
    }
    
    float currentBG = sensor->getCurrentBG();
    float carbs = sensor->getCarbInput();
    return createManualBolus(currentBG, carbs);
}

void InsulinPump::deliverBolus(BolusPlan* plan) {
    if (!plan) {
        systemLog->log("Error: Invalid bolus plan");
        return;
    }
    
    if (cartridge->isEmpty()) {
        systemLog->log("Error: Cartridge empty");
        return;
    }
    
    // In a real implementation, this would control the pump mechanism
    systemLog->log("Delivering bolus: " + plan->getSummary());
    iob += plan->getTotalBolus();
}

void InsulinPump::stopDelivery() {
    systemLog->log("Insulin delivery stopped");
}

void InsulinPump::logEvent(const std::string& event) {
    systemLog->log(event);
} 