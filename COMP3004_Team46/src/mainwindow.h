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
#include <vector>
#include <QtGlobal>
#include <QDateTime>
#include <QDebug>

#include "menu.h"
#include "qcustomplot.h"
#include <QTimer>
#include "battery.h"
#include "dbmanager.h"
#include "session.h"
#include "chrono.h"
#include "eegsite.h"


using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/* Class Purpose: Manages the main user interface of the application, handling user interactions,
 * controlling device state, managing session data, and updating the UI accordingly.
 *
 * Data Members:
 * - Menu* masterMenu, mainMenuOG: Pointers to top-level and original menus for navigation.
 * - DBManager* db: Handles database operations for session persistence.
 * - Session* currentSession: Currently active session, if any.
 * - QVector<Log*> recordings: Stores logs of past sessions.
 * - QListWidget *activeQListWidget: Currently active list widget for menu navigation.
 * - DeviceState currentState: Represents the current operational state of the device.
 * - bool isDeviceOn: Indicates whether the device is powered on.
 * - QTimer flashRedTimer, flashGreenTimer: Timers for LED flashing functionality.
 * - bool redLedState, greenLedState: Indicates the on/off state of the LEDs.
 * - Battery battery: Manages the battery level and drain rates.
 * - QTimer batteryDrainTimer: Timer for simulating battery drain.
 * - int dtCount: Counter used in date/time setting operations.
 * - Chrono chrono: Manages device chronometer functionalities.
 * - QTimer clockUpdate: Timer for device clock display.
 * - int roundCount: A counter to know how many rounds of therapy have occurred.
 * - Session* s: Stores the current session data.
 * - QVector<EegSite> eegSites: stores all the EegSites (For testing and project purposes there are 7 eegSites).
 * - QTimer sessionTimer: The timer for timing each session.
 * - QTimer analysisTimer: The timer to time each analysis time, and update state when it times out.
 * - QTime startTime: Start time variable, stored to session so session data can be correctly stored.
 * - QTimer deliverTreatmentTimer: The timer to time each treatment delivery time, and update state when it times out.
 * - QTimer pausingTimer: The timer to time contact lost/ paused time, and update state when it times out
 * - QVector<QVector<double>> waveforms: Stores the waveform from the most recent analysis to plot the eeg signals using qcustomplot
 * - QVector<double> xData - the x axis data- values 0 to 61 for plotting of waveforms over a minute- a signal per second
 *
 * Class Functions:
 * - MainWindow(QWidget *parent = nullptr): Initializes the UI and connects signals to slots.
 * - ~MainWindow(): Destructor, cleans up allocated resources.
 * - DeviceState getDeviceState() const: Returns the current state of the device.
 * - void updateMenu(const QString, const QStringList): Updates the active menu displayed on the UI.
 *
 * Slots:
 * - void showMainMenuView(): Shows the main menu interface.
 * - void handleMenuItemSelected(): Handles user selections from the menu.
 * - void powerButtonClicked(): Toggles the device's power state.
 * - void upButtonClicked(): Navigates up through the menu or options.
 * - void downButtonClicked(): Navigates down through the menu or options.
 * - void pauseButtonClicked(): Pauses the current session or operation.
 * - void startButtonClicked(): Starts a new session or resumes a paused session.
 * - void stopButtonClicked(): Stops the current session.
 * - void menuButtonClicked(): Returns to the main menu from any state.
 * - void selectButtonClicked(): Selects the current menu option or confirms an action.
 * - void applyToScalpChanged(int index): Updates the state based on the scalp application.
 * - void selectElectrode(int index): Selects the electrode to know which electrode signal to plot.
 * - void startDrainBattery(): Initiates battery drainage at the current rate.
 * - void stopDrainBattery(): Stops the battery from draining.
 * - void chargeBattery(): Recharges the battery to full.
 * - void updateBatteryLevel(): Updates the UI to reflect the current battery level.
 * - void lowerBatteryLevel(): Manually decreases the battery level for testing.
 * - void flashRedLED(): Toggles the red LED's flashing state.
 * - void flashGreenLED(): Toggles the green LED's flashing state.
 * - void setLedState(QPushButton* led, const QString& state): Sets an LED to a specific state.
 * - void flashLED(QPushButton* led, QTimer* timer): Initiates or stops flashing an LED.
 * - void stopRedFlashing(): Stops the red LED from flashing.
 * - void stopGreenFlashing(): Stops the green LED from flashing.
 * - void analysisTimeComplete(): update the DeviceState from sessionActive to DeliverTreatment or SessionCompletion
 * - void updateSessionTimerLabel(): updates the timer in session view
 * - void treatmentTimeComplete(): update the DeviceState from DeliverTreatment to SessionActive
 * - void pausingOVER(): updates the DeviceState to SessionCompletion
 *
 * Utility Functions:
 * - void showNewSessionView(): Displays UI elements for starting a new session.
 * - void showSessionLogView(): Displays the session log view.
 * - void showDeviceSessionLogView(): Shows the device session log.
 * - void showPCSessionLogView(): Shows the PC session log.
 * - void beginSession(Session* s): Starts a new session with the provided session object.
 * - void updateDeviceState(DeviceState newState): Changes the device's state and updates the UI accordingly.
 * - void setDateTime(): Initiates the date/time setting process.
 * - void selectDateTime(): Confirms the selected date/time.
 * - void upDateTime(): Increments the selected date/time component.
 * - void downDateTime(): Decrements the selected date/time component.
 * - void setupDateTime(): Prepares the UI for date/time setting.
 * - void interruptDateTime(): Cancels the date/time setting process.
 * - void updateDeviceClock(): Updates the device's clock display with the current time.
 * - void continueUpdate(): Updates the UI to reflect the current device time.
 *
 * -const int TREATMENT_TIME, PAUSE_TIME, ANALYSIS_TIME, ESTIMATED_SESSION_TIME, NUM_EEG_SITES: constants set currently for testing purposes
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

enum class DeviceState {
    Off,
    On,
    InMainMenu, // initial state
    SessionActive,      //menu 1, blue light, New Session
    InSessionLogMenu, // menu 2
    InTimeDateMenu,     // menu 3
    ContactLost,        // red flash
    DeliverTreatment,   // green flash
    SessionPaused, //unknown?
    SessionCompletion,//unknown?
    SessionLogViewing,//unknown?
};

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    DeviceState getDeviceState() const;
//    DeviceState currentState;

private:
    Menu* masterMenu;
    Menu* mainMenuOG;
    DBManager* db;
    Session* currentSession; // changed
    int currentTimerCount;

    //Ui::MainWindow *ui;
    QListWidget *activeQListWidget;

    void updateMenu(const QString, const QStringList);
    bool appliedToScalp;


private slots:
    void showMainMenuView();
    void handleMenuItemSelected();

    // ui buttons
    void powerButtonClicked();
    void upButtonClicked();
    void downButtonClicked();
    void pauseButtonClicked();
    void startButtonClicked();
    void stopButtonClicked();
    void menuButtonClicked();
    void selectButtonClicked();
    void applyToScalpChanged(int index);
    void selectElectrode(int index);

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

    // Slots for session treatment progression
    void analysisTimeComplete();
    void updateSessionTimerLabel();
    void treatmentTimeComplete();
    void pausingOVER();


private:
    const int TREATMENT_TIME = 1000;
    const int PAUSE_TIME = 10000;
    const int ANALYSIS_TIME = 5000;
    const double ESTIMATED_SESSION_TIME = 30;
    const int NUM_EEG_SITES = 7;

    Ui::MainWindow *ui;

    void showNewSessionView(); // Function to switch view to "New Session"
    void showSessionLogView();  // switches view to "Logged Sessions"
    void showDeviceSessionLogView();
    void showPCSessionLogView();
    void beginSession(Session* s); //changed

    // Device state management
    DeviceState currentState;
    void updateDeviceState(DeviceState newState);
    bool isDeviceOn; // on/off flag

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

    //chrono
    int dtCount = 0;
    Chrono chrono;
    QTimer clockUpdate;
    void setDateTime();
    void selectDateTime();
    void upDateTime();
    void downDateTime();
    void setupDateTime();
    void interruptDateTime();
    void updateDeviceClock();
    void continueUpdate();

    //Running of session variables
    int roundCount = 0;
    Session* s;
    QVector<EegSite> eegSites;
    QTimer sessionTimer;
    QTimer analysisTimer;
    QTime startTime;
    QTimer deliverTreatmentTimer;
    QTimer pausingTimer;
    //

    //Plotting of waveforms
    QVector<QVector<double>> waveforms;
    QVector<double> xData;





};
#endif // MAINWINDOW_H
