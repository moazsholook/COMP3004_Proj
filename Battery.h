#ifndef BATTERY_H
#define BATTERY_H

class Battery {
private:
    int level;

public:
    Battery(int initialLevel = 100);
    
    void charge();
    bool isLow() const;
    bool isCritical() const;
    
    // Getters and setters
    int getLevel() const { return level; }
    void setLevel(int newLevel) { level = newLevel; }
};

#endif // BATTERY_H 