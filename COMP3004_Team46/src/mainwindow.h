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

enum class DeviceState {
    Initialization,
    Menu,
    SessionPreparation,
    SessionActive,
    ContactLoss,
    DeliverTreatment,
    SessionPaused,
    SessionCompletion,
    SessionLogViewing,
    TimeDateSetting,
    Shutdown
};

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

    // battery slots
    void startDrainBattery();
    void stopDrainBattery();
    void chargeBattery();
    void updateBatteryLevel(); // specifically updating battery ui displays
    void lowerBatteryLevel();  // drop battery level by 10 to have some testing functionality

    //  led slots
    void flashRedLED();
    void flashGreenLED();

    // temporary led buttons
    void blueOnClicked();
    void redOnClicked();
    void greenOnClicked();
    void blueOffClicked();
    void redOffClicked();
    void greenOffClicked();

    void contactInitButtonClicked();
    void contactLostButtonClicked();
    void applyTreatmentButtonClicked();


private:
    Ui::MainWindow *ui;
    // led
    QTimer flashRedTimer;
    QTimer flashGreenTimer;
    bool redLedState = false; // false means off, true means on
    bool greenLedState = false; // false means off, true means on

    void setLedState(QPushButton* led, const QString& state);
    void flashLED(QPushButton* led, QTimer* timer, const QString& color);
    // battery
    Battery battery;
    QTimer batteryDrainTimer;


};
#endif // MAINWINDOW_H
