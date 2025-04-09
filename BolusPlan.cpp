#include "BolusPlan.h"
#include <sstream>  // for building the summary string efficiently

// Constructor initializes all dose parameters and duration
// Also calculates the total bolus as the sum of immediate and extended doses
BolusPlan::BolusPlan(float carbBolus, float correctionBolus, float immediateDose, 
                     float extendedDose, int duration)
    : carbBolus(carbBolus), correctionBolus(correctionBolus),
      immediateDose(immediateDose), extendedDose(extendedDose),
      duration(duration) {
    totalBolus = immediateDose + extendedDose;
}

// Recalculates the bolus plan based on updated patient data
// bg: current blood glucose level
// carbs: amount of carbs to be consumed
// iob: insulin on board (already active in the body)
// icr: insulin-to-carb ratio (how many grams of carbs one unit of insulin covers)
void BolusPlan::recalculate(float bg, float carbs, float iob, float icr) {
    // Calculate carb bolus: carbs divided by insulin-to-carb ratio
    carbBolus = carbs / icr;

    // Calculate correction bolus: (bg - target) / sensitivity factor
    // Here, target BG is 100 and sensitivity factor is assumed to be 50
    correctionBolus = (bg - 100) / 50.0f;

    // Total bolus is the sum of both types of bolus minus insulin already on board
    totalBolus = carbBolus + correctionBolus - iob;
}

// Returns a human-readable summary of the bolus plan
// Useful for debugging, logging, or displaying to users
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
