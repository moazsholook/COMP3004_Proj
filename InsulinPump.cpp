#include "InsulinPump.h"

// Constructor: initializes all components of the pump
// Dynamically allocates memory for Battery, Cartridge, Sensor, and SystemLog
// Starts with pump powered off and locked
InsulinPump::InsulinPump()
    : poweredOn(false), locked(true), battery(new Battery()),
      cartridge(new Cartridge()), activeProfile(nullptr), iob(0.0f),
      sensor(new Sensor()), systemLog(new SystemLog()) {}

// Destructor: ensures proper cleanup of dynamically allocated memory
// Deletes battery, cartridge, sensor, system log, and all profiles
InsulinPump::~InsulinPump() {
    delete battery;
    delete cartridge;
    delete sensor;
    delete systemLog;
    for (Profile* profile : profiles) {
        delete profile;
    }
}

// Powers on the pump and logs the event
void InsulinPump::powerOn() {
    poweredOn = true;
    systemLog->log("Pump powered on");
}

// Powers off the pump and logs the event
void InsulinPump::powerOff() {
    poweredOn = false;
    systemLog->log("Pump powered off");
}

// Locks the pump to prevent unintended interaction
void InsulinPump::lock() {
    locked = true;
    systemLog->log("Pump locked");
}

// Unlocks the pump if correct PIN is provided
// Logs success and returns true, otherwise returns false
bool InsulinPump::unlock(const std::string& pin) {
    if (pin == "1234") {  // NOTE: Placeholder PIN; not secure in production
        locked = false;
        systemLog->log("Pump unlocked");
        return true;
    }
    return false;
}

// Adds a new user profile to the pump
// Each profile can have unique insulin-to-carb ratios or settings
void InsulinPump::addProfile(Profile* profile) {
    profiles.push_back(profile);
    systemLog->log("New profile added: " + profile->getName());
}

// Selects a profile by name and sets it as the active profile
// Only one profile can be active at a time
void InsulinPump::selectProfile(const std::string& name) {
    for (Profile* profile : profiles) {
        if (profile->getName() == name) {
            activeProfile = profile;
            systemLog->log("Profile selected: " + name);
            return;
        }
    }
}

// Manually creates a bolus plan using current blood glucose and carb input
// Requires an active profile to determine insulin-to-carb ratio (ICR)
// Returns nullptr if no profile is active
BolusPlan* InsulinPump::createManualBolus(float bg, float carbs) {
    if (!activeProfile) {
        systemLog->log("Error: No active profile selected");
        return nullptr;
    }
    
    BolusPlan* plan = new BolusPlan(0, 0, 0, 0, 0); // Initial values; recalculated below
    plan->recalculate(bg, carbs, iob, activeProfile->getICR());
    return plan;
}

// Automatically creates a bolus plan using sensor data for BG and carbs
// Delegates to createManualBolus after gathering data
BolusPlan* InsulinPump::createAutomaticBolus() {
    if (!activeProfile) {
        systemLog->log("Error: No active profile selected");
        return nullptr;
    }
    
    float currentBG = sensor->getCurrentBG();
    float carbs = sensor->getCarbInput();
    return createManualBolus(currentBG, carbs);
}

// Delivers a bolus using the provided plan
// Logs errors for null plans or empty cartridges
// Increments IOB (insulin on board) based on total bolus delivered
void InsulinPump::deliverBolus(BolusPlan* plan) {
    if (!plan) {
        systemLog->log("Error: Invalid bolus plan");
        return;
    }
    
    if (cartridge->isEmpty()) {
        systemLog->log("Error: Cartridge empty");
        return;
    }
    
    // In real hardware, this would interface with the delivery system
    systemLog->log("Delivering bolus: " + plan->getSummary());
    iob += plan->getTotalBolus();
}

// Stops insulin delivery (e.g., in emergencies)
void InsulinPump::stopDelivery() {
    systemLog->log("Insulin delivery stopped");
}

// Allows arbitrary event logging (for UI, debugging, etc.)
void InsulinPump::logEvent(const std::string& event) {
    systemLog->log(event);
}
