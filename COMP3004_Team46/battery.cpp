#include "battery.h"
#include <algorithm>

Battery::Battery() : batteryLevel(100.0), drainRate(DRAIN_RATE_IDLE) {
    // device starts in idle state
}

void Battery::rechargeBattery() {
    setBatteryLevel(100.0);
}

void Battery::setBatteryLevel(double newLevel) {
    batteryLevel = std::max(0.0, std::min(newLevel, 100.0));
}

void Battery::drainBattery() {
    setBatteryLevel(batteryLevel - drainRate);
}

double Battery::getBatteryLevel() const {
    return batteryLevel;
}

void Battery::setDrainRateIdle() {
    drainRate = DRAIN_RATE_IDLE;
}

void Battery::setDrainRateActive() {
    drainRate = DRAIN_RATE_ACTIVE;
}


