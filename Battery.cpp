#include "Battery.h"

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