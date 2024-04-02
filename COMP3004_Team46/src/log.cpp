#include "log.h"

Log::Log(const QString& name, const QDateTime& startTime, const int duration, const int baselineHZ)
{
    if(startTime.isValid()){
        this->name = name;
        this->startTime = startTime;
        this->duration = duration;
        this->baselineHZ = baselineHZ;

    }
}

//QString Log::toString(){ }

//getters
QString Log:: getName() { return name; }
QDateTime Log:: getStartTime() { return startTime; }
int Log:: getDuration() { return duration; }
int Log:: getBaselineHZ() { return  baselineHZ; }

//setters

