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

    bool addSession(Session* sessionInfo);
    bool deleteSessions();
//    Session* getSession(const QDateTime& date_time);
    QVector<QString> getSessionsHistoryDevice();
    QVector<QString> getSessionsHistoryPC();

private:
    QSqlDatabase neuresetDB;
    bool DBInit();
    bool isValidRecord(const QString& recordType, const QDateTime& time);
};

#endif // DBMANAGER_H
