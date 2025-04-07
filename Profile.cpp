#include "Profile.h"

Profile::Profile(const std::string& name, float basalRate, float icr, 
                float correctionFactor, float targetBG)
    : name(name), basalRate(basalRate), icr(icr),
      correctionFactor(correctionFactor), targetBG(targetBG) {}

std::map<std::string, float> Profile::getParameters() const {
    std::map<std::string, float> params;
    params["basalRate"] = basalRate;
    params["icr"] = icr;
    params["correctionFactor"] = correctionFactor;
    params["targetBG"] = targetBG;
    return params;
} 