#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect each button to its respective slot
    connect(ui->powerButton, &QPushButton::clicked, this, &MainWindow::powerButtonClicked);
    connect(ui->upButton, &QPushButton::clicked, this, &MainWindow::upButtonClicked);
    connect(ui->downButton, &QPushButton::clicked, this, &MainWindow::downButtonClicked);
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::pauseButtonClicked);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startButtonClicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopButtonClicked);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::menuButtonClicked);

    connect(ui->blueOnButton, &QPushButton::clicked, this, &MainWindow::blueOnClicked);
    connect(ui->redOnButton, &QPushButton::clicked, this, &MainWindow::redOnClicked);
    connect(ui->greenOnButton, &QPushButton::clicked, this, &MainWindow::greenOnClicked);

    connect(ui->blueOffButton, &QPushButton::clicked, this, &MainWindow::blueOffClicked);
    connect(ui->redOffButton, &QPushButton::clicked, this, &MainWindow::redOffClicked);
    connect(ui->greenOffButton, &QPushButton::clicked, this, &MainWindow::greenOffClicked);

    connect(ui->contactInitButton, &QPushButton::clicked, this, &MainWindow::contactInitButtonClicked);
    connect(ui->contactLostButton, &QPushButton::clicked, this, &MainWindow::contactLostButtonClicked);
    connect(ui->applyTreatmentButton, &QPushButton::clicked, this, &MainWindow::applyTreatmentButtonClicked);

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

    //SET LED BUTTOSN TO OFF
    setLedState(ui->redLED, "off");
    setLedState(ui->blueLED, "off");
    setLedState(ui->greenLED, "off");


    }


    MainWindow::~MainWindow()
    {
        delete ui;
    }

    void MainWindow::powerButtonClicked() {
        qDebug() << "Power button was clicked!";
    }

    void MainWindow::upButtonClicked() {
        qDebug() << "Up button was clicked!";
    }

    void MainWindow::downButtonClicked() {
        qDebug() << "Down button was clicked!";
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
        setLedState(ui->greenLED, "treatment");
        qDebug() << "Green LED on!";
    }
    void MainWindow::blueOffClicked() {
        setLedState(ui->blueLED, "off");
        qDebug() << "Blue LED off!";
    }

    void MainWindow::redOffClicked() {
        setLedState(ui->redLED, "off");
        qDebug() << "Red LED off!";
    }

    void MainWindow::greenOffClicked() {
        setLedState(ui->greenLED, "off");
        qDebug() << "Green LED off!";
    }

    void MainWindow::contactInitButtonClicked() {
        setLedState(ui->blueLED, "contact_initiated");
        flashRedTimer.stop();
        setLedState(ui->redLED, "off");
        flashGreenTimer.stop();
        setLedState(ui->greenLED, "off");

    }

    void MainWindow::contactLostButtonClicked() {
        ui->blueLED->setStyleSheet("QPushButton { background-color: blue; border-radius: 20px; }");

        // Start flashing the red LED to indicate contact loss
        flashLED(ui->redLED, &flashRedTimer, "red");

        // If the green LED was previously flashing or on, stop it and set it to its "off" state
        if(flashGreenTimer.isActive()) {
            flashGreenTimer.stop();
            ui->greenLED->setStyleSheet("QPushButton { background-color: grey; border-radius: 20px; }");
        }
    }


    void MainWindow::applyTreatmentButtonClicked() {
        // Turn the blue LED on without affecting its flashing state.
        ui->blueLED->setStyleSheet("QPushButton { background-color: blue; border-radius: 20px; }");

        // Ensure only the green LED flashes.
        // If the red LED was previously flashing, stop it and set it to its "off" state.
        if(flashRedTimer.isActive()) {
            flashRedTimer.stop();
            ui->redLED->setStyleSheet("QPushButton { background-color: grey; border-radius: 20px; }");
        }

        // Start flashing the green LED.
        flashLED(ui->greenLED, &flashGreenTimer, "green");
    }


    void MainWindow::flashLED(QPushButton* led, QTimer* timer, const QString& color) {
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
        } else if (state == "treatment") {
            flashLED(ui->greenLED, &flashGreenTimer, "green");
        } else if (state == "contact_lost") {
            flashLED(ui->redLED, &flashRedTimer, "red");
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

