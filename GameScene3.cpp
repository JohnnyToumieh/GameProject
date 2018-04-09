#include "GameScene3.h"

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

    setBackgroundBrush(QBrush(QImage(":game2Background").scaledToHeight(height).scaledToWidth(width)));
    setSceneRect(0,0,width,height);

    aquarium = new QGraphicsPixmapItem();
    updateAquariumImage();
    aquarium->setPos(52, 313);
    addItem(aquarium);
    aquarium->setFlag(QGraphicsItem::ItemIsFocusable);

    timeLabel = new QLabel();
    timeLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 40px; }");
    timeLabel->move(this->width() / 2 - 30, 25);
    addWidget(timeLabel);

    pestilenceTimeLabel = new QLabel();
    pestilenceTimeLabel->setStyleSheet("QLabel { background-color : red; color : green; font: 60px; }");
    pestilenceTimeLabel->move(this->width() / 2 + 320, 160);
    addWidget(pestilenceTimeLabel);
    pestilenceTimeLabel->hide();

    pestilenceTimeLabel2 = new QLabel("Pestilence will get summoned after:");
    pestilenceTimeLabel2->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; font: 20px; }");
    pestilenceTimeLabel2->move(this->width() / 2 + 310, 110);
    pestilenceTimeLabel2->setWordWrap(true);
    addWidget(pestilenceTimeLabel2);
    pestilenceTimeLabel2->hide();

    unpauseLabel = new QLabel();
    unpauseLabel->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : white; font: 140px; }");
    unpauseLabel->move(this->width() / 2 - 90, this->height() / 2 - 90);
    unpauseLabel->setWordWrap(true);
    QGraphicsProxyWidget* proxyWidget = addWidget(unpauseLabel);
    proxyWidget->setZValue(10000);
    unpauseLabel->hide();

    levelLabel = new QLabel();
    levelLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 20px; }");
    levelLabel->move(300, 20);
    levelLabel->setText(QStringLiteral("Level: %1").arg(office->level));
    addWidget(levelLabel);

    scoreLabel = new QLabel();
    scoreLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 20px; }");
    scoreLabel->move(300, 50);
    scoreLabel->setText(QStringLiteral("Score: %1").arg(office->score));
    scoreLabel->adjustSize();
    addWidget(scoreLabel);

    pixmapNeedle = new QGraphicsPixmapItem();
    QPixmap *picNeedle  = new QPixmap(":needle");
    pixmapNeedle->setPixmap(picNeedle->scaled(80,20));
    pixmapNeedle->setPos(850, 80);
    pixmapNeedle->setTransformOriginPoint(pixmapNeedle->boundingRect().center().x() + 20,
                                               pixmapNeedle->boundingRect().center().y());
    addItem(pixmapNeedle);

    pixmapLife1 = new QGraphicsPixmapItem();
    pixmapLife2 = new QGraphicsPixmapItem();
    pixmapLife3 = new QGraphicsPixmapItem();
    QPixmap *picLife  = new QPixmap(":life");

    pixmapLife1->setPixmap(picLife->scaled(50,50));
    pixmapLife1->setPos(600,30);
    addItem(pixmapLife1);
    pixmapLife2->setPixmap(picLife->scaled(50,50));
    pixmapLife2->setPos(650,30);
    addItem(pixmapLife2);
    pixmapLife3->setPixmap(picLife->scaled(50,50));
    pixmapLife3->setPos(700,30);
    addItem(pixmapLife3);

    pixmapLifeList = new QGraphicsPixmapItem*[3];

    pixmapLifeList[0]=pixmapLife1;
    pixmapLifeList[1]=pixmapLife2;
    pixmapLifeList[2]=pixmapLife3;

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
            patients[patientsIndex] = new Patient(currentPatient["type"].toInt(), office);
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

    quit = new QPushButton("Quit");
    quit->move(this->width() / 2 - 30, this->height() / 2 + 10);
    proxyWidget = addWidget(quit);
    proxyWidget->setZValue(10000);
    quit->hide();

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

    nextLevelButton = new QPushButton("Next Level");
    nextLevelButton->move(this->width() / 2 - 50, this->height() / 2 + 150);
    proxyWidget = addWidget(nextLevelButton);
    proxyWidget->setZValue(10000);
    nextLevelButton->hide();

    patientBox = new QWidget();
    patientBox->setStyleSheet("background-color: rgba(105, 105, 105, 100);");
    addWidget(patientBox);
    patientBox->setFixedSize(250, 250);
    patientBox->move(this->width() / 2 - patientBox->width() / 2, this->height() / 2 - patientBox->height() / 2 + 50);
    patientBox->hide();

    accept = new QPushButton("Accept");
    addWidget(accept);
    accept->move(patientBox->x() + 30, patientBox->y() + patientBox->height() - 20 - accept->height());
    accept->hide();

    reject = new QPushButton("Reject");
    addWidget(reject);
    reject->move(patientBox->x() + patientBox->width() - 30 - reject->width(), patientBox->y() + patientBox->height() - 20 - reject->height());
    reject->hide();

    description = new QLabel("long long string");
    description->setWordWrap(true);
    addWidget(description);
    description->setFixedSize(patientBox->width() - 2 * 15, patientBox->height() - 2 * 15 - accept->height() - 2 * 15);
    description->move(patientBox->x() + 15, patientBox->y() + 15);
    description->hide();

    connect(unpause, SIGNAL(clicked()), SLOT(unpauseClicked()));
    connect(quit, SIGNAL(clicked()), SLOT(quitClicked()));
    connect(quit2, SIGNAL(clicked()), SLOT(quitClicked()));
    connect(nextLevelButton, SIGNAL(clicked()), SLOT(nextLevel()));

    QSignalMapper* signalMapper = new QSignalMapper(this);
    connect(accept, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(reject, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(accept, 1);
    signalMapper->setMapping(reject, 0);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(handlePatient(int))) ;

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
        updatePatientsTimer->start(pausedTimesSave["pausedUpdatePatientsTimer"].toInt());
        updateAquariumTimer->start(pausedTimesSave["pausedUpdateAquariumTimer"].toInt());
    } else {
        timeUpdater->start(500);
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
    int time = (rand() % 1000) + office->levels[office->level]["dirtinessRate"] - 500;
    updateAquariumTimer->start(time);

    if (office->currentAquariumState < 3) {
        office->currentAquariumState++;
    }
    updateAquariumImage();
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

    timeLabel->setText(QString("%1:%2")
    .arg(hours, 2, 10, QLatin1Char('0'))
    .arg(mins, 2, 10, QLatin1Char('0')) );

    office->currentTime = time->elapsed() + pausedTime;
}

