#ifndef CHRONO_H
#define CHRONO_H

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>

/* Class Purpose: Manages and stores the internal clock date/time for a device.
 * It allows the date time to be set, saved, and retrieved.
 *
 * Data Members:
 * - QDateTime dt: Represents the set date/time.
 * - QDateTime currentdt: Represents the current date/time based on how much time has passed.
 *
 * Class Functions:
 * - Chrono(): Constructor that initializes the chrono object.
 * - void setChrono(QDateTime dtEdit): Sets the dt variable to the dtEdit.
 * - QDateTime getChrono(): Returns the dt variable.
 * - void saveChrono(): Saves the current machine date/time and the dt variable to the text file "chrono.txt".
 * - void readChrono(): Reads the text file "chrono.txt" and outputs the text.
 * - QDateTime retrieveChrono(): Returns the set QDateTime from the file and adds how long since it was set.
 */

class Chrono
{
public:
    Chrono();

    void setChrono(QDateTime dtEdit);
    QDateTime getChrono();
    void saveChrono();
    void readChrono();
    QDateTime retrieveChrono();

private:
    QDateTime dt;
    QDateTime currentdt;
};

#endif // CHRONO_H
