#include "Sensor.h"

float Sensor::getCurrentBG() const {
    // This would be implemented to read from actual glucose sensor
    // For now, returning a placeholder value
    return 120.0f;
}

float Sensor::getCarbInput() const {
    // This would be implemented to read from actual carb input
    // For now, returning a placeholder value
    return 0.0f;
} 