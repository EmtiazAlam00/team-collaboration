#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTimer>

#include "battery.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void powerButtonClicked();
    void upButtonClicked();
    void downButtonClicked();
    void pauseButtonClicked();
    void startButtonClicked();
    void stopButtonClicked();
    void menuButtonClicked();

    void setLedState(QPushButton* led, const QString& state);

    // battery slots
    void startDrainBattery();
    void stopDrainBattery();
    void chargeBattery();
    void updateBatteryLevel();


    // temporary led buttons
    void blueOnClicked();
    void redOnClicked();
    void greenOnClicked();
    void blueOffClicked();
    void redOffClicked();
    void greenOffClicked();


private:
    Ui::MainWindow *ui;

    // battery
    Battery battery;
    QTimer batteryDrainTimer;


};
#endif // MAINWINDOW_H
