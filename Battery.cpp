#include "Battery.h"
#include <algorithm> // for std::max

// Constructor: initializes the battery level to the given initialLevel
Battery::Battery(int initialLevel) : level(initialLevel) {}

// Fully charges the battery, but only if it's not already at 100%
// Prevents unnecessary overwrite if battery is already full
void Battery::charge() {
    if (level < 100) {
        level = 100;
    }
}

// Checks if the battery level is low (defined as 20% or below)
// Could be used to trigger low battery warnings
bool Battery::isLow() const {
    return level <= 20;
}

// Checks if the battery level is critically low (defined as 5% or below)
// Useful for emergency shutdown or alerts
bool Battery::isCritical() const {
    return level <= 5;
}

// Reduces the battery level by a specified amount
// Ensures the level never drops below 0
// Also ignores invalid (non-positive) drain amounts
void Battery::drain(int amount) {
    if (amount > 0) {
        level = std::max(0, level - amount);
    }
}

// Immediately restores battery level to 100%
// Useful for resetting state or testing
void Battery::refill() {
    level = 100;
}
