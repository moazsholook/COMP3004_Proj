#include "Profile.h"

Profile::Profile(const QString& name, float basalRate, float icr, float cf, float targetBG)
    : name(name.toStdString())
    , basalRate(basalRate)
    , icr(icr)
    , correctionFactor(cf)
    , targetBG(targetBG)
{
}

std::map<std::string, float> Profile::getParameters() const {
    std::map<std::string, float> params;
    params["basalRate"] = basalRate;
    params["icr"] = icr;
    params["correctionFactor"] = correctionFactor;
    params["targetBG"] = targetBG;
    return params;
} 