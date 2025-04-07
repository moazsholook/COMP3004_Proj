#ifndef CARTRIDGE_H
#define CARTRIDGE_H

class Cartridge {
private:
    int fillLevel;

public:
    Cartridge(int initialFill = 100);
    
    void refill();
    bool isFull() const;
    bool isEmpty() const;
    
    // Getters and setters
    int getFillLevel() const { return fillLevel; }
    void setFillLevel(int newLevel) { fillLevel = newLevel; }
};

#endif // CARTRIDGE_H 