/**
 * @brief GameScene3::updatePatients
 *
 * A member function that generates a new patients.
 */
void GameScene3::updatePatients(){
    int time = (rand() % 1000) + office->levels[office->level]["patientGenerationRate"] - 500;
    //updatePatientsTimer->start(time);

    if (patientsIndex > 19) {
        patientsIndex = 0;
    }

    int weight = (rand() % (office->levels[office->level]["patientWeight1"]
                        + office->levels[office->level]["patientWeight2"]
                        + office->levels[office->level]["patientWeight3"]));

    int type = 1;
    if (weight < office->levels[office->level]["patientWeight1"]) {
        type = 1;
    } else if (weight < office->levels[office->level]["patientWeight1"] + office->levels[office->level]["patientWeight2"]) {
        type = 2;
    } else {
        type = 3;
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
            currentPatient["type"] = this->patients[i]->type;

            patients.append(currentPatient);
        }
    }

    if (!patients.empty()) {
        saveObject["patients"] = patients;
    }

    // Add paused times
    QJsonObject pausedTimes;

    pausedTimes["pausedTimeUpdater"] = this->pausedTimeUpdater;
    pausedTimes["pausedUpdatePatientsTimer"] = this->pausedUpdatePatientsTimer;
    pausedTimes["pausedUpdateAquariumTimer"] = this->pausedUpdateAquariumTimer;

    saveObject["pausedTimes"] = pausedTimes;

    // Add needle orientation
    QJsonObject needle;

    needle["rotation"] = this->pixmapNeedle->rotation();

    saveObject["needle"] = needle;
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
    updatePatientsTimer->start(pausedUpdatePatientsTimer);

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
                pausedUpdatePatientsTimer = updatePatientsTimer->remainingTime();

                if (pausedTimeUpdater < 0) {
                    pausedTimeUpdater = 0;
                }
                if (pausedUpdatePatientsTimer < 0) {
                    pausedUpdatePatientsTimer = 0;
                }

                timeUpdater->stop();
                updatePatientsTimer->stop();

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

    // Check if Aquarium clicked
    if (aquarium->hasFocus() && !office->inAMiniGame) {
        if (office->currentAquariumState > 0) {
            office->inAMiniGame = true;

            GameScene1 *game1 = new GameScene1(widget, 800, 500, user, dataFile, false, office->currentAquariumState, true);
            miniGameView = new QGraphicsView(game1);
            miniGameView->setFixedSize(800, 500);
            miniGameView->setHorizontalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
            miniGameView->setVerticalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
            addWidget(miniGameView);
            miniGameView->setFocus();
            miniGameView->move(100, 50);
        }
    } else if (office->inAMiniGame) {
        if (!miniGameView->scene()->isActive()) {
            office->inAMiniGame = false;
        }
    } else {
        //timeLabel->setFocus();
    }

    // Remove patients
    for (int i = 0; i < 20; i++) {
        if (patients[i] != NULL && patients[i]->toDelete) {
            delete patients[i];
            patients[i] = NULL;
        }
    }

    // Check patient's status
    int index = (patientsIndex == 0) ? 19 : patientsIndex - 1;
    if (patients[index] != NULL) {
        if (patients[index]->motionState == Patient::Waiting) {
            patientBox->show();
            description->show();
            accept->show();
            reject->show();
        } else if (patients[index]->motionState == Patient::Ready) {
            patients[index]->motionState = Patient::InProgress;

            office->inAMiniGame = true;
            //Enter game2
            //Games shouldn't be able to be saved. It should show only the exit button that makes it like rejecting the patient (not losing the game).
            GameScene1 *game1 = new GameScene1(widget, 800, 500, user, dataFile, false, 1, true);
            miniGameView = new QGraphicsView(game1);
            miniGameView->setFixedSize(800, 500);
            miniGameView->setHorizontalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
            miniGameView->setVerticalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
            addWidget(miniGameView);
            miniGameView->setFocus();
            miniGameView->move(100, 50);
        } else if (patients[index]->motionState == Patient::InProgress) {
            miniGameView->setFocus();
            if (!miniGameView->scene()->isActive()) {
                patients[index]->motionState = Patient::Done;
                patients[index]->statusState = Patient::Unsatisfied;

                office->inAMiniGame = false;
            }
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
        timeLabel->setText(QString("%1:%2")
        .arg(hours, 2, 10, QLatin1Char('0'))
        .arg(mins, 2, 10, QLatin1Char('0')) );

        office->currentTime = office->levels[office->level]["endTime"];

        gameOver(false);
    }

    // Check if the office is cleaned
    if (office->currentReputation == office->levels[office->level]["maxReputation"]) {
        gameOver(true);
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
