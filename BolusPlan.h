#ifndef BOLUSPLAN_H
#define BOLUSPLAN_H

#include <string>

class BolusPlan {
private:
    float carbBolus;
    float correctionBolus;
    float totalBolus;
    float immediateDose;
    float extendedDose;
    int duration;

public:
    BolusPlan(float carbBolus, float correctionBolus, float immediateDose, 
              float extendedDose, int duration);
    
    void recalculate(float bg, float carbs, float iob, float icr);
    std::string getSummary() const;
    
    // Getters
    float getCarbBolus() const { return carbBolus; }
    float getCorrectionBolus() const { return correctionBolus; }
    float getTotalBolus() const { return totalBolus; }
    float getImmediateDose() const { return immediateDose; }
    float getExtendedDose() const { return extendedDose; }
    int getDuration() const { return duration; }
};

#endif 