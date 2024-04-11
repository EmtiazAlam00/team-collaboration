#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QDateTime>
#include <QDebug>

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
