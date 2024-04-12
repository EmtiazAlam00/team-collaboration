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

    bool success= query.exec("CREATE TABLE IF NOT EXISTS sessions (session_ID INTEGER PRIMARY KEY AUTOINCREMENT,date_time TEXT NOT NULL, beforeBaseline_site1 FLOAT, afterBaseline_site1 FLOAT, beforeBaseline_site2 FLOAT, afterBaseline_site2 FLOAT, beforeBaseline_site3 FLOAT, afterBaseline_site3 FLOAT, beforeBaseline_site4 FLOAT, afterBaseline_site4 FLOAT, beforeBaseline_site5 FLOAT, afterBaseline_site5 FLOAT, beforeBaseline_site6 FLOAT, afterBaseline_site6 FLOAT, beforeBaseline_site7 FLOAT, afterBaseline_site7 FLOAT);");

    if (!success) {
        qInfo() << "Error creating table:" << query.lastError().text();
        // Handle the error as needed
    } else {
        qInfo() << "Table created successfully";
    }

    return neuresetDB.commit();

}

bool DBManager::addSession(Session* s){

    neuresetDB.transaction();

    QSqlQuery query;
    QString dateTime = (s->getStartTime()).toString(DATE_FORMAT);
    //QDateTime dTdesiredFormat = QDateTime::fromString(dateTime, DATE_FORMAT);

    if(!isValidRecord("session", s->getStartTime())){
        qInfo()<<"This is where it fails";
        return false;
    }
    query.prepare("INSERT INTO sessions (date_time, beforeBaseline_site1, afterBaseline_site1, beforeBaseline_site2, afterBaseline_site2, beforeBaseline_site3, afterBaseline_site3, beforeBaseline_site4, afterBaseline_site4, beforeBaseline_site5, afterBaseline_site5, beforeBaseline_site6, afterBaseline_site6, beforeBaseline_site7, afterBaseline_site7) VALUES (:date_time, :beforeBaseline_site1, :afterBaseline_site1, :beforeBaseline_site2, :afterBaseline_site2, :beforeBaseline_site3, :afterBaseline_site3, :beforeBaseline_site4, :afterBaseline_site4, :beforeBaseline_site5, :afterBaseline_site5, :beforeBaseline_site6, :afterBaseline_site6, :beforeBaseline_site7, :afterBaseline_site7)");

    query.bindValue(":date_time", dateTime);
    QVector<float> baselineBefores = s->getBeforeBaselines();
    QVector<float> baselineAfters = s->getAfterBaselines();
    for(int i=1; i <=7; ++i){
        QString b= ":beforeBaseline_site" + QString::number(i);
        QString a= ":afterBaseline_site" + QString::number(i);

        query.bindValue(b, baselineBefores[i-1]);
        query.bindValue(a, baselineAfters[i-1]);
        qDebug()<< b << baselineBefores[i-1];
        qDebug()<< a << baselineAfters[i-1];
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
bool DBManager::isValidRecord(const QString& recordType, const QDateTime& time){
    return time.isValid();
}

bool DBManager::deleteSessions(){
    QSqlQuery query;
    query.prepare("DELETE FROM sessions");
    return query.exec();
}

QVector<QString> DBManager::getSessionsHistoryDevice(){
    QSqlQuery query;
    QVector<QString> qSessionList;
    neuresetDB.transaction();

    query.prepare("SELECT date_time FROM sessions");
    query.exec();

    while (query.next()) {
        QDateTime qDt = QDateTime::fromString(query.value(0).toString(), DATE_FORMAT);
        QString dt = qDt.toString(); // hopefully this is the date string in the correct format
        qDebug()<<dt;
        qSessionList.push_back(dt);
    }
    qInfo() << "Got Sessions History for the DEVICE";
    return qSessionList;
}

QVector<QString> DBManager::getSessionsHistoryPC(){
    QSqlQuery query;
    QVector<QString> qSessionList;
    neuresetDB.transaction();

    query.prepare("SELECT * FROM sessions");
    query.exec();

    while (query.next()) {
        QDateTime qDt = QDateTime::fromString(query.value(1).toString(), DATE_FORMAT);
        QString dt = qDt.toString(); 
        QString id = query.value(0).toString();

        QString s = id+"\t"+dt;
        for (int i=1; i<15; i++){   // adding the hz to the string
            QString temp_hz = query.value(i).toString();
            double hz_double = temp_hz.toDouble();
            QString hz = QString::number(hz_double, 'f', 2);
            s = s + "\t\t" + hz;
        }
        qSessionList.push_back(s);
    }
    qInfo() << "Got Sessions History for the PC";
    return qSessionList;
}
