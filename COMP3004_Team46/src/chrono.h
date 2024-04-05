#ifndef CHRONO_H
#define CHRONO_H

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>

class Chrono
{
public:
    Chrono();

    void setChrono(QDateTime dtEdit);
    QDateTime getChrono();
    void saveChrono();
    void readChrono();
    QDateTime retrieveChrono(); //will return what should be current Date/Time based on what was inputed and how much time has passed

private:
    QDateTime dt;
    QDateTime currentdt;
};

#endif // CHRONO_H
