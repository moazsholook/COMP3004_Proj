#include "Profile.h"

// Constructor: Initializes all parameters of a user's insulin profile
// - name: User-friendly name for the profile (converted from QString to std::string)
// - basalRate: Baseline insulin delivery rate (units per hour)
// - icr: Insulin-to-Carb Ratio (grams of carbohydrate covered per unit of insulin)
// - cf: Correction Factor (BG drop per unit of insulin)
// - targetBG: Target blood glucose level
Profile::Profile(const QString& name, float basalRate, float icr, float cf, float targetBG)
    : name(name.toStdString())  // Convert QString to std::string for internal storage
    , basalRate(basalRate)
    , icr(icr)
    , correctionFactor(cf)
    , targetBG(targetBG)
{
}

// Returns all numeric profile parameters in a std::map for easy access/display
// Useful for logging, serialization, or debugging UI
std::map<std::string, float> Profile::getParameters() const {
    std::map<std::string, float> params;
    params["basalRate"] = basalRate;
    params["icr"] = icr;
    params["correctionFactor"] = correctionFactor;
    params["targetBG"] = targetBG;
    return params;
}
