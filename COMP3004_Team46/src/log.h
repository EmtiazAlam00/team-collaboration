#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QDateTime>
#include <QTime>



class Log
{
public:
    Log(const QString& name, const QDateTime& startTime, const int duration, const int baslineHZ);

    QString getName();
    QDateTime getStartTime();
    int getDuration();
    int getBaselineHZ();

    QString toString();

private:
    QString name;
    QDateTime startTime;
    int duration;
    int baselineHZ;
};

#endif // LOG_H
