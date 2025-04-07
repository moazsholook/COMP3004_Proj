#include "SystemLog.h"
#include <iostream>
#include <ctime>

void SystemLog::log(const std::string& event) {
    time_t now = time(0);
    char* dt = ctime(&now);
    std::cout << "[" << dt << "] " << event << std::endl;
} 