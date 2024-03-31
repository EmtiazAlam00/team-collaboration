#include "battery.h"
#include <algorithm>

Battery::Battery() : batteryLevel(100.0) {
}

void Battery::rechargeBattery() {
    setBatteryLevel(100.0);
}

void Battery::setBatteryLevel(double newLevel) {
    batteryLevel = std::max(0.0, std::min(newLevel, 100.0));
}

void Battery::drainBattery() {
    setBatteryLevel(batteryLevel - DRAIN_RATE);
}

double Battery::getBatteryLevel() const {
    return batteryLevel;
}
