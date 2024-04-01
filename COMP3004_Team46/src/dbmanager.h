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

class DBManager
{
public:

    static const QString DATABASE_PATH;
    const QString DATE_FORMAT = "yyyy-MM-dd hh:mm";

    DBManager();

    bool addSession(const QDateTime& date_time, int length,  float* baselineBefore, float* baselineAfter);
    bool deleteSessions();
    Session* getSession(const QDateTime& date_time);
    QVector<QStringList> getSessionsHistoryDevice();
    QVector<QStringList> getSessionsHistoryPC();

private:
    QSqlDatabase neuresetDB;
    bool DBInit();
    bool isValidRecord(const QString& recordType, const QDateTime& time, int length, float* baselineBefore, float* baselineAfter);
};

#endif // DBMANAGER_H
