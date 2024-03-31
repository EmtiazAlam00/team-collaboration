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
    bool success= query.exec("CREATE TABLE IF NOT EXISTS sessions (session_ID INTEGER PRIMARY KEY AUTOINCREMENT,date_time TEXT NOT NULL, beforeBaseline_site1 FLOAT, afterBaseline_site1 FLOAT, beforeBaseline_site2 FLOAT, afterBaseline_site2 FLOAT, beforeBaseline_site3 FLOAT, afterBaseline_site3 FLOAT, beforeBaseline_site4 FLOAT, afterBaseline_site4 FLOAT, beforeBaseline_site5 FLOAT, afterBaseline_site5 FLOAT, beforeBaseline_site6 FLOAT, afterBaseline_site6 FLOAT, beforeBaseline_site7 FLOAT, afterBaseline_site7 FLOAT, beforeBaseline_site8 FLOAT, afterBaseline_site8 FLOAT, beforeBaseline_site9 FLOAT, afterBaseline_site9 FLOAT, beforeBaseline_site10 FLOAT, afterBaseline_site10 FLOAT, beforeBaseline_site11 FLOAT, afterBaseline_site11 FLOAT, beforeBaseline_site12 FLOAT, afterBaseline_site12 FLOAT, beforeBaseline_site13 FLOAT, afterBaseline_site13 FLOAT, beforeBaseline_site14 FLOAT, afterBaseline_site14 FLOAT, beforeBaseline_site15 FLOAT, afterBaseline_site15 FLOAT, beforeBaseline_site16 FLOAT, afterBaseline_site16 FLOAT, beforeBaseline_site17 FLOAT, afterBaseline_site17 FLOAT, beforeBaseline_site18 FLOAT, afterBaseline_site18 FLOAT, beforeBaseline_site19 FLOAT, afterBaseline_site19 FLOAT, beforeBaseline_site20 FLOAT, afterBaseline_site20 FLOAT, beforeBaseline_site21 FLOAT, afterBaseline_site21 FLOAT);");

    if (!success) {
        qInfo() << "Error creating table:" << query.lastError().text();
        // Handle the error as needed
    } else {
        qInfo() << "Table created successfully";
    }



    return neuresetDB.commit();

}

bool DBManager::addSession(const QDateTime& date_time, float* baselineBefores, float* baselineAfters){

    neuresetDB.transaction();

    QSqlQuery query;
    if(!isValidRecord("session", date_time, baselineBefores, baselineAfters)){
        //qInfo()<<"This is where it fails";
        return false;
    }
    query.prepare("INSERT INTO sessions (date_time, beforeBaseline_site1, afterBaseline_site1, beforeBaseline_site2, afterBaseline_site2, beforeBaseline_site3, afterBaseline_site3, beforeBaseline_site4, afterBaseline_site4, beforeBaseline_site5, afterBaseline_site5, beforeBaseline_site6, afterBaseline_site6, beforeBaseline_site7, afterBaseline_site7, beforeBaseline_site8, afterBaseline_site8, beforeBaseline_site9, afterBaseline_site9, beforeBaseline_site10, afterBaseline_site10, beforeBaseline_site11, afterBaseline_site11, beforeBaseline_site12, afterBaseline_site12, beforeBaseline_site13, afterBaseline_site13, beforeBaseline_site14, afterBaseline_site14, beforeBaseline_site15, afterBaseline_site15, beforeBaseline_site16, afterBaseline_site16, beforeBaseline_site17, afterBaseline_site17, beforeBaseline_site18, afterBaseline_site18, beforeBaseline_site19, afterBaseline_site19, beforeBaseline_site20, afterBaseline_site20, beforeBaseline_site21, afterBaseline_site21) VALUES (:date_time, :beforeBaseline_site1, :afterBaseline_site1, :beforeBaseline_site2, :afterBaseline_site2, :beforeBaseline_site3, :afterBaseline_site3, :beforeBaseline_site4, :afterBaseline_site4, :beforeBaseline_site5, :afterBaseline_site5, :beforeBaseline_site6, :afterBaseline_site6, :beforeBaseline_site7, :afterBaseline_site7, :beforeBaseline_site8, :afterBaseline_site8, :beforeBaseline_site9, :afterBaseline_site9, :beforeBaseline_site10, :afterBaseline_site10, :beforeBaseline_site11, :afterBaseline_site11, :beforeBaseline_site12, :afterBaseline_site12, :beforeBaseline_site13, :afterBaseline_site13, :beforeBaseline_site14, :afterBaseline_site14, :beforeBaseline_site15, :afterBaseline_site15, :beforeBaseline_site16, :afterBaseline_site16, :beforeBaseline_site17, :afterBaseline_site17, :beforeBaseline_site18, :afterBaseline_site18, :beforeBaseline_site19, :afterBaseline_site19, :beforeBaseline_site20, :afterBaseline_site20, :beforeBaseline_site21, :afterBaseline_site21 );");
    query.bindValue(":date_time", date_time.toString(DATE_FORMAT));

    for(int i =1; i <=21; ++i){
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
bool DBManager::isValidRecord(const QString& recordType, const QDateTime& time, float* baselineBefore, float* baselineAfter){
    return time.isValid();
}

bool DBManager::deleteSessions(){
    QSqlQuery query;
    query.prepare("DELETE FROM sessions");
    return query.exec();
}
