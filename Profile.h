#ifndef PROFILE_H
#define PROFILE_H

#include <string>
#include <map>

class Profile {
private:
    std::string name;
    float basalRate;
    float icr;  // Insulin to Carb Ratio
    float correctionFactor;
    float targetBG;

public:
    Profile(const std::string& name, float basalRate, float icr, 
            float correctionFactor, float targetBG);
    
    std::map<std::string, float> getParameters() const;
    
    // Getters
    std::string getName() const { return name; }
    float getBasalRate() const { return basalRate; }
    float getICR() const { return icr; }
    float getCorrectionFactor() const { return correctionFactor; }
    float getTargetBG() const { return targetBG; }
};

#endif // PROFILE_H 