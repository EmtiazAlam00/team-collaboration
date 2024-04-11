#include "session.h"

Session::Session( const QDateTime& startTime)
    :  startTime(startTime), beforeBaselines(7,0.0), afterBaselines(7, 0.0){

}

Session::~Session(){

}

void Session::print()
{
    qDebug()<< startTime.toString();
    qDebug()<< "Befores: "<< beforeBaselines;
    qDebug()<< "Afters: " << afterBaselines;
}

//getters


QDateTime Session::getStartTime() { return startTime;}
float Session::getBaselineAfter(int num){
    return afterBaselines[num];
}

void Session::setBefore(int i, float baselineVal)
{
    beforeBaselines[i] = baselineVal;
}

void Session::setAfter(int i, float baselineVal)
{
    afterBaselines[i] = baselineVal;
}


