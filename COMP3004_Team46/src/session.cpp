#include "session.h"

Session::Session(int ID, int l, QDateTime date_time){
    id = ID;
    length = l;
    dt = date_time;
    timer = new QTimer(this);
}

Session::~Session(){
    delete timer;
}

//getters
int Session:: getID() { return id;}
QTimer* Session:: getTimer() { return timer;}
int Session:: getLength() { return length;}
QDateTime Session:: getStartTime() { return dt;}

