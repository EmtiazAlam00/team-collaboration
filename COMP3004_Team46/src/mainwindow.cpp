#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Create menu tree
    masterMenu = new Menu("MAIN MENU", {"NEW SESSION","SESSION LOG","TIME AND DATE"}, nullptr);
    mainMenuOG = masterMenu;
    //initializeMainMenu(masterMenu);

    // Initialize the main menu view
    activeQListWidget = ui->mainMenuListView;
    activeQListWidget->addItems(masterMenu->getMenuItems());
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(masterMenu->getName());


    // Connect each button to its respective slot
    connect(ui->powerButton, &QPushButton::clicked, this, &MainWindow::powerButtonClicked);
    connect(ui->upButton, &QPushButton::clicked, this, &MainWindow::upButtonClicked);
    connect(ui->downButton, &QPushButton::clicked, this, &MainWindow::downButtonClicked);
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::pauseButtonClicked);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startButtonClicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopButtonClicked);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::menuButtonClicked);

    connect(&flashRedTimer, &QTimer::timeout, this, &MainWindow::flashRedLED);
    connect(&flashGreenTimer, &QTimer::timeout, this, &MainWindow::flashGreenLED);

    //battery
    connect(ui->startDrainButton, &QPushButton::clicked, this, &MainWindow::startDrainBattery);
    connect(ui->stopDrainButton, &QPushButton::clicked, this, &MainWindow::stopDrainBattery);
    connect(ui->chargeAdminButton, &QPushButton::clicked, this, &MainWindow::chargeBattery);
    connect(ui->lowerBatteryButton, &QPushButton::clicked, this, &MainWindow::lowerBatteryLevel);

    // Setup and connect the battery drain timer
    connect(&batteryDrainTimer, &QTimer::timeout, this, &MainWindow::updateBatteryLevel);

    // Initialize displays
    updateBatteryLevel();
    // disable spin box
    ui->batteryLevelAdminSpinBox->setDisabled(1);

    //DISABLE THE LED BUTTONS so you can't press the lights
    ui->redLED->setDisabled(1);
    ui->blueLED->setDisabled(1);
    ui->greenLED->setDisabled(1);

    //SET LED BUTTONS TO OFF
    setLedState(ui->redLED, "off");
    setLedState(ui->blueLED, "off");
    setLedState(ui->greenLED, "off");

    // temp buttons
    connect(ui->blueOnButton, &QPushButton::clicked, this, &MainWindow::blueOnClicked);
    connect(ui->redOnButton, &QPushButton::clicked, this, &MainWindow::redOnClicked);
    connect(ui->greenOnButton, &QPushButton::clicked, this, &MainWindow::greenOnClicked);

    connect(ui->blueOffButton, &QPushButton::clicked, this, &MainWindow::blueOffClicked);
    connect(ui->redOffButton, &QPushButton::clicked, this, &MainWindow::redOffClicked);
    connect(ui->greenOffButton, &QPushButton::clicked, this, &MainWindow::greenOffClicked);

    connect(ui->contactInitButton, &QPushButton::clicked, this, &MainWindow::contactInitButtonClicked);
    connect(ui->contactLostButton, &QPushButton::clicked, this, &MainWindow::contactLostButtonClicked);
    connect(ui->deliverTreatmentButton, &QPushButton::clicked, this, &MainWindow::deliverTreatmentButtonClicked);

    connect(ui->shutdownButton, &QPushButton::clicked, this, &MainWindow::shutdownButtonClicked);

    }


    MainWindow::~MainWindow()
    {
        delete ui;
    }

    void MainWindow::updateDeviceState(DeviceState newState) {
        currentState = newState;

        switch(currentState) {
            case DeviceState::SessionActive:
                setLedState(ui->blueLED, "contact_initiated");
                flashRedTimer.stop();
                setLedState(ui->redLED, "off");
                flashGreenTimer.stop();
                setLedState(ui->greenLED, "off");
                break;
            case DeviceState::ContactLoss:
                ui->blueLED->setStyleSheet("QPushButton { background-color: blue; border-radius: 20px; }");

                // Start flashing the red LED to indicate contact loss
                flashLED(ui->redLED, &flashRedTimer);
                stopGreenFlashing();
                break;
            case DeviceState::DeliverTreatment:
                // Turn the blue LED on without affecting its flashing state.
                ui->blueLED->setStyleSheet("QPushButton { background-color: blue; border-radius: 20px; }");

                // Ensure only the green LED flashes.
                stopRedFlashing();

                // Start flashing the green LED.
                flashLED(ui->greenLED, &flashGreenTimer);
                break;
            case DeviceState::Shutdown:

                // Set battery level to 0 and update display
                battery.setBatteryLevel(0);
                updateBatteryLevel();

                // Turn off all LED lights
                stopRedFlashing();
                stopGreenFlashing();
                setLedState(ui->blueLED, "off");

            default:
                // other states
                break;
        }
    }
    // If the red LED was previously flashing, stop it and set it to its "off" state.
    void MainWindow::stopRedFlashing() {
        if (flashRedTimer.isActive()) {
            flashRedTimer.stop();
            ui->redLED->setStyleSheet("QPushButton { background-color: grey; border-radius: 20px; }");
        }
    }
    // If the green LED was previously flashing or on, stop it and set it to its "off" state
    void MainWindow::stopGreenFlashing() {
        if (flashGreenTimer.isActive()) {
            flashGreenTimer.stop();
            ui->greenLED->setStyleSheet("QPushButton { background-color: grey; border-radius: 20px; }");
        }
    }

    void MainWindow::shutdownButtonClicked() {
        // Set the device state to Shutdown
        updateDeviceState(DeviceState::Shutdown);

    }

    void MainWindow::powerButtonClicked() {
        qDebug() << "Power button was clicked!";
    }

    void MainWindow::upButtonClicked() {
        qDebug() << "Up button was clicked!";
        int nextIndex = activeQListWidget->currentRow()-1;
        if(nextIndex < 0){
            nextIndex = activeQListWidget->count()-1;
        }
        activeQListWidget->setCurrentRow(nextIndex);
    }

    void MainWindow::downButtonClicked() {
        qDebug() << "Down button was clicked!";
        int nextIndex = activeQListWidget->currentRow() + 1;

        if (nextIndex > activeQListWidget->count() - 1) {
            nextIndex = 0;
        }

        activeQListWidget->setCurrentRow(nextIndex);
    }

    void MainWindow::pauseButtonClicked() {
        qDebug() << "Pause button was clicked!";
    }

    void MainWindow::startButtonClicked() {
        qDebug() << "Start button was clicked!";
    }

    void MainWindow::stopButtonClicked() {
        qDebug() << "Stop button was clicked!";
    }

    void MainWindow::menuButtonClicked() {
        qDebug() << "Menu button was clicked!";
    }

    void MainWindow::blueOnClicked() {
        setLedState(ui->blueLED, "contact_initiated");
        qDebug() << "Blue LED on!";
    }
    void MainWindow::redOnClicked() {
        setLedState(ui->redLED, "contact_lost");
        qDebug() << "Red LED on!";
    }
    void MainWindow::greenOnClicked() {
        setLedState(ui->greenLED, "deliver_treatment");
        qDebug() << "Green LED on!";
    }
    void MainWindow::blueOffClicked() {
        setLedState(ui->blueLED, "off");
        qDebug() << "Blue LED off!";
    }

    void MainWindow::redOffClicked() {
        setLedState(ui->redLED, "off");
        flashRedTimer.stop();
        qDebug() << "Red LED off!";
    }

    void MainWindow::greenOffClicked() {
        setLedState(ui->greenLED, "off");
        flashGreenTimer.stop();
        qDebug() << "Green LED off!";
    }

    void MainWindow::contactInitButtonClicked() {
        updateDeviceState(DeviceState::SessionActive);
    }

    void MainWindow::contactLostButtonClicked() {
        updateDeviceState(DeviceState::ContactLoss);
    }

    void MainWindow::deliverTreatmentButtonClicked() {
        updateDeviceState(DeviceState::DeliverTreatment);
    }




    void MainWindow::flashLED(QPushButton* led, QTimer* timer) {
        if (!timer->isActive()) {
            // Start flashing
            timer->start(500); // Adjust the interval as needed
        } else {
            // Already flashing, so stop
            timer->stop();
            led->setStyleSheet("QPushButton { background-color: grey; border-radius: 20px; }");
        }
    }

    void MainWindow::flashRedLED() {
        redLedState = !redLedState;
        ui->redLED->setStyleSheet(redLedState ? "QPushButton { background-color: red; border-radius: 20px; }" : "QPushButton { background-color: grey; border-radius: 20px; }");
    }

    void MainWindow::flashGreenLED() {
        greenLedState = !greenLedState;
        ui->greenLED->setStyleSheet(greenLedState ? "QPushButton { background-color: green; border-radius: 20px; }" : "QPushButton { background-color: grey; border-radius: 20px; }");
    }

    void MainWindow::setLedState(QPushButton* led, const QString& state) {
        if (state == "off") {
            led->setStyleSheet("QPushButton { background-color: grey; border-radius: 20px; }");
            led->setVisible(true);
        } else if (state == "contact_initiated") {
            led->setStyleSheet("QPushButton { background-color: blue; border-radius: 20px; }");
            led->setVisible(true);
        } else if (state == "deliver_treatment") {
            flashLED(ui->greenLED, &flashGreenTimer);
        } else if (state == "contact_lost") {
            flashLED(ui->redLED, &flashRedTimer);
        } else {
            led->setVisible(false); // Hide the LED if the state doesn't match any condition
        }
    }

    void MainWindow::startDrainBattery() {
        batteryDrainTimer.start(1000); // Drain battery every second
    }

    void MainWindow::stopDrainBattery() {
        batteryDrainTimer.stop();
    }

    void MainWindow::chargeBattery() {
        battery.setBatteryLevel(100);
        updateBatteryLevel();
    }

    void MainWindow::lowerBatteryLevel() {
        double currentLevel = battery.getBatteryLevel();
        double newLevel = std::max(0.0, currentLevel - 10.0); // Ensure battery level does not go below 0
        battery.setBatteryLevel(newLevel);
        updateBatteryLevel(); // Update the battery level display
    }

    void MainWindow::updateBatteryLevel() {
        if (batteryDrainTimer.isActive()) {
            battery.drainBattery(); // Decrease battery level
        }

        double batteryLevel = battery.getBatteryLevel();
        ui->batteryLevelAdminSpinBox->setValue(batteryLevel); // Update display
        ui->batteryLevelBar->setValue(batteryLevel);

        QString highBatteryHealth = "QProgressBar { selection-background-color: rgb(78, 154, 6); color: white; background-color: rgb(0, 0, 0); }";
        QString mediumBatteryHealth = "QProgressBar { selection-background-color: rgb(196, 160, 0); color: white; background-color: rgb(0, 0, 0); }";
        QString lowBatteryHealth = "QProgressBar { selection-background-color: rgb(164, 0, 0); color: white; background-color: rgb(0, 0, 0); }";

        if (batteryLevel >= 50) {
            ui->batteryLevelBar->setStyleSheet(highBatteryHealth);
        }
        else if (batteryLevel >= 20) {
            ui->batteryLevelBar->setStyleSheet(mediumBatteryHealth);
        }
        else {
            ui->batteryLevelBar->setStyleSheet(lowBatteryHealth);
        }
    }


