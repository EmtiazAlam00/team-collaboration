#ifndef BATTERY_H
#define BATTERY_H

class Battery {
public:
    Battery();

    void rechargeBattery(); //internally call setBatteryLevel(100)
    void setBatteryLevel(double newLevel); // Set the battery level to a specific value
    void drainBattery();
    double getBatteryLevel() const;

private:
    double batteryLevel;
    static constexpr double DRAIN_RATE = 0.05;
};

#endif // BATTERY_H
