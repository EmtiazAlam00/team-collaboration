#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QPushButton>

#include <QMainWindow>
#include <QSpinBox>
#include <QListWidget>
#include <QGraphicsView>
#include <QTime>
#include <QStatusBar>
#include <QVector>
#include <QtGlobal>

#include "menu.h"

#include <QTimer>

#include "battery.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

enum class DeviceState {
    Initialization,//unknown?
    Menu,//unknown?
    SessionPreparation, //unknown?
    SessionActive,      //blue light
    AppliedToScalp,      // if admin is selected true
    ContactLoss,        // red flash
    DeliverTreatment,   // green flash
    SessionPaused, //unknown?
    SessionCompletion,//unknown?
    SessionLogViewing,//unknown?
    TimeDateSetting,//unknown?
    Shutdown            // no battery
};

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Menu* masterMenu;
    Menu* mainMenuOG;

    //Ui::MainWindow *ui;
    QListWidget *activeQListWidget;

    void updateMenu(const QString, const QStringList);



private slots:
    // ui buttons
    void powerButtonClicked();
    void upButtonClicked();
    void downButtonClicked();
    void pauseButtonClicked();
    void startButtonClicked();
    void stopButtonClicked();
    void menuButtonClicked();
    void selectClicked();
    void applyToScalpChanged(int index);

    // battery slots
    void startDrainBattery();
    void stopDrainBattery();
    void chargeBattery();
    void updateBatteryLevel(); // specifically updating battery ui displays
    void lowerBatteryLevel();  // drop battery level by 10 to have some testing functionality

    //  led slots
    void flashRedLED();
    void flashGreenLED();

    // temporary buttons
    void blueOnClicked();
    void redOnClicked();
    void greenOnClicked();
    void blueOffClicked();
    void redOffClicked();
    void greenOffClicked();


    void contactInitButtonClicked();
    void contactLostButtonClicked();
    void deliverTreatmentButtonClicked();

    void shutdownButtonClicked();

private:
    Ui::MainWindow *ui;

    DeviceState currentState;
    void updateDeviceState(DeviceState newState);


    // led
    QTimer flashRedTimer;
    QTimer flashGreenTimer;
    bool redLedState = false; // false means off, true means on
    bool greenLedState = false; // false means off, true means on

    void setLedState(QPushButton* led, const QString& state);
    void flashLED(QPushButton* led, QTimer* timer);
    void stopRedFlashing();
    void stopGreenFlashing();
    // battery
    Battery battery;
    QTimer batteryDrainTimer;


};
#endif // MAINWINDOW_H
