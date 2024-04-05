#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QDateTime>

class Session: public QObject{

    Q_OBJECT

    public:
        Session(int ID, int l, QDateTime dt);
        ~Session();
        int getID();
        QTimer* getTimer();
        int getLength();
        QDateTime getStartTime();

    private:
        int id;
        int length;
        QDateTime dt;
        QTimer* timer;

};

#endif // SESSION_H
