#include "BolusPlan.h"
#include <sstream>

BolusPlan::BolusPlan(float carbBolus, float correctionBolus, float immediateDose, 
                     float extendedDose, int duration)
    : carbBolus(carbBolus), correctionBolus(correctionBolus),
      immediateDose(immediateDose), extendedDose(extendedDose),
      duration(duration) {
    totalBolus = immediateDose + extendedDose;
}

void BolusPlan::recalculate(float bg, float carbs, float iob, float icr) {

    carbBolus = carbs / icr;  // Calculate using insulin-to-carb ratio
    correctionBolus = (bg - 100) / 50.0f;  // Assuming 50mg/dL per unit
    totalBolus = carbBolus + correctionBolus - iob;
}

std::string BolusPlan::getSummary() const {
    std::stringstream ss;
    ss << "Bolus Summary:\n"
       << "Carb Bolus: " << carbBolus << " units\n"
       << "Correction Bolus: " << correctionBolus << " units\n"
       << "Total Bolus: " << totalBolus << " units\n"
       << "Immediate Dose: " << immediateDose << " units\n"
       << "Extended Dose: " << extendedDose << " units\n"
       << "Duration: " << duration << " minutes";
    return ss.str();
} 