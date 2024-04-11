#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    s = nullptr;

    ui->setupUi(this);
    ui->plotArea->addGraph();
    ui->plotArea->setVisible(false);
    ui->plotArea->yAxis->setRange(-3, 3);
    ui->plotArea->xAxis->setRange(0,61);
    ui->plotArea->replot();

    ui->sessionTimerLabel->setText("00:00");

    // create database connection
    db = new DBManager();

    // Create menu tree
    masterMenu = new Menu("MAIN MENU", {"NEW SESSION","SESSION LOG","TIME AND DATE"}, nullptr);
    mainMenuOG = masterMenu;
    activeQListWidget = ui->mainMenuListView;
    ui->menuLabel->setText(masterMenu->getName());

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

    //connect selecting electrode functions
    connect(ui->selectElectrode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::selectElectrode);

    //battery
//    connect(ui->startDrainButton, &QPushButton::clicked, this, &MainWindow::startDrainBattery);
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

    dtCount = 0;
    connect(&clockUpdate, &QTimer::timeout, this, &MainWindow::continueUpdate);
    chrono.readChrono();
    clockUpdate.start(1000);

    // recordings = db->getSessionsHistoryPC(); // changed
    // how should i store recordings and the history
        // maybe having QVector<Session*> then having a toString functions

    ui->SessionViewWidget->setVisible(false);
    //Populating waveforms and Populating EegSites:
    for(int a = 0; a <7; ++a){
        QVector<double> temp(60,0.0);
        waveforms.push_back(temp);
        EegSite tempEeg;
        eegSites.push_back(tempEeg);
    }
    for(int b = 0; b< 60; b++){
        xData.push_back(b);
    }

    ///connecting timers
    QObject::connect(&analysisTimer, &QTimer::timeout, this, &MainWindow::analysisTimeComplete);
    QObject::connect(&deliverTreatmentTimer, &QTimer::timeout, this, &MainWindow::treatmentTimeComplete);
    QObject::connect(&sessionTimer, &QTimer::timeout, this, &MainWindow::updateSessionTimerLabel);
    QObject::connect(&pausingTimer, &QTimer::timeout, this, &MainWindow::pausingOVER);

    }


    MainWindow::~MainWindow()
    {
        for (int i=0; i<recordings.size(); i++){
            delete recordings[i];
        }
        delete ui;
        delete db;
    }

    void MainWindow::beginSession(Session* s){
        currentSession = s;

        // update timer stuff
        
        const QDateTime& sTime = s->getStartTime();
        Log* loggedSession = new Log("1", sTime, 0, 0);
        recordings.append(loggedSession); // should be removed if current session is stopped
    }

    void MainWindow::showMainMenuView() {
        if (dtCount != 0) {
            interruptDateTime();
        }
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
            //Resseting round count
            roundCount = 0;
            ui->sessionTimerLabel->setText("00:00");
            showNewSessionView();
        } else if (selectedItemText == "SESSION LOG") {
            qDebug() << "Selected Session Log";
            showSessionLogView();
        } else if (selectedItemText == "TIME AND DATE") {
            setDateTime();
        } else if (selectedItemText == "DEVICE SESSION LOG HISTORY"){
            qDebug() << "Selected device session log";
            showDeviceSessionLogView();
        } else if (selectedItemText == "PC SESSION LOG HISTORY"){
            qDebug() << "Selected PC session log";
            showPCSessionLogView();
        }
        // Add else-if blocks for other menu items as needed
    }

    void MainWindow::showNewSessionView(){
        qDebug() << "showNewSessionView()";
        if(battery.getBatteryLevel()< 20){
            ui->batteryAlert->setText("Battery level too low to start session");
            return;
        }
        ui->SessionProgressBar->setValue(0);
        ui->SessionViewWidget->setVisible(true);
        QCoreApplication::processEvents();
        updateDeviceState(DeviceState::SessionActive);

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
        QVector<QString> items = db->getSessionsHistoryDevice();
        qDebug()<< items;
        for (int i=0; i<items.size(); i++){
            QString listInfo = items[i];
            qDebug()<< listInfo;
//            QString sessionInfo = "Session ID: " + listInfo.at(0) + " Date Time: " + listInfo.at(1) + "Length: " + (2);
            activeQListWidget->addItem(listInfo);
        }

    }

    void MainWindow::showPCSessionLogView(){
        qDebug() << "showPCSessionLogView()";
        activeQListWidget->clear(); // Clears items
        QVector<QString> items = db->getSessionsHistoryPC();
        ui->pcView->addItem("ID\t DateTime \t\t\t\tBeforeSite1 \tAfterSite1\t\tBeforeSite2\t\tAfterSite2\t\tBeforeSite3\t\tAfterSite3\t\tBeforeSite4\t\t AfterSite4 \t\tBeforeSite5 \tAfterSite5 \t\tBeforeSite6 \tAfterSite6 \t\tBeforeSit7 \t\tAfterSite7");
        for (int i=0; i<items.size(); i++){
            QString listInfo = items[i];    // need to add in baseline info still
//            QString sessionInfo = "ID: " + listInfo.at(0) + " Date Times: " + listInfo.at(1) + "Length: " + (2);
            ui->pcView  ->addItem(listInfo);
        }
        updateDeviceState(DeviceState::InMainMenu);
    }

    MainWindow::DeviceState MainWindow::getDeviceState() const {
        return currentState; // Return the current device state
    }

    void MainWindow::updateDeviceState(DeviceState newState) {
        currentState = newState;

        switch(currentState) {
            case DeviceState::Off:
                qDebug()<< "DeviceState::Off - Not Draining - Device Off";
                if(s!=nullptr){
                    delete s;
                    s  = nullptr;
                  }
                isDeviceOn = false;
                if (dtCount != 0) {
                    qDebug() << dtCount;
                    interruptDateTime();
                }
                ui->offFrame->setVisible(true);
                ui->SessionViewWidget->setVisible(false);
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
                //enusre red light stops when stop happens
                stopRedFlashing();
                break;
            case DeviceState::SessionActive: {
                QApplication::processEvents();
                if(!appliedToScalp){
                    updateDeviceState(DeviceState::ContactLost);
                    break;
                }
                qDebug() << "Session Active - Using active drain rate.";
                battery.setDrainRateActive(); // Use active drain rate
                MainWindow::startDrainBattery();
                pausingTimer.stop();
//                setLedState(ui->blueLED, "contact_initiated");
                flashRedTimer.stop();
                setLedState(ui->redLED, "off");
                flashGreenTimer.stop();
                setLedState(ui->greenLED, "off");
                if(roundCount > 4){
                    updateDeviceState(DeviceState::SessionCompletion);
                    break;
                }
                //Potential for error here if paused and resumed within round 0
                sessionTimer.blockSignals(true);
                if(battery.getBatteryLevel()<10){
                    qDebug()<< "Battery too low to complete session"<<Qt::endl;
                    ui->sessionTimerLabel->setText("Battery Low\n Ending Session");
                    updateDeviceState(DeviceState::SessionCompletion);
                    break;
                }
                if(roundCount == 0) {

                    qDebug()<< "New session started and creatd"<<Qt::endl;
                    s = new Session(chrono.retrieveChrono());
                    sessionTimer.start(1000);
                }
                vector<double> v;
                int i=0;
                qDebug()<< "Gathering waveforms and calculating Dominant"<<Qt::endl;
                for(EegSite site: eegSites){
                    site.getSignal(v, 60);
                    for(int j  = 0; j < 60; ++j){
                        waveforms[i][j] = (v[j*(v.size()/60)]);
                    }
//                    qInfo()<<waveforms[i];
                    float tempBaseline = EegSite::calculateDominant(v);
//                    qInfo()<< "Baseline: " << tempBaseline;
                    s->setAfter(i,tempBaseline);
                    if(roundCount ==  0){
                       s->setBefore(i,tempBaseline);
                    }
                    i++;
                }
                analysisTimer.blockSignals(false);
                analysisTimer.setInterval(ANALYSIS_TIME);
                sessionTimer.blockSignals(false);
                analysisTimer.setSingleShot(true);
                analysisTimer.start();



                break;
            }
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

               // ui->blueLED->setStyleSheet("QPushButton { background-color: blue; border-radius: 20px; }");

                // Start flashing the red LED to indicate contact loss
                flashLED(ui->redLED, &flashRedTimer);
                stopGreenFlashing();
                analysisTimer.blockSignals(true);
                sessionTimer.blockSignals(true);
                pausingTimer.setSingleShot(true);
                pausingTimer.start(PAUSE_TIME);

                break;

            case DeviceState::DeliverTreatment:
                QApplication::processEvents();
                qDebug() << "DeviceState::DeliverTreatment - Using active drain rate";
                battery.setDrainRateActive();
                MainWindow::startDrainBattery();

                // Turn the blue LED on without affecting its flashing state.
//                ui->blueLED->setStyleSheet("QPushButton { background-color: blue; border-radius: 20px;}");

                // Ensure only the green LED flashes.
                stopRedFlashing();

                // Start flashing the green LED.
                flashLED(ui->greenLED, &flashGreenTimer);

                if(s==NULL){
                    updateDeviceState(DeviceState::SessionActive);
                }
                for(int i = 0; i < NUM_EEG_SITES; i++){
                    float treatFreq = s->getBaselineAfter(i)+ ((roundCount+1)*5);
                    eegSites[i].deliverTreatment(treatFreq, TREATMENT_TIME);
                }
                deliverTreatmentTimer.setSingleShot(true);
                deliverTreatmentTimer.start(TREATMENT_TIME);
                ++roundCount;

                break;
            case DeviceState::SessionPaused:
                 battery.setDrainRateIdle();
                 MainWindow::startDrainBattery();
                analysisTimer.blockSignals(true);
                sessionTimer.blockSignals(true);
               pausingTimer.setSingleShot(true);
               pausingTimer.start(PAUSE_TIME);
                break;

            case DeviceState::SessionCompletion:
                qDebug()<<"Session Complete"<< Qt::endl;
                sessionTimer.stop();
                ui->sessionTimerLabel->setText("End Session");
                if(roundCount >= 4){
                    qDebug()<< "Save session"<<Qt::endl;
                    s->print();
                    db->addSession(s);
                }

                ui->SessionViewWidget->setVisible(false);
                if(s != nullptr) delete s;
                s =  nullptr;
                updateDeviceState(DeviceState::InMainMenu);
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
        if(getDeviceState() == DeviceState::SessionActive || getDeviceState() == DeviceState::DeliverTreatment){
            updateDeviceState(DeviceState::SessionPaused);
        }
    }

    void MainWindow::startButtonClicked() {
        qDebug() << "Start button was clicked!";
        if(getDeviceState() ==DeviceState::SessionPaused){
            updateDeviceState(DeviceState::SessionActive);
        }
    }

    void MainWindow::stopButtonClicked() {
        qDebug() << "Stop button was clicked!";
        if(getDeviceState() ==DeviceState::SessionPaused || getDeviceState() == DeviceState::SessionActive || getDeviceState() == DeviceState::DeliverTreatment||  getDeviceState() == DeviceState::ContactLost){
            updateDeviceState(DeviceState::SessionCompletion);
        }
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

    void MainWindow::analysisTimeComplete()
    {

        qDebug()<< "Analyssis Timer complete"<< Qt::endl;
        if(roundCount<4) updateDeviceState(DeviceState::DeliverTreatment);
        else updateDeviceState(DeviceState::SessionCompletion);
    }

    void MainWindow::updateSessionTimerLabel()
    {
        // Update the timer label with the elapsed time
        QTime currentTime = QTime::fromString(ui->sessionTimerLabel->text(),"mm:ss");
        QTime elapsedTime = currentTime.addMSecs(1000);

        // Display elapsed time in "MM:SS" format
        QString timeStr = elapsedTime.toString("mm:ss");
        qDebug()<< timeStr<<Qt::endl;
        ui->sessionTimerLabel->setText(timeStr);

        int progress =(QTime::fromString(ui->sessionTimerLabel->text(),"mm:ss").second()/ESTIMATED_SESSION_TIME)*100;
        ui->SessionProgressBar->setValue(progress);

    }

    void MainWindow::treatmentTimeComplete()
    {
        qDebug()<< "treatment time complete"<<Qt::endl;
        updateDeviceState(DeviceState::SessionActive);
    }

    void MainWindow::pausingOVER()
    {
        updateDeviceState(DeviceState::SessionCompletion);
    }

    void MainWindow::selectButtonClicked() {
        if (currentState == DeviceState::InMainMenu){
            handleMenuItemSelected();
        }else{
            qDebug() << "Select action not defined for current state";
        }
    }


    void MainWindow::applyToScalpChanged(int index) {

        if (currentState == DeviceState::Off){
            qDebug() << "Device is off, scalp signal doesn't matter.";
        }else{

            bool isAppliedToScalp = (index == 1); // 0 = false, 1 = true
            //If there is contact i.e.  connected to scalp blue light on
            if(isAppliedToScalp) blueOnClicked();
            else blueOffClicked();
            // Update the appliedToScalp member variable accordingly
            appliedToScalp = isAppliedToScalp;
            qDebug() << "Apply to Scalp is" << (isAppliedToScalp ? "true" : "false");

            if (!isAppliedToScalp &&(getDeviceState()== DeviceState::SessionActive||getDeviceState() == DeviceState::DeliverTreatment)){
                updateDeviceState(DeviceState::ContactLost);
            }
            else if(isAppliedToScalp && getDeviceState() ==  DeviceState::ContactLost){
                updateDeviceState(DeviceState::SessionActive);
            }
        }

    }

    void MainWindow::selectElectrode(int index)
    {
        if(index == 0){
            ui->plotArea->setVisible(false);
            return;
        }


       ui->plotArea->setVisible(true);
       ui->plotArea->graph(0)->setData(xData, waveforms[index-1]);
       ui->plotArea->replot();

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

        // save session only if session completed
        // DeviceState == "ON"
        // if (batteryLevel > 0 && currentSession->getTimer()->remainingTime() == 0){ // current session/timer does not currently exits so it will segfault for now
            // if (ui->mainMenuListView->currentItem()->text() == "NEW SESSION"){
            //     // set or get db baseline hz info recordings.last()
            //     db->addSession(recordings.last()->getStartTime(), recordings.last()->getDuration(), recordings.last()->getBaselineHZ(), recordings.last()->getBaselineHZ());
            // }
        // }

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
        dtCount = 1;
    }

    void MainWindow::selectDateTime(){
        qDebug() << dtCount;
        dtCount += 1;

        if (dtCount == 7) {
            chrono.setChrono(ui->dateTimeEdit->dateTime());
            chrono.saveChrono();
            chrono.readChrono();
            interruptDateTime();
        }
    }

    void MainWindow::upDateTime(){
        qDebug() << "Date/time edit up clicked";
        QDateTime currentDateTime = ui->dateTimeEdit->dateTime();
        QDateTime newDateTime = currentDateTime;
        switch(dtCount) {
            case 1:
                newDateTime = newDateTime.addMonths(1);
                break;
            case 2:
                newDateTime = newDateTime.addDays(1);
                break;
            case 3:
                newDateTime = newDateTime.addYears(1);
                break;
            case 4:
                newDateTime = newDateTime.addSecs(3600);
                break;
            case 5:
                newDateTime = newDateTime.addSecs(60);
                break;
            case 6:
                newDateTime = newDateTime.addSecs(12 * 3600);
                break;
        }
        ui->dateTimeEdit->setDateTime(newDateTime);
    }


    void MainWindow::downDateTime(){
        qDebug() << "Date/time edit down clicked";
        QDateTime currentDateTime = ui->dateTimeEdit->dateTime();
        QDateTime newDateTime = currentDateTime;
        switch(dtCount) {
            case 1:
                newDateTime = newDateTime.addMonths(-1);
                break;
            case 2:
                newDateTime = newDateTime.addDays(-1);
                break;
            case 3:
                newDateTime = newDateTime.addYears(-1);
                break;
            case 4:
                newDateTime = newDateTime.addSecs(-3600);
                break;
            case 5:
                newDateTime = newDateTime.addSecs(-60);
                break;
            case 6:
                newDateTime = newDateTime.addSecs(-12 * 3600);
                break;
        }
        ui->dateTimeEdit->setDateTime(newDateTime);
    }

    void MainWindow::setupDateTime() {
        QDateTime defaultDateTime(QDate(2000, 1, 1), QTime(0, 0));
        ui->dateTimeEdit->setDateTime(defaultDateTime);
        disconnect(ui->upButton, &QPushButton::clicked, this, &MainWindow::upButtonClicked);
        disconnect(ui->downButton, &QPushButton::clicked, this, &MainWindow::downButtonClicked);
        disconnect(ui->selectButton, &QPushButton::clicked, this, &MainWindow::selectButtonClicked);
        connect(ui->upButton, &QPushButton::clicked, this, &MainWindow::upDateTime);
        connect(ui->downButton, &QPushButton::clicked, this, &MainWindow::downDateTime);
        connect(ui->selectButton, &QPushButton::clicked, this, &MainWindow::selectDateTime);
    }

    void MainWindow::interruptDateTime() {
        ui->dateTimeEdit->setVisible(false);
        QDateTime defaultDateTime(QDate(2000, 1, 1), QTime(0, 0));
        ui->dateTimeEdit->setDateTime(defaultDateTime);
        disconnect(ui->upButton, &QPushButton::clicked, this, &MainWindow::upDateTime);
        disconnect(ui->downButton, &QPushButton::clicked, this, &MainWindow::downDateTime);
        disconnect(ui->selectButton, &QPushButton::clicked, this, &MainWindow::selectDateTime);
        connect(ui->upButton, &QPushButton::clicked, this, &MainWindow::upButtonClicked);
        connect(ui->downButton, &QPushButton::clicked, this, &MainWindow::downButtonClicked);
        connect(ui->selectButton, &QPushButton::clicked, this, &MainWindow::selectButtonClicked);
        dtCount = 0;
    }

    void MainWindow::updateDeviceClock() {
        QDateTime realdt = chrono.retrieveChrono();
        QString timeString = realdt.toString("hh:mm:ss");
        ui->deviceClock->setText(timeString);
    }

    void MainWindow::continueUpdate() {
        updateDeviceClock();
    }
