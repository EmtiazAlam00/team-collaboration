#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>

class DBManager
{
public:

    static const QString DATABASE_PATH;
    const QString DATE_FORMAT = "yyyy-MM-dd hh:mm";

    DBManager();

    bool addSession(const QDateTime& date_time,  float* baselineBefore, float* baselineAfter);
    bool deleteSessions();

private:
    QSqlDatabase neuresetDB;
    bool DBInit();
    bool isValidRecord(const QString& recordType, const QDateTime& time, float* baselineBefore, float* baselineAfter);
};

#endif // DBMANAGER_H
