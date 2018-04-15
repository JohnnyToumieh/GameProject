#include "GameScene3.h"
#include <string>
#include "Message.h"

/**
 *\file GameScene3.cpp
 *@brief contains GameScene3 class definition which represents the game 2 shell.
 *
 * It handles saving of games, the creation and deletion of elements and tracking the game state.
 *
 */

/**
 * @brief GameScene3::GameScene3 constructor of GameScene3 class
 *
 * A constructor that sets up the game with all initial attributes.
 * @param QWidget *widget represents the main widget holding all items
 * @param User* user is the user signed in
 * @param QJsonObject dataFile holds the info of the user
 * @param bool resume determines is the game is being resumed
 * @param int level determines if the game should start at a specific level
 */
GameScene3::GameScene3(QWidget *widget, int width, int height, User* user, QJsonObject dataFile, bool resume, int level, bool isMiniGame) : GameScene(widget, user, dataFile, 2, isMiniGame)
{
    srand(QTime::currentTime().msec());

    if (resume) {
        QJsonObject officeSave = read("office").object();
        office = new Office(officeSave["level"].toInt(),
                officeSave["currentReputation"].toInt(),
                officeSave["currentTime"].toInt(),
                officeSave["currentAquariumState"].toInt(),
                officeSave["score"].toInt());
    } else {
        office = new Office(level, 0, 0, 0, 0);
    }

    QFile file("questions.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }
    QString questionsString="";
    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        questionsString = questionsString +line;
    }

    questionsString.replace(".","\n");
    questionsString.replace("?","?\n");

    questionList = questionsString.split("<Q>");
    file.close();

    QFile file1("answers.txt");
    if(!file1.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }
    QString answersString="";
    QTextStream in1(&file1);

    while(!in1.atEnd()) {
        QString line = in1.readLine();
        answersString = answersString +line;
    }

    answerList = answersString.split(",");
    file.close();

    for(int i=0;i<questionList.size();i++){
        questions[questionList[i]] = answerList[i].toInt();
    }

    office->setFlag(QGraphicsItem::ItemIsFocusable);
    addItem(office);

    setBackgroundBrush(QBrush(QImage(":game2Background2").scaledToHeight(height).scaledToWidth(width)));
    setSceneRect(0,0,width,height);

    aquarium = new QGraphicsPixmapItem();
    updateAquariumImage();
    aquarium->setPos(52, 313);
    addItem(aquarium);
    aquarium->setFlag(QGraphicsItem::ItemIsFocusable);

    timeLabel = new QLabel("00:00 AM");
    timeLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 35px; }");
    addWidget(timeLabel);
    timeLabel->move(this->width() / 2 - timeLabel->width() / 2, 13);

    unpauseLabel = new QLabel();
    unpauseLabel->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : white; font: 140px; }");
    unpauseLabel->move(this->width() / 2 - 90, this->height() / 2 - 90);
    unpauseLabel->setWordWrap(true);
    QGraphicsProxyWidget* proxyWidget = addWidget(unpauseLabel);
    proxyWidget->setZValue(10000);
    unpauseLabel->hide();

    levelLabel = new QLabel();
    levelLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 20px; }");
    levelLabel->move(30, 10);
    levelLabel->setText(QStringLiteral("Level: %1").arg(office->level));
    addWidget(levelLabel);

    scoreLabel = new QLabel();
    scoreLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 20px; }");
    scoreLabel->move(30, 30);
    scoreLabel->setText(QStringLiteral("Score: %1").arg(office->score));
    scoreLabel->adjustSize();
    addWidget(scoreLabel);

    pixmapNeedle = new QGraphicsPixmapItem();
    QPixmap *picNeedle  = new QPixmap(":needle");
    pixmapNeedle->setPixmap(picNeedle->scaled(80,20));
    pixmapNeedle->setPos(855, 50);
    pixmapNeedle->setTransformOriginPoint(pixmapNeedle->boundingRect().center().x() + 20,
                                               pixmapNeedle->boundingRect().center().y());
    addItem(pixmapNeedle);

    greenColorItem= new QGraphicsPixmapItem();
    greenColorItem->setPos(15,51);
    QPixmap *greenColor = new QPixmap(":needle");
    greenColor->fill(Qt::green);
    greenColorItem->setPixmap(greenColor->scaled((230 / office->levels[office->level]["maxReputation"]) * office->currentReputation, 20));
    addItem(greenColorItem);

    patients = new Patient*[20];
    for (int i = 0; i < 20; i++) {
        patients[i] = NULL;
    }
    patientsIndex = 0;

    if (resume) {
        QJsonArray patientsSave = read("patients").array();
        for (patientsIndex = 0; patientsIndex < patientsSave.size(); patientsIndex++) {
            QJsonObject currentPatient = patientsSave[patientsIndex].toObject();
            patients[patientsIndex] = new Patient(currentPatient["type"].toInt(), office,
                    currentPatient["motionState"].toString(), currentPatient["statusState"].toString());
            patients[patientsIndex]->setX(currentPatient["x"].toDouble());
            patients[patientsIndex]->setY(currentPatient["y"].toDouble());
            addItem(patients[patientsIndex]);
        }
    }

   if (resume) {
        pausedTime = office->currentTime;
    } else {
        pausedTime = 0;
    }
    justPaused = true;

    greyForeground = new QWidget();
    greyForeground->setStyleSheet("background-color: rgba(105, 105, 105, 100);");
    greyForeground->setFixedHeight(this->height());
    greyForeground->setFixedWidth(this->width());
    proxyWidget = addWidget(greyForeground);
    proxyWidget->setZValue(10000);
    greyForeground->hide();

    unpause = new QPushButton("Unpause");
    unpause->move(this->width() / 2 - 30, this->height() / 2 - 20);
    proxyWidget = addWidget(unpause);
    proxyWidget->setZValue(10000);
    unpause->hide();
    unpause->setFocusPolicy(Qt::NoFocus);

    quit = new QPushButton("Quit");
    quit->move(this->width() / 2 - 30, this->height() / 2 + 10);
    proxyWidget = addWidget(quit);
    proxyWidget->setZValue(10000);
    quit->hide();
    quit->setFocusPolicy(Qt::NoFocus);

    gameOverLabel = new QLabel("GAME OVER");
    gameOverLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 140px; }");
    gameOverLabel->move(90, 150);
    proxyWidget = addWidget(gameOverLabel);
    proxyWidget->setZValue(10000);
    gameOverLabel->hide();

    scoreLabel2 = new QLabel();
    scoreLabel2->setStyleSheet("QLabel { background-color : black; color : white; font: 80px; }");
    proxyWidget = addWidget(scoreLabel2);
    proxyWidget->setZValue(10000);
    scoreLabel2->hide();

    quit2 = new QPushButton("Quit");
    quit2->move(this->width() / 2 - 60, this->height() / 2 + 150);
    proxyWidget = addWidget(quit2);
    proxyWidget->setZValue(10000);
    quit2->hide();
    quit2->setFocusPolicy(Qt::NoFocus);

    nextLevelButton = new QPushButton("Next Level");
    nextLevelButton->move(this->width() / 2 - 50, this->height() / 2 + 150);
    proxyWidget = addWidget(nextLevelButton);
    proxyWidget->setZValue(10000);
    nextLevelButton->hide();
    nextLevelButton->setFocusPolicy(Qt::NoFocus);

    patientBox = new QWidget();
    patientBox->setStyleSheet("background-color: rgba(105, 105, 105, 100);");
    addWidget(patientBox);
    patientBox->setFixedSize(250, 250);
    patientBox->move(this->width() / 2 - patientBox->width() / 2, this->height() / 2 - patientBox->height() / 2 + 50);
    patientBox->hide();

    choiceA = new QPushButton("A");
    addWidget(choiceA);
    choiceA->move(patientBox->x() + 5, patientBox->y() + patientBox->height() - 20 - choiceA->height());
    choiceA->hide();
    choiceA->setFocusPolicy(Qt::NoFocus);

    choiceB = new QPushButton("B");
    addWidget(choiceB);
    choiceB->move(choiceA->x()+80, patientBox->y() + patientBox->height() - 20 - choiceB->height());
    choiceB->hide();
    choiceB->setFocusPolicy(Qt::NoFocus);

    choiceC = new QPushButton("C");
    addWidget(choiceC);
    choiceC->move(choiceB->x()+ 80, patientBox->y() + patientBox->height() - 20 - choiceC->height());
    choiceC->hide();
    choiceC->setFocusPolicy(Qt::NoFocus);

    reject = new QPushButton("Reject");
    addWidget(reject);
    reject->move(patientBox->x() + 30, patientBox->y() + patientBox->height() - 20 - reject->height());
    reject->hide();
    reject->setFocusPolicy(Qt::NoFocus);

    accept = new QPushButton("Accept");
    addWidget(accept);
    accept->move(patientBox->x() + patientBox->width() - 30 - accept->width(), patientBox->y() + patientBox->height() - 20 - accept->height());
    accept->hide();
    accept->setFocusPolicy(Qt::NoFocus);

    description = new QLabel("long long string");
    description->setWordWrap(true);
    addWidget(description);
    description->setFixedSize(patientBox->width() - 2 * 15, patientBox->height() - 2 * 15 - accept->height() - 2 * 15);
    description->move(patientBox->x() + 15, patientBox->y() + 15);
    description->hide();

    aquariumBox = new QWidget();
    aquariumBox->setStyleSheet("background-color: rgba(105, 105, 105, 100);");
    addWidget(aquariumBox);
    aquariumBox->setFixedSize(250, 250);
    aquariumBox->move(this->width() / 2 - aquariumBox->width() / 2, this->height() / 2 - aquariumBox->height() / 2 + 50);
    aquariumBox->hide();

    cancelAquarium = new QPushButton("Cancel");
    addWidget(cancelAquarium);
    cancelAquarium->move(aquariumBox->x() + 30, aquariumBox->y() + aquariumBox->height() - 20 - cancelAquarium->height());
    cancelAquarium->hide();
    cancelAquarium->setFocusPolicy(Qt::NoFocus);

    cleanAquarium = new QPushButton("Clean");
    addWidget(cleanAquarium);
    cleanAquarium->move(aquariumBox->x() + aquariumBox->width() - 30 - cleanAquarium->width(), aquariumBox->y() + aquariumBox->height() - 20 - cleanAquarium->height());
    cleanAquarium->hide();
    cleanAquarium->setFocusPolicy(Qt::NoFocus);

    aquariumDescription = new QLabel("long long");
    aquariumDescription->setWordWrap(true);
    addWidget(aquariumDescription);
    aquariumDescription->setFixedSize(aquariumBox->width() - 2 * 15, aquariumBox->height() - 2 * 15 - aquariumDescription->height() - 2 * 15);
    aquariumDescription->move(aquariumBox->x() + 15, aquariumBox->y() + 15);
    aquariumDescription->hide();

    reputationBox = new QWidget();
    reputationBox->setStyleSheet("background-color: rgba(105, 105, 105, 100);");
    proxyWidget = addWidget(reputationBox);
    proxyWidget->setZValue(10000);
    reputationBox->setFixedSize(250, 250);
    reputationBox->move(this->width() / 2 - reputationBox->width() / 2, this->height() / 2 - reputationBox->height() / 2 + 50);
    reputationBox->hide();

    cancelReputation = new QPushButton("Cancel");
    proxyWidget = addWidget(cancelReputation);
    proxyWidget->setZValue(10000);
    cancelReputation->move(reputationBox->x() + 30, reputationBox->y() + reputationBox->height() - 20 - cancelReputation->height());
    cancelReputation->hide();
    cancelReputation->setFocusPolicy(Qt::NoFocus);

    useReputation = new QPushButton("Use");
    proxyWidget = addWidget(useReputation);
    proxyWidget->setZValue(10000);
    useReputation->move(reputationBox->x() + reputationBox->width() - 30 - useReputation->width(), reputationBox->y() + reputationBox->height() - 20 - useReputation->height());
    useReputation->hide();
    useReputation->setFocusPolicy(Qt::NoFocus);

    reputationDescription = new QLabel("Your reputation is full! Do you want to use up your reputation points to retry the level?");
    reputationDescription->setWordWrap(true);
    proxyWidget = addWidget(reputationDescription);
    proxyWidget->setZValue(10000);
    reputationDescription->setFixedSize(reputationBox->width() - 2 * 15, reputationBox->height() - 2 * 15 - reputationDescription->height() - 2 * 15);
    reputationDescription->move(reputationBox->x() + 15, reputationBox->y() + 15);
    reputationDescription->hide();

    connect(unpause, SIGNAL(clicked()), SLOT(unpauseClicked()));
    connect(quit, SIGNAL(clicked()), SLOT(quitClicked()));
    connect(quit2, SIGNAL(clicked()), SLOT(quitClicked()));
    connect(nextLevelButton, SIGNAL(clicked()), SLOT(nextLevel()));
    connect(cleanAquarium, SIGNAL(clicked()), SLOT(handleAquariumRequest()));
    connect(cancelAquarium, SIGNAL(clicked()), SLOT(cancelAquariumRequest()));

    QSignalMapper* signalMapper = new QSignalMapper(this);
    connect(choiceA, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(choiceB, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(choiceC, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(choiceA, 1);
    signalMapper->setMapping(choiceB, 2);
    signalMapper->setMapping(choiceC, 3);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(answerClicked(int))) ;

    signalMapper = new QSignalMapper(this);
    connect(accept, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(reject, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(accept, 1);
    signalMapper->setMapping(reject, 0);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(handlePatient(int))) ;

    signalMapper = new QSignalMapper(this);
    connect(useReputation, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(cancelReputation, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(useReputation, 1);
    signalMapper->setMapping(cancelReputation, 0);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(handleReputation(int))) ;

    checkGameStateTimer = new QTimer(this);
    connect(checkGameStateTimer, SIGNAL(timeout()), this, SLOT(checkGameState()));
    checkGameStateTimer->start(100);

    time = new QTime();
    time->start();

    timeUpdater = new QTimer(this);
    connect(timeUpdater, SIGNAL(timeout()), this, SLOT(updateTimer()));
    unpauseTimer = new QTimer(this);
    connect(unpauseTimer, SIGNAL(timeout()), this, SLOT(unpauseGame()));
    updatePatientsTimer = new QTimer(this);
    connect(updatePatientsTimer, SIGNAL(timeout()), this, SLOT(updatePatients()));
    updateAquariumTimer = new QTimer(this);
    connect(updateAquariumTimer, SIGNAL(timeout()), this, SLOT(updateAquarium()));

    unpauseTimer->setSingleShot(true);
    updatePatientsTimer->setSingleShot(true);
    updateAquariumTimer->setSingleShot(true);

    if (resume) {
        QJsonObject pausedTimesSave = read("pausedTimes").object();

        timeUpdater->setSingleShot(true);

        timeUpdater->start(pausedTimesSave["pausedTimeUpdater"].toInt());
        if (pausedTimesSave.contains("pausedUpdatePatientsTimer")) {
            updatePatientsTimer->start(pausedTimesSave["pausedUpdatePatientsTimer"].toInt());
        }
        if (pausedTimesSave.contains("pausedUpdateAquariumTimer")) {
            updateAquariumTimer->start(pausedTimesSave["pausedUpdateAquariumTimer"].toInt());
        }
    } else {
        timeUpdater->start(100);
        updatePatientsTimer->start(office->levels[office->level]["patientGenerationRate"]);
        updateAquariumTimer->start(office->levels[office->level]["dirtinessRate"]);
    }

    this->pausedTimeUpdater = 0;
    this->pausedUpdatePatientsTimer = 0;
    this->pausedUpdateAquariumTimer = 0;

    updateTimer();
}

int GameScene3::getCurrentScore() {
    return office->score;
}

int GameScene3::getLevelState() {
    return office->levels[office->level]["levelState"];
}

/**
 * @brief GameScene3::nextLevel
 *
 * A member function that loads the next level and mainly sets the timers.
 */
void GameScene3::nextLevel() {
    scoreLabel2->hide();
    quit2->hide();
    greyForeground->hide();
    gameOverLabel->hide();
    nextLevelButton->hide();

    checkGameStateTimer->start(100);
    updatePatientsTimer->start(office->levels[office->level]["patientGenerationRate"]);
    updateAquariumTimer->start(office->levels[office->level]["dirtinessRate"]);
    timeUpdater->start(500);

    pausedTime = 0;
    time->restart();
    updateTimer();
}

/**
 * @brief GameScene3::setUpNextLevel
 *
 * A member function that sets up the next level.
 */
void GameScene3::setUpNextLevel() {
    office->level++;
    office->currentReputation = 0;
    office->currentTime = 0;

    QPixmap *greenColor = new QPixmap(":needle");
    greenColor->fill(Qt::green);
    greenColorItem->setPixmap(greenColor->scaled((230 / office->levels[office->level]["maxReputation"]) * office->currentReputation, 20));

    pixmapNeedle->setTransformOriginPoint(pixmapNeedle->boundingRect().center().x() + 20,
                                               pixmapNeedle->boundingRect().center().y());
    pixmapNeedle->setRotation(0);
}

void GameScene3::updateAquariumImage() {
    QPixmap *picAquarium;
    if (office->currentAquariumState == 0) {
        picAquarium  = new QPixmap(":aquarium0");
    } else if (office->currentAquariumState == 1) {
        picAquarium  = new QPixmap(":aquarium1");
    } else if (office->currentAquariumState == 2) {
        picAquarium  = new QPixmap(":aquarium2");
    } else if (office->currentAquariumState == 3) {
        picAquarium  = new QPixmap(":aquarium3");
    }
    aquarium->setPixmap(picAquarium->scaled(153, 87));
}

void GameScene3::updateAquarium() {
    if (office->currentAquariumState < 2) {
        int time = (rand() % 1000) + office->levels[office->level]["dirtinessRate"] - 500;
        updateAquariumTimer->start(time);
    }

    office->currentAquariumState++;
}

/**
 * @brief GameScene3::updateTimer
 *
 * A member function that updates time based labels.
 */
void GameScene3::updateTimer() {
    if (timeUpdater->isSingleShot()) {
        timeUpdater->setSingleShot(false);
        timeUpdater->start(500);
    }

    int timeConversion = (time->elapsed() + pausedTime) / office->levels[office->level]["minuteInMilliSeconds"] + office->levels[office->level]["startTime"];
    int hours = (timeConversion / 60) % 60;
    int mins = timeConversion % 60;

    std::string dayTime;
    if (hours < 12) {
        dayTime = "AM";
    } else {
        if (hours != 12) {
            hours = hours - 12;
        }
        dayTime = "PM";
    }

    timeLabel->setText(QString("%1:%2 ")
    .arg(hours, 2, 10, QLatin1Char('0'))
    .arg(mins, 2, 10, QLatin1Char('0')) + QString::fromStdString(dayTime));

    office->currentTime = time->elapsed() + pausedTime;
}

/**
 * @brief GameScene3::updatePatients
 *
 * A member function that generates a new patients.
 */
void GameScene3::updatePatients(){
    if (patientsIndex > 19) {
        patientsIndex = 0;
    }

    int weight = (rand() % (office->levels[office->level]["patientWeight1"]
                        + office->levels[office->level]["patientWeight2"]
                        + office->levels[office->level]["patientWeight3"]
                        +  office->levels[office->level]["patientWeight4"]
                        + office->levels[office->level]["patientWeight5"]
                        + office->levels[office->level]["patientWeight6"]
                        + office->levels[office->level]["patientWeight7"]
                        + office->levels[office->level]["patientWeight8"]
                        + office->levels[office->level]["patientWeight9"]));

    int type = 1;
    if (weight < office->levels[office->level]["patientWeight1"]) {
        type = 1;
    } else if (weight < office->levels[office->level]["patientWeight1"] + office->levels[office->level]["patientWeight2"]) {
        type = 2;
    } else if (weight < office->levels[office->level]["patientWeight1"] + office->levels[office->level]["patientWeight2"]+ office->levels[office->level]["patientWeight3"]){
        type = 3;
    }else if(weight < office->levels[office->level]["patientWeight1"] + office->levels[office->level]["patientWeight2"]+ office->levels[office->level]["patientWeight3"] + office->levels[office->level]["patientWeight4"]){
         type = 4;
    }else if(weight < office->levels[office->level]["patientWeight1"] + office->levels[office->level]["patientWeight2"]+ office->levels[office->level]["patientWeight3"] + office->levels[office->level]["patientWeight4"]+ office->levels[office->level]["patientWeight5"]){
         type = 5;
    }else if(weight < office->levels[office->level]["patientWeight1"] + office->levels[office->level]["patientWeight2"]+ office->levels[office->level]["patientWeight3"] + office->levels[office->level]["patientWeight4"]+ office->levels[office->level]["patientWeight5"]+ office->levels[office->level]["patientWeight6"]){
        type = 6;
    }else if(weight < office->levels[office->level]["patientWeight1"] + office->levels[office->level]["patientWeight2"]+ office->levels[office->level]["patientWeight3"] + office->levels[office->level]["patientWeight4"]+ office->levels[office->level]["patientWeight5"]+office->levels[office->level]["patientWeight6"]+ office->levels[office->level]["patientWeight7"]){
        type = 7;
    }else if(weight < office->levels[office->level]["patientWeight1"] + office->levels[office->level]["patientWeight2"]+ office->levels[office->level]["patientWeight3"] + office->levels[office->level]["patientWeight4"]+ office->levels[office->level]["patientWeight5"]+office->levels[office->level]["patientWeight6"]+ office->levels[office->level]["patientWeight7"]+office->levels[office->level]["patientWeight8"] ){
        type = 8;
    }else if(weight < office->levels[office->level]["patientWeight1"] + office->levels[office->level]["patientWeight2"]+ office->levels[office->level]["patientWeight3"] + office->levels[office->level]["patientWeight4"]+ office->levels[office->level]["patientWeight5"]+office->levels[office->level]["patientWeight6"]+ office->levels[office->level]["patientWeight7"]+ office->levels[office->level]["patientWeight8"]+ office->levels[office->level]["patientWeight9"]  ){
        type = 9;
    }

    patients[patientsIndex] = new Patient(type, office);
    addItem(patients[patientsIndex++]);
}

void GameScene3::handlePatient(int status) {
    reject->hide();
    accept->hide();
    description->hide();
    patientBox->hide();

    int index = (patientsIndex == 0) ? 19 : patientsIndex - 1;

    if (status == 1) {
        patients[index]->motionState = Patient::GettingReady;
        patients[index]->statusState = Patient::Accepted;
    } else if (status == 0) {
        patients[index]->motionState = Patient::Leaving;
        patients[index]->statusState = Patient::Rejected;

        //Reduce reputation
        if (office->currentReputation >= office->levels[office->level]["incrementReputation"]) {
            office->currentReputation -= office->levels[office->level]["incrementReputation"];
        } else {
            office->currentReputation = 0;
        }

        int time = (rand() % 1000) + office->levels[office->level]["patientGenerationRate"] - 500;
        updatePatientsTimer->start(time);
    }
}

void GameScene3::handleAquariumRequest() {
    GameScene1 *game1 = new GameScene1(widget, 800, 500, user, dataFile, false, office->currentAquariumState, true);
    miniGameView = new QGraphicsView(game1);
    miniGameView->setFixedSize(800, 500);
    miniGameView->setHorizontalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
    miniGameView->setVerticalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
    addWidget(miniGameView);
    miniGameView->setFocus();
    miniGameView->move(100, 50);

    updateAquariumTimer->stop();

    office->inAMiniGame = true;

    aquariumBox->hide();
    aquariumDescription->hide();
    cleanAquarium->hide();
    cancelAquarium->hide();
}

void GameScene3::cancelAquariumRequest() {
    aquariumBox->hide();
    aquariumDescription->hide();
    cleanAquarium->hide();
    cancelAquarium->hide();
}

void GameScene3::handleReputation(int status) {
    reputationBox->hide();
    reputationDescription->hide();
    useReputation->hide();
    cancelReputation->hide();

    int index = (patientsIndex == 0) ? 19 : patientsIndex - 1;
    if (status == 1) {
        office->currentReputation = 0;

        patients[index]->motionState = Patient::GettingReady;
        patients[index]->statusState = Patient::Accepted;
    } else if (status == 0) {
        if (office->currentMiniGameState == 2) {
            patients[index]->motionState = Patient::Done;
            patients[index]->statusState = Patient::Unsatisfied;
        } else if (office->currentMiniGameState == 0) {
            patients[index]->motionState = Patient::Done;
            patients[index]->statusState = Patient::Rejected;

            if (office->currentReputation - office->levels[office->level]["incrementReputation"] > 0) {
                office->currentReputation -= office->levels[office->level]["incrementReputation"];
            } else {
                office->currentReputation = 0;
            }
        }

        int time = (rand() % 1000) + office->levels[office->level]["patientGenerationRate"] - 500;
        updatePatientsTimer->start(time);
    }
}

/**
 * @brief GameScene3::unpauseClicked
 *
 * A member function that unpauses the game (sending it into the 3..2..1 state).
 */
void GameScene3::unpauseClicked() {
   office->requestForUnpause = true;
}

/**
 * @brief GameScene3::quitClicked
 *
 * A member function that exists the game and goes back to the GamePage.
 */
void GameScene3::quitClicked() {
    saveProgress();
    saveFile();

    backToGamePage();

}

void GameScene3::showQuestion(){
    int randNumb = (rand() % (questionList.size()));
    description->setText(questionList[randNumb]);

    patientBox->show();
    description->show();
    choiceA->show();
    choiceB->show();
    choiceC->show();
}

void GameScene3::answerClicked(int answer){
    int index = (patientsIndex == 0) ? 19 : patientsIndex - 1;
    if (patients[index] != NULL) {
        patients[index]->motionState = Patient::Leaving;
    } else {
        return;
    }

    patientBox->hide();
    description->hide();
    choiceA->hide();
    choiceB->hide();
    choiceC->hide();

    if(answer == questions[description->text()]){
        if (office->currentReputation + office->levels[office->level]["incrementReputation"] < office->levels[office->level]["maxReputation"]) {
            office->currentReputation += office->levels[office->level]["incrementReputation"];
        } else {
            office->currentReputation = office->levels[office->level]["maxReputation"];
        }
    }

    int time = (rand() % 1000) + office->levels[office->level]["patientGenerationRate"] - 500;
    updatePatientsTimer->start(time);
}

/**
 * @brief GameScene3::saveProgressHelper
 *
 * A member function that saves all game state parameters.
 *
 * @param QJsonObject &saveObject that helps save all the current game state parameters.
 */
void GameScene3::saveProgressHelper(QJsonObject &saveObject) const
{
    // Add office fields
    QJsonObject office;

    office["level"] = this->office->level;

    office["currentReputation"] = this->office->currentReputation;
    office["currentTime"] = this->office->currentTime;
    office["currentAquariumState"] = this->office->currentAquariumState;

    office["score"] = this->office->score;

    saveObject["office"] = office;

    // Add patients fields
    QJsonArray patients;

    for (int i = 0; i < 20; i++) {
        if (this->patients[i] != NULL) {
            QJsonObject currentPatient;

            currentPatient["x"] = this->patients[i]->x();
            currentPatient["y"] = this->patients[i]->y();
            currentPatient["type"] = 3 * (this->patients[i]->type - 1) + this->patients[i]->diff;
            currentPatient["motionState"] = this->patients[i]->getMotionState();
            currentPatient["statusState"] = this->patients[i]->getStatusState();

            patients.append(currentPatient);
        }
    }

    if (!patients.empty()) {
        saveObject["patients"] = patients;
    }

    // Add paused times
    QJsonObject pausedTimes;

    pausedTimes["pausedTimeUpdater"] = this->pausedTimeUpdater;
    if (this->pausedUpdatePatientsTimer > 0) {
        pausedTimes["pausedUpdatePatientsTimer"] = this->pausedUpdatePatientsTimer;
    }
    if (this->pausedUpdateAquariumTimer > 0) {
        pausedTimes["pausedUpdateAquariumTimer"] = this->pausedUpdateAquariumTimer;
    }

    saveObject["pausedTimes"] = pausedTimes;
}

/**
 * @brief GameScene3::saveScoreHelper
 *
 * A member function that saves the current score alongside the date.
 *
 * @param QJsonObject &saveObject that helps save data related to the score.
 */
void GameScene3::saveScoreHelper(QJsonObject &saveObject) const
{
    saveObject["score"] = office->score;
    saveObject["day"] = QDate::currentDate().day();
    saveObject["month"] = QDate::currentDate().month();
    saveObject["year"] = QDate::currentDate().year();
}

/**
 * @brief GameScene3::unpauseGame
 *
 * A member function that unpaused the game after the 3..2..1.. state.
 */
void GameScene3::unpauseGame() {
    time->restart();

    timeUpdater->setSingleShot(true);

    timeUpdater->start(pausedTimeUpdater);
    if (pausedUpdatePatientsTimer > 0) {
        updatePatientsTimer->start(pausedUpdatePatientsTimer);
        pausedUpdatePatientsTimer = 0;
    }
    if (pausedUpdateAquariumTimer > 0) {
        updateAquariumTimer->start(pausedUpdateAquariumTimer);
        pausedUpdateAquariumTimer = 0;
    }

    greyForeground->hide();
    unpauseLabel->hide();

    office->gamePaused = false;
    office->requestForUnpause = false;
}

/**
 * @brief GameScene3::checkGameState
 *
 * A member function that checks and updates the game's state.
 */
void GameScene3::checkGameState() {
    // Check if game paused
    if (office->gamePaused) {
       // Pause everything. We need those stats anw for the save functionality

        if (office->requestForUnpause) {
            if (!justPaused) {
                unpauseTimer->start(3000);

                justPaused = true;

                greyForeground->hide();
                unpause->hide();
                quit->hide();

                unpauseLabel->show();
            }

            if (unpauseTimer->isActive()) {
                int secs = (unpauseTimer->remainingTime()) / 1000;
                secs = secs % 60 + 1;
                if (secs == 4) {
                    secs = 3;
                }

                unpauseLabel->setText(QString("%1")
                .arg(secs, 2, 10, QLatin1Char('0')) );
            }
        } else {
            if (justPaused) {
                pausedTime += time->elapsed();

                if (unpauseTimer->isActive()) {
                    unpauseTimer->stop();
                }

                pausedTimeUpdater = timeUpdater->remainingTime();

                if (updatePatientsTimer->isActive()) {
                    pausedUpdatePatientsTimer = updatePatientsTimer->remainingTime();

                    if (pausedUpdatePatientsTimer < 0) {
                        pausedUpdatePatientsTimer = 0;
                    }

                    updatePatientsTimer->stop();
                }
                if (updateAquariumTimer->isActive()) {
                    pausedUpdateAquariumTimer = updateAquariumTimer->remainingTime();

                    if (pausedUpdateAquariumTimer < 0) {
                        pausedUpdateAquariumTimer = 0;
                    }

                    updateAquariumTimer->stop();
                }

                if (pausedTimeUpdater < 0) {
                    pausedTimeUpdater = 0;
                }

                timeUpdater->stop();

                justPaused = false;

                unpauseLabel->hide();

                greyForeground->setStyleSheet("background-color: rgba(105, 105, 105, 100);");
                greyForeground->show();
                unpause->show();
                quit->show();
            }
        }

        return;
    }

    // Update reputation meter view
    pixmapNeedle->setRotation(180 * office->currentReputation / office->levels[office->level]["maxReputation"]);

    // Check if Aquarium clicked
    int index = (patientsIndex == 0) ? 19 : patientsIndex - 1;
    if (aquarium->hasFocus() && !office->inAMiniGame) {
        if (office->currentAquariumState > 0) {
            aquariumBox->show();
            aquariumDescription->show();
            cleanAquarium->show();
            cancelAquarium->show();
        }
    } else if (office->inAMiniGame && patients[index]->motionState != Patient::InProgress) {
        GameScene1* game1 = (GameScene1*) miniGameView->scene();
        office->currentMiniGameScore = game1->getCurrentScore();

        if (!game1->hasFocus()) {
            office->inAMiniGame = false;
            office->currentMiniGameState = game1->getLevelState();

            if (office->currentMiniGameState == 1) {
                office->score += office->currentMiniGameScore;
                office->currentAquariumState = 0;
            }

            office->currentMiniGameScore = 0;

            int time = (rand() % 1000) + office->levels[office->level]["dirtinessRate"] - 500;
            updateAquariumTimer->start(time);
        }
    }

    if (!office->inAMiniGame && !office->hasFocus()) {
        office->setFocus();
    }

    updateAquariumImage();

    // Remove patients
    for (int i = 0; i < 20; i++) {
        if (patients[i] != NULL && patients[i]->toDelete) {
            delete patients[i];
            patients[i] = NULL;
        }
    }

    // Hide patients
    for (int i = 0; i < 20; i++) {
        if (patients[i] != NULL && patients[i]->motionState == Patient::Left) {
            patients[i]->hide();
        }
    }

    // Check patient's status
    index = (patientsIndex == 0) ? 19 : patientsIndex - 1;
    if (patients[index] != NULL) {
        if (patients[index]->motionState == Patient::Waiting) {
            patientBox->show();
            description->setText("Hello Dr."+ user->username +" \n I need your help can you please check my teeth" +
                                                               "my case is of level: " + QString::number(patients[index]->type) + " and difficulty " + QString::number(patients[index]->diff) +"\n will you please help me?");
            description->show();
            accept->show();
            reject->show();
        } else if (patients[index]->motionState == Patient::Ready) {
            office->inAMiniGame = true;

            //Enter game2
            //Games shouldn't be able to be saved. It should show only the exit button that makes it like rejecting the patient (not losing the game).
            GameScene2 *game2 = new GameScene2(widget, 800, 500, user, dataFile, false, patients[index]->type, patients[index]->diff, true);
            miniGameView = new QGraphicsView(game2);
            miniGameView->setFixedSize(800, 500);
            miniGameView->setHorizontalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
            miniGameView->setVerticalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
            addWidget(miniGameView);
            miniGameView->setFocus();
            miniGameView->move(100, 50);

            patients[index]->motionState = Patient::InProgress;
        } else if (patients[index]->motionState == Patient::InProgress) {         
            GameScene2* game2 = (GameScene2*) miniGameView->scene();
            office->currentMiniGameScore = game2->getCurrentScore();

            if (!game2->hasFocus()) {
                office->inAMiniGame = false;
                office->currentMiniGameState = game2->getLevelState();

                if (office->currentMiniGameState == 1) {
                    patients[index]->motionState = Patient::Done;
                    patients[index]->statusState = Patient::Satisfied;
                    office->score += office->currentMiniGameScore;
                } else {
                    // Check if reputation meter full
                    if (office->currentReputation == office->levels[office->level]["maxReputation"]) {
                        reputationBox->show();
                        useReputation->show();
                        cancelReputation->show();
                        reputationDescription->show();
                    } else {
                        if (office->currentMiniGameState == 2) {
                            patients[index]->motionState = Patient::Done;
                            patients[index]->statusState = Patient::Unsatisfied;
                        } else if (office->currentMiniGameState == 0) {
                            patients[index]->motionState = Patient::Done;
                            patients[index]->statusState = Patient::Rejected;

                            if (office->currentReputation - office->levels[office->level]["incrementReputation"] > 0) {
                                office->currentReputation -= office->levels[office->level]["incrementReputation"];
                            } else {
                                office->currentReputation = 0;
                            }
                        }

                        int time = (rand() % 1000) + office->levels[office->level]["patientGenerationRate"] - 500;
                        updatePatientsTimer->start(time);
                    }
                }

                office->currentMiniGameScore = 0;
            }
        } else if (patients[index]->motionState == Patient::ReadyForAdvice) {
            patients[index]->motionState = Patient::ReceivingAdvice;
            showQuestion();
        }
    }

    // Update score
    scoreLabel->setText(QStringLiteral("Score: %1").arg(office->score));
    scoreLabel->adjustSize();

    // Update level
    levelLabel->setText(QStringLiteral("Level: %1").arg(office->level));
    levelLabel->adjustSize();

    // Check if time is up
    if ((time->elapsed() + pausedTime) / office->levels[office->level]["minuteInMilliSeconds"] + office->levels[office->level]["startTime"]
            >= office->levels[office->level]["endTime"]) {
        int timeConverted = office->levels[office->level]["endTime"];
        int hours = (timeConverted / 60) % 60;

        int mins = timeConverted % 60;
        std::string dayTime;
        if (hours < 12) {
            dayTime = "AM";
        } else {
            if (hours != 12) {
                hours = hours - 12;
            }
            dayTime = "PM";
        }

        timeLabel->setText(QString("%1:%2 ")
        .arg(hours, 2, 10, QLatin1Char('0'))
        .arg(mins, 2, 10, QLatin1Char('0')) + QString::fromStdString(dayTime));

        office->currentTime = office->levels[office->level]["endTime"];

        gameOver(false);
    }
}

/**
 * @brief GameScene3::gameOver
 *
 * A member function that shows the game over screen.
 *
 * @param bool result whether the level was won or lost.
 */
void GameScene3::gameOver(bool result) {
    timeUpdater->stop();
    checkGameStateTimer->stop();

    for (int i = 0; i < 20; i++) {
        if (patients[i] != NULL) {
            delete patients[i];
            patients[i] = NULL;
        }
    }
    patientsIndex = 0;

    if (result) {
        office->score += office->levels[office->level]["endTime"] / office->currentTime - 1;
    }

    greyForeground->setStyleSheet("background-color: rgba(0, 0, 0, 255);");
    greyForeground->show();

    gameOverLabel->show();

    scoreLabel2->setText(QStringLiteral("Score: %1").arg(office->score));
    scoreLabel2->adjustSize();
    scoreLabel2->move((this->width() - scoreLabel2->width()) / 2, 330);
    scoreLabel2->show();

    quit2->show();

    if (result && office->level < 3) {
        setUpNextLevel();

        nextLevelButton->show();

        quit2->move(this->width() / 2 - 110, this->height() / 2 + 150);
        nextLevelButton->move(this->width() / 2 + 10, this->height() / 2 + 150);
    } else {
        quit2->move(this->width() / 2 - 60, this->height() / 2 + 150);

        saveScore();
        saveFile();
    }
}
