#include "Battery.h"
#include <algorithm>

Battery::Battery(int initialLevel) : level(initialLevel) {}

void Battery::charge() {
    if (level < 100) {
        level = 100;
    }
}

bool Battery::isLow() const {
    return level <= 20;
}

bool Battery::isCritical() const {
    return level <= 5;
}

void Battery::drain(int amount) {
    if (amount > 0) {
        level = std::max(0, level - amount);
    }
}

void Battery::refill() {
    level = 100;
} 