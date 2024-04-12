#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QDateTime>
#include <QDebug>

/* Class Purpose: Stores the data from a LENS treatment session on the Nearest device. 
The data includes the start time, and the before and after baseline average frequencies. 
 
 Data Members:
- QDateTime startTime: Stores the session’s start time.
- QVector<float> beforeBaselines: Store the before baseline average frequencies in a vector.
- QVector<float> afterBaselines: Store the after baseline average frequencies in a vector.
  
 
 Class Functions:
- Session(const QDateTime& startTime): Initializes the session’s starting time and the before and 
after baseline average frequency vectors are set to size 7.
- print(): Prints the session’s start time, before and after baseline average frequencies values.
- getStartTime(): Returns the session’s start time.
- getBaselineAfter(int num): Returns a float value at index num from the after baseline average frequency vector.
- setBefore(int i, float baselineVal): Sets the baselineVal at index i in the before baseline average frequency vector.
- setAfter(int i, float baselineVal): Sets the baselineVal at index i in the after baseline average frequency vector.
  
*/

class Session: public QObject{

    Q_OBJECT

    public:
        Session(const QDateTime& startTime);
        ~Session();
        void print();
        //getters
        QDateTime getStartTime();
        float getBaselineAfter(int); // get baseline based on 1-7 and "after"/"before"
        void setBefore(int i, float baselineVal);
        void setAfter(int i, float baselineVal);
        const QVector<float>& getBeforeBaselines(){ return beforeBaselines;}
        const QVector<float>& getAfterBaselines() {return afterBaselines;}
        //setter
//        void setBaseline(int, QString&, const float);


    private:

        QDateTime startTime;
        QVector<float> beforeBaselines;
        QVector<float> afterBaselines;


};

#endif // SESSION_H
