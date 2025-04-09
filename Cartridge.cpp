#include "Cartridge.h"

// Constructor: initializes the cartridge's fill level
// Assumes initialFill is a value between 0 and 100 (percent full)
Cartridge::Cartridge(int initialFill) : fillLevel(initialFill) {}

// Refills the cartridge to its full capacity (100%)
// Could represent a full insulin cartridge in a medical device
void Cartridge::refill() {
    fillLevel = 100;
}

// Checks if the cartridge is considered full
// Returns true if fill level is at or above 100%
bool Cartridge::isFull() const {
    return fillLevel >= 100;
}

// Checks if the cartridge is empty
// Returns true if fill level is 0 or below
// Defensive check in case of over-draining
bool Cartridge::isEmpty() const {
    return fillLevel <= 0;
}
