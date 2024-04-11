#include "chrono.h"

Chrono::Chrono()
{

}

void Chrono::setChrono(QDateTime dtEdit) {
    dt = dtEdit;
}

QDateTime Chrono::getChrono() {
    return dt;
}

void Chrono::saveChrono() {
    currentdt = QDateTime::currentDateTime();
    QFile file("chrono.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "Error: Failed to open file for writing.";
        return;
    }
    QTextStream out(&file);
    
    out << "Stored QDateTime: " << dt.toString(Qt::ISODate) << Qt::endl;
    out << "Stored Current QDateTime: " << currentdt.toString(Qt::ISODate) << Qt::endl;

    file.seek(0);
    file.close();
}

void Chrono::readChrono() {
    QFile file("chrono.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Failed to open file for reading.";
        qDebug() << "Error: Using system date/time.";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << line;
    }

    file.close();
}

QDateTime Chrono::retrieveChrono() {
    QDateTime storeddt;
    QDateTime storedCurrentdt;

    QFile file("chrono.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QDateTime::currentDateTime();
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith("Stored QDateTime: ")) {
            QStringList dateTimeParts = line.split(": ");
            if (dateTimeParts.length() == 2) {
                QString dateTimeString = dateTimeParts.at(1);
                storeddt = QDateTime::fromString(dateTimeString, Qt::ISODate);
            }
        }
        if (line.startsWith("Stored Current QDateTime: ")) {
            QStringList dateTimeParts = line.split(": ");
            if (dateTimeParts.length() == 2) {
                QString dateTimeString = dateTimeParts.at(1);
                storedCurrentdt = QDateTime::fromString(dateTimeString, Qt::ISODate);
            }
        }
    }

    if (storeddt.isNull() || storedCurrentdt.isNull()) {
        file.close();
        return QDateTime::currentDateTime();
    }

    int offset = storedCurrentdt.secsTo(storeddt);
    QDateTime realCurrentdt = QDateTime::currentDateTime().addSecs(offset);
    file.close();
    return realCurrentdt;
}
