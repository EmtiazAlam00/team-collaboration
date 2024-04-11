#ifndef BATTERY_H
#define BATTERY_H

#include <algorithm>

/* Class Purpose: Manages the battery level and drain rate for a device.
 * It allows the battery to be drained at different rates depending on the device's activity state,
 * recharged, and provides the current battery level.
 *
 * Data Members:
 * - double batteryLevel: Represents the current battery level as a percentage (0 to 100).
 * - double drainRate: The current rate at which the battery is draining.
 * - static constexpr double DRAIN_RATE_IDLE: The drain rate when the device is in idle mode.
 * - static constexpr double DRAIN_RATE_ACTIVE: The drain rate when the device is active.
 *
 * Class Functions:
 * - Battery(): Constructor that initializes the battery level to 100% and sets the initial drain rate to idle.
 * - void rechargeBattery(): Recharges the battery to full (100%).
 * - void setBatteryLevel(double newLevel): Sets the battery level to a specific value, ensuring it remains within 0 to 100%.
 * - void drainBattery(): Decreases the battery level by the current drain rate, ensuring it does not go below 0%.
 * - double getBatteryLevel() const: Returns the current battery level.
 * - void setDrainRateIdle(): Sets the drain rate to the idle mode's rate.
 * - void setDrainRateActive(): Sets the drain rate to the active mode's rate.
 */

class Battery {
public:
    Battery();

    void rechargeBattery(); // Internally calls setBatteryLevel(100)
    void setBatteryLevel(double newLevel); // Set the battery level to a specific value, ensuring it remains within bounds
    void drainBattery(); // Decreases the battery level based on the current drain rate
    double getBatteryLevel() const; // Returns the current battery level
    void setDrainRateIdle(); // Sets the drain rate to the predefined idle rate
    void setDrainRateActive(); // Sets the drain rate to the predefined active rate

private:
    double batteryLevel;
    double drainRate; // Current drain rate
    static constexpr double DRAIN_RATE_IDLE = 0.01;
    static constexpr double DRAIN_RATE_ACTIVE = 1.0;
};

#endif // BATTERY_H
