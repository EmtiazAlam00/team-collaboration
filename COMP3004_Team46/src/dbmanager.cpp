#include "dbmanager.h"

const QString DBManager::DATABASE_PATH = "/database/neureset.db";

DBManager::DBManager(){
    neuresetDB = QSqlDatabase::addDatabase("QSQLITE");
    neuresetDB.setDatabaseName("neureset.db");

    if (!neuresetDB.open()) {
        throw "Error: Database could not be opened";
    }

    if (!DBInit()) {
        throw "Error: Database could not be initialized";
    }
}

bool DBManager::DBInit(){
    neuresetDB.transaction();

    QSqlQuery query;
    //Sessions table- stores the date time, the before and after baselines for all 21 sites
    bool success= query.exec("CREATE TABLE IF NOT EXISTS sessions (session_ID INTEGER PRIMARY KEY AUTOINCREMENT,date_time TEXT NOT NULL, length INTEGER NOT NULL, beforeBaseline_site1 FLOAT, afterBaseline_site1 FLOAT, beforeBaseline_site2 FLOAT, afterBaseline_site2 FLOAT, beforeBaseline_site3 FLOAT, afterBaseline_site3 FLOAT, beforeBaseline_site4 FLOAT, afterBaseline_site4 FLOAT, beforeBaseline_site5 FLOAT, afterBaseline_site5 FLOAT, beforeBaseline_site6 FLOAT, afterBaseline_site6 FLOAT, beforeBaseline_site7 FLOAT, afterBaseline_site7 FLOAT);");

    if (!success) {
        qInfo() << "Error creating table:" << query.lastError().text();
        // Handle the error as needed
    } else {
        qInfo() << "Table created successfully";
    }



    return neuresetDB.commit();

}

bool DBManager::addSession(const QDateTime& date_time, int length, float* baselineBefores, float* baselineAfters){

    neuresetDB.transaction();

    QSqlQuery query;
    if(!isValidRecord("session", date_time, length, baselineBefores, baselineAfters)){
        //qInfo()<<"This is where it fails";
        return false;
    }
    query.prepare("INSERT INTO sessions (date_time, length, beforeBaseline_site1, afterBaseline_site1, beforeBaseline_site2, afterBaseline_site2, beforeBaseline_site3, afterBaseline_site3, beforeBaseline_site4, afterBaseline_site4, beforeBaseline_site5, afterBaseline_site5, beforeBaseline_site6, afterBaseline_site6, beforeBaseline_site7, afterBaseline_site7);");
    query.bindValue(":date_time", date_time.toString(DATE_FORMAT));

    for(int i=2; i <=16; ++i){
        QString b= ":beforeBaseline_site" + QString::number(i);
        QString a= ":afterBaseline_site" + QString::number(i);
        query.bindValue(b, baselineBefores[i-1]);
        query.bindValue(a, baselineAfters[i-1]);
    }

    bool success = query.exec();
    if (success) {
            neuresetDB.commit(); // Commit the transaction if the query succeeds
        } else {
            qInfo() << "Query execution failed:" << query.lastError().text();
            neuresetDB.rollback(); // Rollback the transaction if the query fails
        }

    return success;

}


//TO DO if we want an extra check
bool DBManager::isValidRecord(const QString& recordType, const QDateTime& time, int length, float* baselineBefore, float* baselineAfter){
    return time.isValid();
}

bool DBManager::deleteSessions(){
    QSqlQuery query;
    query.prepare("DELETE FROM sessions");
    return query.exec();
}

Session* DBManager::getSession(const QDateTime& date_time){
    QSqlQuery query;
    neuresetDB.transaction();

    query.prepare("SELECT date_time as session_ID, length FROM sessions");
    query.bindValue(":date_time", date_time.toString(DATE_FORMAT));
    query.exec();

    QDateTime dt = QDateTime::fromString(query.value(0).toString(), DATE_FORMAT);
    int id = query.value(1).toInt();
    int length = query.value(2).toInt();
    Session* s = new Session(id, length, dt);

    qInfo() << "Got the session from "<< date_time;
    return s;
}
QVector<QStringList> DBManager::getSessionsHistoryDevice(){
    QSqlQuery query;
    QVector<QStringList> qSessionList;
    neuresetDB.transaction();

    query.prepare("SELECT session_ID, date_time, length FROM sessions");
    query.exec();

    while (query.next()) {
        QDateTime qDt = QDateTime::fromString(query.value(0).toString(), DATE_FORMAT);
        QString dt = qDt.toString(); // hopefully this is the date string in the correct format
        QString id = query.value(1).toString();
        QString length = query.value(2).toString();
        // may cause an error because it's not dynamic aka no 'new'
        QStringList s = {id, length, dt};
        qSessionList.push_back(s);
    }
    qInfo() << "Got Sessions History for the DEVICE";
    return qSessionList;
}
QVector<QStringList> DBManager::getSessionsHistoryPC(){
    QSqlQuery query;
    QVector<QStringList> qSessionList;
    neuresetDB.transaction();

    query.prepare("SELECT * FROM sessions");
    query.exec();

    while (query.next()) {
        QDateTime qDt = QDateTime::fromString(query.value(0).toString(), DATE_FORMAT);
        QString dt = qDt.toString(); // hopefully this is the date string in the correct format
        QString id = query.value(1).toString();
        QString length = query.value(2).toString();
        // may cause an error because it's not dynamic aka no 'new'
        QStringList s = {id, length, dt};
        for (int i=2; i<23; i++){   // adding the hz to the string
            QString hz = query.value(i).toString();
            s.append(hz);
        }
        qSessionList.push_back(s);
    }
    qInfo() << "Got Sessions History for the PC";
    return qSessionList;
}
