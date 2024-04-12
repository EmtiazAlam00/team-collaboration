#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include "session.h"
#include <QStringList>
#include <QVector>

/* Class Purpose: Manages the neureset database and the sessions table within it.
 * It allows sessions to be inserted, and deleted as well as the retrieval of the pc and device session histories.
 *
 * Data Members:
 * - QSqlDatabase neuresetDB: Represents the database we create tables in.
 *
 * Class Functions:
 * - DBManager(): Constructor that initializes and opens the database.
 * - void DBInit(): Creates the sessions table in the database.
 * - bool addSession(Session* s): Adds the session pointer’s values into the database’s table for them to be stored.
 * - bool deleteSessions(): Deletes all the data in the sessions table within the database.
 * - bool isValidRecord(const QString& recordType, const QDateTime& time): Checks if the time is valid.
 * - QVector<QString> getSessionsHistoryDevice(): Returns a vector of strings of each date from all of sessions in 
 * the sessions table in the database.
 * - QVector<QString> getSessionsHistoryPC(): Returns a vector of strings of each date and all the before and after 
 * baselines from all of sessions in the sessions table in the database.
 * 
 * Consts:
 * - static const QString DATABASE_PATH: Path to the neureset database.
 * - const QString DATE_FORMAT: Format of the ideal date and time in a string for the date_time attribute in the sessions table.
 */

class DBManager
{
public:

    static const QString DATABASE_PATH;
    const QString DATE_FORMAT = "yyyy-MM-dd hh:mm";

    DBManager();
    bool addSession(Session* sessionInfo);
    bool deleteSessions();
    QVector<QString> getSessionsHistoryDevice();
    QVector<QString> getSessionsHistoryPC();

private:
    QSqlDatabase neuresetDB;
    bool DBInit();
    bool isValidRecord(const QString& recordType, const QDateTime& time);
};

#endif // DBMANAGER_H
