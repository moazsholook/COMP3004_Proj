#include "Cartridge.h"

Cartridge::Cartridge(int initialFill) : fillLevel(initialFill) {}

void Cartridge::refill() {
    fillLevel = 100;
}

bool Cartridge::isFull() const {
    return fillLevel >= 100;
}

bool Cartridge::isEmpty() const {
    return fillLevel <= 0;
} 