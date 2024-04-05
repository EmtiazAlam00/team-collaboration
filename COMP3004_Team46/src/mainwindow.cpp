#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    count = 0; // used for setting date/time
    // create database connection
    db = new DBManager();

    // Create menu tree
    masterMenu = new Menu("MAIN MENU", {"NEW SESSION","SESSION LOG","TIME AND DATE"}, nullptr);
    mainMenuOG = masterMenu;
    activeQListWidget = ui->mainMenuListView;
    ui->menuLabel->setText(masterMenu->getName());



    // Initialize new session view
    // connect
    connect(ui->mainMenuListView, &QListWidget::itemClicked, this, &MainWindow::handleMenuItemSelected);




    // Connect each button to its respective slot
    connect(ui->powerButton, &QPushButton::clicked, this, &MainWindow::powerButtonClicked);
    connect(ui->upButton, &QPushButton::clicked, this, &MainWindow::upButtonClicked);
    connect(ui->downButton, &QPushButton::clicked, this, &MainWindow::downButtonClicked);
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::pauseButtonClicked);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startButtonClicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopButtonClicked);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::menuButtonClicked);
    connect(ui->selectButton, &QPushButton::clicked, this, &MainWindow::selectButtonClicked);

    connect(&flashRedTimer, &QTimer::timeout, this, &MainWindow::flashRedLED);
    connect(&flashGreenTimer, &QTimer::timeout, this, &MainWindow::flashGreenLED);

    // true false for applied to scalp, Apply to skin QComboBox connections
    connect(ui->applyToScalpAdminBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::applyToScalpChanged);

    //battery
    connect(ui->startDrainButton, &QPushButton::clicked, this, &MainWindow::startDrainBattery);
    connect(ui->stopDrainButton, &QPushButton::clicked, this, &MainWindow::stopDrainBattery);
    connect(ui->chargeAdminButton, &QPushButton::clicked, this, &MainWindow::chargeBattery);
    connect(ui->lowerBatteryButton, &QPushButton::clicked, this, &MainWindow::lowerBatteryLevel);

    // Setup and connect the battery drain timer
    connect(&batteryDrainTimer, &QTimer::timeout, this, &MainWindow::updateBatteryLevel);




    //DISABLE THE LED BUTTONS so you can't press the lights
    ui->redLED->setDisabled(1);
    ui->blueLED->setDisabled(1);
    ui->greenLED->setDisabled(1);

    //SET LED BUTTONS TO OFF
    setLedState(ui->redLED, "off");
    setLedState(ui->blueLED, "off");
    setLedState(ui->greenLED, "off");


    // Initialize displays
    updateBatteryLevel();
    // disable spin box
    ui->batteryLevelAdminSpinBox->setDisabled(1);
    // set device state to inMenu
    updateDeviceState(DeviceState::Off);


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

    void MainWindow::showMainMenuView() {

        // Clear current items
        activeQListWidget->clear();

        // Repopulate with main menu items
        activeQListWidget->addItems(masterMenu->getMenuItems());
        activeQListWidget->setCurrentRow(0);

        updateDeviceState(DeviceState::InMainMenu);

        qDebug() << "Main menu view is now active";
    }

    void MainWindow::handleMenuItemSelected() {
        QListWidgetItem *selectedItem = ui->mainMenuListView->currentItem();
        if (!selectedItem) {
            qDebug() << "No item selected";
            return;
        }

        QString selectedItemText = selectedItem->text();
        if (selectedItemText == "NEW SESSION") {
            qDebug() << "Selected New Session";
            showNewSessionView();
        } else if (selectedItemText == "SESSION LOG") {
            qDebug() << "Selected Session Log";
            showSessionLogView();
        } else if (selectedItemText == "TIME AND DATE") {
            setDateTime();
        } else if (selectedItemText == "DEVICE SESSION LOG HISTORY"){
            qDebug() << "Selected device session log";
            showDeviceSessionLogView();
        }
        // Add else-if blocks for other menu items as needed
    }

    void MainWindow::showNewSessionView(){
        qDebug() << "showNewSessionView()";
        activeQListWidget->clear(); // Clear current items
        activeQListWidget->addItem("Placeholder 1");
        activeQListWidget->addItem("Placeholder 2");
    }

    void MainWindow::showSessionLogView(){
        qDebug() << "showSessionLogView()";
        activeQListWidget->clear(); // Clears items
        activeQListWidget->addItem("DEVICE SESSION LOG HISTORY");
        activeQListWidget->addItem("PC SESSION LOG HISTORY");
    }

    void MainWindow::showDeviceSessionLogView(){
        qDebug() << "showDeviceSessionLogView()";
        activeQListWidget->clear(); // Clears items

        activeQListWidget->addItem("DEVICE SESSION LOG HISTORY");
        for (int i=0; i<db->getSessionsHistoryDevice().size(); i++){
            QStringList listInfo = db->getSessionsHistoryDevice().at(i);
            QString sessionInfo = "Session ID: " + listInfo.at(0) + " Date Time: " + listInfo.at(1) + "Length: " + (2);
            activeQListWidget->addItem(sessionInfo);
        }

    }

    void MainWindow::showPCSessionLogView(){
        qDebug() << "showDeviceSessionLogView()";
        activeQListWidget->clear(); // Clears items

        activeQListWidget->addItem("PC SESSION LOG HISTORY");
        for (int i=0; i<db->getSessionsHistoryPC().size(); i++){
            QStringList listInfo = db->getSessionsHistoryPC().at(i);    // need to add in baseline info still
            QString sessionInfo = "ID: " + listInfo.at(0) + " Date Times: " + listInfo.at(1) + "Length: " + (2);
            activeQListWidget->addItem(sessionInfo);
        }

    }

    MainWindow::DeviceState MainWindow::getDeviceState() const {
        return currentState; // Return the current device state
    }
    void MainWindow::updateDeviceState(DeviceState newState) {
        currentState = newState;

        switch(currentState) {
            case DeviceState::Off:
                qDebug()<< "DeviceState::Off - Not Draining - Device Off";
                isDeviceOn = false;
                ui->offFrame->setVisible(true);

                MainWindow::stopDrainBattery();

                // Turn off all LED lights
                stopRedFlashing();
                stopGreenFlashing();
                setLedState(ui->blueLED, "off");

                break;

            case DeviceState::On:
                qDebug() << "DeviceState::On - Device On";
                isDeviceOn = true;

                ui->offFrame->setVisible(false);

                showMainMenuView();
                break;

            case DeviceState::InMainMenu:
                qDebug() << "DeviceState::InMainMenu - Using idle drain rate";
                battery.setDrainRateIdle();
                MainWindow::startDrainBattery();

                break;
            case DeviceState::SessionActive:
                qDebug() << "Session Active - Using active drain rate.";
                battery.setDrainRateActive(); // Use active drain rate
                MainWindow::startDrainBattery();

                setLedState(ui->blueLED, "contact_initiated");
                flashRedTimer.stop();
                setLedState(ui->redLED, "off");
                flashGreenTimer.stop();
                setLedState(ui->greenLED, "off");
                break;

            case DeviceState::InSessionLogMenu:
                qDebug() << "DeviceState::InSessionLogMenu - Using idle drain rate";
                battery.setDrainRateIdle();
                MainWindow::startDrainBattery();

                break;
            case DeviceState::InTimeDateMenu:
                qDebug() << "DeviceState::InTimeDateMenu - Using idle drain rate";
                battery.setDrainRateIdle();
                MainWindow::startDrainBattery();

                break;
            case DeviceState::ContactLost:
                qDebug() << "DeviceState::ContactLost - Using idle drain rate";
                battery.setDrainRateIdle();
                MainWindow::startDrainBattery();

                ui->blueLED->setStyleSheet("QPushButton { background-color: blue; border-radius: 20px; }");

                // Start flashing the red LED to indicate contact loss
                flashLED(ui->redLED, &flashRedTimer);
                stopGreenFlashing();
                break;

            case DeviceState::DeliverTreatment:
                qDebug() << "DeviceState::DeliverTreatment - Using active drain rate";
                battery.setDrainRateActive();
                MainWindow::startDrainBattery();

                // Turn the blue LED on without affecting its flashing state.
                ui->blueLED->setStyleSheet("QPushButton { background-color: blue; border-radius: 20px; }");

                // Ensure only the green LED flashes.
                stopRedFlashing();

                // Start flashing the green LED.
                flashLED(ui->greenLED, &flashGreenTimer);
                break;

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
        double batteryLevel = battery.getBatteryLevel();
        if (batteryLevel == 0){
            qDebug() << "Not enough battery. Please charge and try again!";

        } else {
            // Toggle the state of the device
            isDeviceOn = !isDeviceOn;
            qDebug() << "Power button was clicked! Device is now" << (isDeviceOn ? "ON" : "OFF");
            // Call updateDeviceState to switch between On and Off states
            updateDeviceState(isDeviceOn ? DeviceState::On : DeviceState::Off);
        }

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
        showMainMenuView();
        ui->dateTimeEdit->setVisible(false);
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
        updateDeviceState(DeviceState::ContactLost);
    }

    void MainWindow::deliverTreatmentButtonClicked() {
        updateDeviceState(DeviceState::DeliverTreatment);
    }

    void MainWindow::selectButtonClicked() {
        if (currentState == DeviceState::InMainMenu){
            handleMenuItemSelected();
        }else{
            qDebug() << "Select action not defined for current state";
        }
    }


    void MainWindow::applyToScalpChanged(int index) {
        bool isAppliedToScalp = (index == 1); // 0 = false, 1 = true

        // Update the appliedToScalp member variable accordingly
        appliedToScalp = isAppliedToScalp;
        qDebug() << "Apply to Scalp is" << (isAppliedToScalp ? "true" : "false");

        if (isAppliedToScalp) {
            updateDeviceState(DeviceState::SessionActive);
        } else {
            updateDeviceState(DeviceState::ContactLost);
        }
    }


    void MainWindow::updateMenu(const QString selectMenuItem, const QStringList menuItems){
        activeQListWidget->clear();
        activeQListWidget->addItems(menuItems);
        activeQListWidget->setCurrentRow(0);

        ui->menuLabel->setText(selectMenuItem);
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
        double batteryLevel = battery.getBatteryLevel();
        qDebug() << "Battery draining - Battery at" << batteryLevel << "%";

    }

    void MainWindow::stopDrainBattery() {
        batteryDrainTimer.stop();
        double batteryLevel = battery.getBatteryLevel();
        qDebug() << "Battery not draining - Battery at" << batteryLevel << "%";
    }

    void MainWindow::chargeBattery() {
        battery.setBatteryLevel(100);
        qDebug() << "Battery charged! - Battery at 100%";
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
        ui->batteryLevelBar->setValue(static_cast<int>(batteryLevel));

        // Check battery level and update batteryAlert QLabel accordingly
        if (batteryLevel <= 20) {
            ui->batteryAlert->setText("Battery low: Please charge soon.");
            ui->batteryAlert->setStyleSheet("QLabel { background-color: rgb(192, 191, 188); color : red; }");
        } else {
            ui->batteryAlert->setText("Battery level sufficient.");
            ui->batteryAlert->setStyleSheet("QLabel { background-color: rgb(192, 191, 188); color : green; }");
        }

        // Update battery level bar color
        QString highBatteryHealth = "QProgressBar { selection-background-color: rgb(78, 154, 6); color: white; background-color: rgb(0, 0, 0); }";
        QString mediumBatteryHealth = "QProgressBar { selection-background-color: rgb(196, 160, 0); color: white; background-color: rgb(0, 0, 0); }";
        QString lowBatteryHealth = "QProgressBar { selection-background-color: rgb(164, 0, 0); color: white; background-color: rgb(0, 0, 0); }";

        if (batteryLevel >= 50) {
            ui->batteryLevelBar->setStyleSheet(highBatteryHealth);
        } else if (batteryLevel >= 20 && batteryLevel < 50) {
            ui->batteryLevelBar->setStyleSheet(mediumBatteryHealth);
        } else {
            ui->batteryLevelBar->setStyleSheet(lowBatteryHealth);
        }

        // If battery level is below 0, shut down the device
        if (batteryLevel <= 0) {
            qDebug() << "Battery depleted - Turning Off.";
            updateDeviceState(DeviceState::Off);
        }
    }

    void MainWindow::setDateTime(){
        qDebug() << "Selected Time and Date";
        ui->dateTimeEdit->setVisible(true);
        setupDateTime();
        count = 0;
    }

    void MainWindow::selectDateTime(){
        qDebug() << count;
        count += 1;

        if (count == 6) {
            qDebug() << "Date time is set to" << ui->dateTimeEdit->dateTime().date() << ui->dateTimeEdit->dateTime().time();
            ui->dateTimeEdit->setVisible(false);
            disconnect(ui->upButton, &QPushButton::clicked, this, &MainWindow::upDateTime);
            disconnect(ui->downButton, &QPushButton::clicked, this, &MainWindow::downDateTime);
            disconnect(ui->selectButton, &QPushButton::clicked, this, &MainWindow::selectDateTime);
            connect(ui->upButton, &QPushButton::clicked, this, &MainWindow::upButtonClicked);
            connect(ui->downButton, &QPushButton::clicked, this, &MainWindow::downButtonClicked);
            connect(ui->selectButton, &QPushButton::clicked, this, &MainWindow::selectButtonClicked);
        }
    }

    //TODO: if lowest or highest possible time/date setting (e.g. January), disable and gray out button to prevent carryover

    void MainWindow::upDateTime(){
        qDebug() << "Date/time edit up clicked";
        QDateTime currentDateTime = ui->dateTimeEdit->dateTime();
        QDateTime newDateTime = currentDateTime;
        switch(count) {
            case 0:
                newDateTime = newDateTime.addMonths(1);
                break;
            case 1:
                newDateTime = newDateTime.addDays(1);
                break;
            case 2:
                newDateTime = newDateTime.addYears(1);
                break;
            case 3:
                newDateTime = newDateTime.addSecs(3600);
                break;
            case 4:
                newDateTime = newDateTime.addSecs(60);
                break;
            case 5:
                newDateTime = newDateTime.addSecs(12 * 3600);
                break;
        }
        ui->dateTimeEdit->setDateTime(newDateTime);
    }


    void MainWindow::downDateTime(){
        qDebug() << "Date/time edit down clicked";
        QDateTime currentDateTime = ui->dateTimeEdit->dateTime();
        QDateTime newDateTime = currentDateTime;
        switch(count) {
            case 0:
                newDateTime = newDateTime.addMonths(-1);
                break;
            case 1:
                newDateTime = newDateTime.addDays(-1);
                break;
            case 2:
                newDateTime = newDateTime.addYears(-1);
                break;
            case 3:
                newDateTime = newDateTime.addSecs(-3600);
                break;
            case 4:
                newDateTime = newDateTime.addSecs(-60);
                break;
            case 5:
                newDateTime = newDateTime.addSecs(-12 * 3600);
                break;
        }
        ui->dateTimeEdit->setDateTime(newDateTime);
    }

    void MainWindow::setupDateTime() {
        disconnect(ui->upButton, &QPushButton::clicked, this, &MainWindow::upButtonClicked);
        disconnect(ui->downButton, &QPushButton::clicked, this, &MainWindow::downButtonClicked);
        disconnect(ui->selectButton, &QPushButton::clicked, this, &MainWindow::selectButtonClicked);
        connect(ui->upButton, &QPushButton::clicked, this, &MainWindow::upDateTime);
        connect(ui->downButton, &QPushButton::clicked, this, &MainWindow::downDateTime);
        connect(ui->selectButton, &QPushButton::clicked, this, &MainWindow::selectDateTime);

    }


