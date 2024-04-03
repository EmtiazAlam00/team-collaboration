#ifndef BATTERY_H
#define BATTERY_H

class Battery {
public:
    Battery();

    void rechargeBattery(); //internally call setBatteryLevel(100)
    void setBatteryLevel(double newLevel); // Set the battery level to a specific value
    void drainBattery(); // Use the current drain rate
    double getBatteryLevel() const;
    void setDrainRateIdle();
    void setDrainRateActive();

private:
    double batteryLevel;
    double drainRate; // Current drain rate
    static constexpr double DRAIN_RATE_IDLE = 0.01;
    static constexpr double DRAIN_RATE_ACTIVE = 0.05;
};

#endif // BATTERY_H
