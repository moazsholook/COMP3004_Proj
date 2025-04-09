#ifndef PROFILE_H
#define PROFILE_H

#include <string>
#include <map>
#include <QString>

class Profile {
private:
    std::string name;
    float basalRate;
    float icr;  
    float correctionFactor;
    float targetBG;

public:
    Profile(const QString& name, float basalRate, float icr, float cf, float targetBG);
    
    std::map<std::string, float> getParameters() const;
    
    // Getters
    QString getName() const { return QString::fromStdString(name); }
    float getBasalRate() const { return basalRate; }
    float getICR() const { return icr; }
    float getCorrectionFactor() const { return correctionFactor; }
    float getTargetBG() const { return targetBG; }
    
    // Setters
    void setBasalRate(float rate) { basalRate = rate; }
    void setICR(float ratio) { icr = ratio; }
    void setCorrectionFactor(float factor) { correctionFactor = factor; }
    void setTargetBG(float bg) { targetBG = bg; }
};

#endif 