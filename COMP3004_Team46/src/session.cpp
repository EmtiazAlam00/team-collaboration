#include "session.h"

Session::Session(int ID, int l, QDateTime date_time){
    id = ID;
    length = l;
    dt = date_time;
}

Session::~Session(){

}

//getters
int Session:: getID() { return id;}
int Session:: getLength() { return length;}
QDateTime Session:: getStartTime() { return dt;}
