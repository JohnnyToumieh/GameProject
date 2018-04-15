#include "GameScene2.h"

#include "Message.h"

/**
 *\file GameScene2.cpp
 *@brief contains GameScene2 class definition which represents the mini game of guessing the infected teeth.
 *
 * It handles saving of games, the creation and deletion of elements and tracking the game state.
 *
 */

/**
 * @brief GameScene2::GameScene2 constructor of GameScene2 class
 *
 * A constructor that sets up the game with all initial attributes.
 * @param QWidget *widget represents the main widget holding all items
 * @param int width the width of the window
 * @param int height the height of the window
 * @param User* user is the user signed in
 * @param QJsonObject dataFile holds the info of the user
 * @param bool resume determines is the game is being resumed
 * @param int level determines if the game should start at a specific level
 * @param int difficulity specify the diffuclity of the arriving patient mini game
 * @param int timelimit is the time limit of the mini game
 * @param int specialTimeLimit the time limit that if finished within the player is awarded
 * @param int points awarded when finished within time
 * @param int specialPoints awarded when finished within special time limit
 * @param bool isMiniGame specify that it is a minigame
 *
 */
GameScene2::GameScene2(QWidget *widget, int width, int height, User* user, QJsonObject dataFile, bool resume,
                       int level, int difficulity,
                       int timeLimit, int specialTimeLimit, int points, int specialPoints,
                       bool isMiniGame) : GameScene(widget, user, dataFile, 2, isMiniGame)
{
    srand(QTime::currentTime().msec());

    gameState = NotStarted;

    if (resume) {
        QJsonObject stateTracker2Save = read("stateTracker2").object();
        stateTracker2 = new StateTracker2(stateTracker2Save["level"].toInt(),
                stateTracker2Save["difficulity"].toInt(),
                stateTracker2Save["currentTime"].toInt(),
                stateTracker2Save["score"].toInt(), 0, 0, 0, 0);
    } else {
        stateTracker2 = new StateTracker2(level, difficulity, 0, 0, points, specialPoints, timeLimit, specialTimeLimit);
    }
    stateTracker2->setFlag(QGraphicsItem::ItemIsFocusable);
    addItem(stateTracker2);

    setBackgroundBrush(QBrush(QImage(":miniGame2Background").scaledToHeight(height).scaledToWidth(width)));
    setSceneRect(0,0,width,height);

    eyes = new QLabel();
    eyesAnimation = new QMovie(":movingEyes");
    eyesAnimation->setScaledSize(QSize(200, 200));
    eyes->setMovie(eyesAnimation);
    addWidget(eyes);
    eyes->move(this->width() / 2 - 100 - 150, this->height() / 2 - 200);

    mouth = new QGraphicsPixmapItem();
    QPixmap* picMouth  = new QPixmap(":mouth");
    mouth->setPixmap(picMouth->scaled(400, 300));
    addItem(mouth);
    mouth->setPos(this->width() / 2 - 200 - 150, this->height() / 2 - 50);
    mouth->setFlag(QGraphicsItem::ItemIsFocusable);

    QSignalMapper* signalMapper = new QSignalMapper(this);
    QSignalMapper* signalMapper1 = new QSignalMapper(this);

    if (stateTracker2->level == 3) {
        tool = new Tool*[3];
        toolClicked = 0;

        for (int i=0;i<3;i++){
            tool[i] = new Tool(i+1);
            tool[i]->setFocusPolicy(Qt::ClickFocus);
            tool[i]->setStyleSheet("QLabel { border-radius: 4px; border: 3px solid black; }");
            if(i==0)
                tool[i]->move(450,200);
            if(i==1)
                tool[i]->move(450,280);
            if(i==2)
                tool[i]->move(450,360);

            addWidget(tool[i]);

            connect(tool[i], SIGNAL(clicked()), signalMapper1, SLOT(map()));
            signalMapper1->setMapping(tool[i], i);
        }

        connect(signalMapper1, SIGNAL(mapped(int)), this, SLOT(updateToolClicked(int)));
    }

    upperTeeth = new Tooth*[6];
    for (int i = 0; i < 6; i++) {
        upperTeeth[i] = new Tooth();
       // upperTeeth[i]->setStyleSheet("QLabel { background-color : white; }");
        addWidget(upperTeeth[i]);
        upperTeeth[i]->setFocusPolicy(Qt::ClickFocus);
        if (i == 0 || i == 5) {
            upperTeeth[i]->setFixedSize(30, 51);
            upperTeeth[i]->move(this->width() / 2 - 157 - 150 + 56 * i, this->height() / 2 + 26);
        }
        if (i == 1 || i == 4) {
            upperTeeth[i]->setFixedSize(50, 50);
            upperTeeth[i]->move(this->width() / 2 - 125 - 150 + 65.5 * (i - 1), this->height() / 2 + 20);
        }
        if (i == 2 || i == 3) {
            upperTeeth[i]->setFixedSize(65, 50);
            upperTeeth[i]->move(this->width() / 2 - 70 - 150 + 71 * (i - 2), this->height() / 2 + 18);
        }
        connect(upperTeeth[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(upperTeeth[i], i);
    }
    upperTeethIndex = 0;

    lowerTeeth = new Tooth*[6];
    for (int i = 0; i < 6; i++) {
        lowerTeeth[i] = new Tooth();
    //    lowerTeeth[i]->setStyleSheet("QLabel { background-color : white; }");
        addWidget(lowerTeeth[i]);
        lowerTeeth[i]->setFocusPolicy(Qt::ClickFocus);
        if (i == 0 || i == 5) {
            lowerTeeth[i]->setFixedSize(30, 51);
            lowerTeeth[i]->move(this->width() / 2 - 157 - 150 + 56 * i, this->height() / 2 + 124);
        }
        if (i == 1 || i == 4) {
            lowerTeeth[i]->setFixedSize(50, 50);
            lowerTeeth[i]->move(this->width() / 2 - 125 - 150 + 65.5 * (i - 1), this->height() / 2 + 128);
        }
        if (i == 2 || i == 3) {
            lowerTeeth[i]->setFixedSize(65, 50);
            lowerTeeth[i]->move(this->width() / 2 - 70 - 150 + 71 * (i - 2), this->height() / 2 + 132);
        }
        connect(lowerTeeth[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(lowerTeeth[i], i + 6);
    }
    lowerTeethIndex = 0;

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(toothClicked(int)));

    start = new QPushButton("Start");
    addWidget(start);
    start->resize(150, 70);
    start->move(this->width() / 2 + 150, this->height() / 2 + 70);
    start->setFocusPolicy(Qt::NoFocus);

    connect(start, SIGNAL(clicked()), SLOT(startClicked()));

    goLabel = new QLabel("GO!");
    goLabel->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; font: 80px; }");
    goLabel->move(this->width() / 2 + 150, this->height() / 2 + 70);
    addWidget(goLabel);
    goLabel->hide();

    timeLabel = new QLabel("00:00");
    timeLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 40px; }");
    addWidget(timeLabel);
    timeLabel->move(this->width() / 2 + 150, this->height() / 2 - 150);

    levelLabel = new QLabel();
    levelLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 20px; }");
    levelLabel->setText(QStringLiteral("Level: %1").arg(stateTracker2->level));
    addWidget(levelLabel);
    levelLabel->move(this->width() / 2 + 150, this->height() / 2 - 50);

    difficulityLabel = new QLabel();
    difficulityLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 20px; }");
    difficulityLabel->setText(QStringLiteral("Diff: %1").arg(stateTracker2->difficulity));
    addWidget(difficulityLabel);
    difficulityLabel->move(this->width() / 2 + 150, this->height() / 2 - 20);

    scoreLabel = new QLabel();
    scoreLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 20px; }");
    scoreLabel->setText(QStringLiteral("Score: %1").arg(stateTracker2->score));
    scoreLabel->adjustSize();
    addWidget(scoreLabel);
    scoreLabel->move(this->width() / 2 + 150, this->height() / 2 + 10);

    unpauseLabel = new QLabel();
    unpauseLabel->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; font: 140px; }");
    unpauseLabel->move(this->width() / 2 - 90, this->height() / 2 - 90);
    unpauseLabel->setWordWrap(true);
    QGraphicsProxyWidget* proxyWidget = addWidget(unpauseLabel);
    proxyWidget->setZValue(10000);
    unpauseLabel->hide();

    if (resume) {
         pausedTime = stateTracker2->currentTime;
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
    quit->move(this->width() / 2 - 30, this->height() / 2 + 30);
    proxyWidget = addWidget(quit);
    proxyWidget->setZValue(10000);
    quit->hide();
    quit->setFocusPolicy(Qt::NoFocus);

    gameOverLabel = new QLabel("GAME OVER");
    gameOverLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 100px; }");
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
    quit2->move(this->width() / 2 - 60, this->height() / 2 + 200);
    proxyWidget = addWidget(quit2);
    proxyWidget->setZValue(10000);
    quit2->hide();
    quit2->setFocusPolicy(Qt::NoFocus);

    nextLevelButton = new QPushButton("Next Level");
    nextLevelButton->move(this->width() / 2 - 50, this->height() / 2 + 200);
    proxyWidget = addWidget(nextLevelButton);
    proxyWidget->setZValue(10000);
    nextLevelButton->hide();
    nextLevelButton->setFocusPolicy(Qt::NoFocus);

    QObject::connect(unpause, SIGNAL(clicked()), SLOT(unpauseClicked()));
    QObject::connect(quit, SIGNAL(clicked()), SLOT(quitClicked()));
    QObject::connect(quit2, SIGNAL(clicked()), SLOT(quitClicked()));
    QObject::connect(nextLevelButton, SIGNAL(clicked()), SLOT(nextLevel()));

    checkGameStateTimer = new QTimer(this);
    connect(checkGameStateTimer, SIGNAL(timeout()), this, SLOT(checkGameState()));
    checkGameStateTimer->start(100);

    time = new QTime();
    time->start();

    unpauseTimer = new QTimer(this);
    connect(unpauseTimer, SIGNAL(timeout()), this, SLOT(unpauseGame()));
    timeUpdater = new QTimer(this);
    connect(timeUpdater, SIGNAL(timeout()), this, SLOT(updateTimer()));
    toothUpdater = new QTimer(this);
    connect(toothUpdater, SIGNAL(timeout()), this, SLOT(highlightTooth()));
    teethUpdater = new QTimer(this);
    connect(teethUpdater, SIGNAL(timeout()), this, SLOT(dehighlightAllTeeth()));

    unpauseTimer->setSingleShot(true);
    toothUpdater->setSingleShot(true);
    teethUpdater->setSingleShot(true);

    if (resume) {
        QJsonObject pausedTimesSave = read("pausedTimes").object();

        timeUpdater->setSingleShot(true);

        timeUpdater->start(pausedTimesSave["pausedTimeUpdater"].toInt());
    } else {
        timeUpdater->start(500);
    }

    this->pausedTimeUpdater = 0;

    updateTimer();
}

/**
 * @brief GameScene2::updateToolClicked
 *
 * A member function that handles clicking tools in level 3
 */
void GameScene2::updateToolClicked(int toolIndex){
    toolClicked = toolIndex+2;

    for (int i = 0; i < 3; i++) {
        if (i == toolIndex) {
            tool[i]->setStyleSheet("QLabel { border-radius: 4px; border: 3px solid green; }");
        } else {
            tool[i]->setStyleSheet("QLabel { border-radius: 4px; border: 3px solid black; }");
        }
    }
}

/**
 * @brief GameScene2::startClicked
 *
 * A member function that handles clicking start button in the minigame.
 * where infected teeth will be displayed according to the level
 */
void GameScene2::startClicked() {
    start->hide();

    orderSize = 0;
    if (stateTracker2->difficulity == 1) {
        orderSize = stateTracker2->levels[stateTracker2->level]["difficulity1NumberOfTeeth"];
    } else if (stateTracker2->difficulity == 2) {
        orderSize = stateTracker2->levels[stateTracker2->level]["difficulity2NumberOfTeeth"];
    } else if (stateTracker2->difficulity == 3) {
        orderSize = stateTracker2->levels[stateTracker2->level]["difficulity3NumberOfTeeth"];
    }
    orderSpeed = 0;
    if (stateTracker2->difficulity == 1) {
        orderSpeed = stateTracker2->levels[stateTracker2->level]["difficulity1SpeedOfTeeth"];
    } else if (stateTracker2->difficulity == 2) {
        orderSpeed = stateTracker2->levels[stateTracker2->level]["difficulity2SpeedOfTeeth"];
    } else if (stateTracker2->difficulity == 3) {
        orderSpeed = stateTracker2->levels[stateTracker2->level]["difficulity3SpeedOfTeeth"];
    }

    order = new int[orderSize];
    guessedOrder = new bool[orderSize];
    infectionTypes = new int[orderSize];

    for (int i = 0; i < orderSize; i++) {
        bool isDuplicate = true;
        while (isDuplicate) {
            int randNumb = (rand() % 12);
            int j = 0;
            for (j = 0; j < i; j++) {
                if (order[j] == randNumb) {
                    break;
                }
            }
            if (j == i) {
                order[i] = randNumb;
                isDuplicate = false;
            }
        }
        guessedOrder[i] = false;
    }

    orderIndex = 0;

    if (stateTracker2->level == 1) {
        highlightAllTeeth();
        teethUpdater->start(orderSpeed);
    } else if (stateTracker2->level == 2) {
        highlightTooth();
        toothUpdater->start(orderSpeed);
    }else if(stateTracker2->level == 3){

        for (int i = 0; i < orderSize; i++) {
                int randNumb = (rand() % 3)+2;
                    infectionTypes[i] = randNumb;
        }

        highlightTooth();
        toothUpdater->start(orderSpeed);
    }

    gameState = DisplayingTeeth;
}

/**
 * @brief GameScene2::toothClicked
 *
 * A member function that handles clicking a tooth when guessing
 * the function check if the tooth is correct and responds
 *
 * @param int toothIndex the index of the clicked tooth
 */
void GameScene2::toothClicked(int toothIndex) {
    if (gameState == GuessingTeeth && !stateTracker2->gamePaused) {
        if (stateTracker2->level == 1) {
            int j = 0;
            for (j = 0; j < orderSize; j++) {
                if (order[j] == toothIndex && !guessedOrder[j]) {
                    if (toothIndex < 6) {
                        upperTeeth[toothIndex]->setType(5);
                    } else {
                        lowerTeeth[toothIndex - 6]->setType(5);
                    }

                    guessedOrder[j] = true;
                    break;
                }
            }
            if (j == orderSize) {
                if (toothIndex < 6) {
                    upperTeeth[toothIndex]->setType(6);
                } else {
                    lowerTeeth[toothIndex - 6]->setType(6);
                }

                gameState = GameLost;
                gameOver(false);
            }
        } else if (stateTracker2->level == 2) {
            if (order[orderIndex] == toothIndex) {
                if (toothIndex < 6) {
                    upperTeeth[toothIndex]->setType(5);
                } else {
                    lowerTeeth[toothIndex - 6]->setType(5);
                }

                guessedOrder[orderIndex] = true;

                orderIndex++;
            } else {
                if (toothIndex < 6) {
                    upperTeeth[toothIndex]->setType(6);
                } else {
                    lowerTeeth[toothIndex - 6]->setType(6);
                }

                gameState = GameLost;
                gameOver(false);
            }
        }else if(stateTracker2->level == 3){
            if (order[orderIndex] == toothIndex && toolClicked == infectionTypes[orderIndex]) {
                if (toothIndex < 6) {
                    upperTeeth[toothIndex]->setType(5);
                } else {
                    lowerTeeth[toothIndex - 6]->setType(5);
                }

                guessedOrder[orderIndex] = true;

                orderIndex++;
            } else {
                if (toothIndex < 6) {
                    upperTeeth[toothIndex]->setType(6);
                } else {
                    lowerTeeth[toothIndex - 6]->setType(6);
                }

                gameState = GameLost;
                gameOver(false);
            }

        }

        // Check if we guessed all teeth
        int i = 0;
        for (i = 0; i < orderSize; i++) {
            if (!guessedOrder[i]) {
                break;
            }
        }

        if (i == orderSize) {
            orderIndex = 0;
            gameState = GameWon;
            gameOver(true);
        }
    }
}

/**
 * @brief GameScene2::highlightAllTeeth
 *
 * A member function that is called in level1 to highlight
 * all the infected teeth at once
 *
 */
void GameScene2::highlightAllTeeth() {
    for (int i = 0; i < orderSize; i++) {
        if (order[i] < 6) {
            upperTeeth[order[i]]->setType(2);
        } else {
            lowerTeeth[order[i] - 6]->setType(2);
        }
    }
}

/**
 * @brief GameScene2::dehighlightAllTeeth
 *
 * A member function that is called to dehighlight all teeth
 * and reset indexes
 *
 */
void GameScene2::dehighlightAllTeeth() {
    resetAllTeeth();

    orderIndex = 0;

    goLabel->show();

    gameState = GuessingTeeth;
}

/**
 * @brief GameScene2::resetAllTeeth
 *
 * A member function that is called from dehighlightAllTeeth to clear colors
 * of all teeth
 *
 */
void GameScene2::resetAllTeeth() {
    for (int i = 0; i < 12; i++) {
        if (i < 6) {
            upperTeeth[i]->setType(1);
        } else {
            lowerTeeth[i - 6]->setType(1);
        }
    }
}

/**
 * @brief GameScene2::highlightTooth
 *
 * A member function that is called to highlight tooth in level 2 and 3 patients
 * it highlight tooth and record the order of tooth highlighted
 *
 */
void GameScene2::highlightTooth() {
    if (orderIndex > 0) {
        if (order[orderIndex - 1] < 6) {
            upperTeeth[order[orderIndex - 1]]->setType(1);
        } else {
            lowerTeeth[order[orderIndex - 1] - 6]->setType(1);
        }
    }

    if (orderIndex < orderSize) {
        if(stateTracker2->level==2){
            if (order[orderIndex] < 6) {
                upperTeeth[order[orderIndex]]->setType(2);
            } else {
                lowerTeeth[order[orderIndex] - 6]->setType(2);
            }
        }else if(stateTracker2->level==3){
            if (order[orderIndex] < 6) {
                upperTeeth[order[orderIndex]]->setType(infectionTypes[orderIndex]);
            } else {
                lowerTeeth[order[orderIndex] - 6]->setType(infectionTypes[orderIndex]);
            }
        }

        orderIndex++;

        toothUpdater->start(orderSpeed);
    } else {
        orderIndex = 0;

        goLabel->show();

        gameState = GuessingTeeth;
    }
}

/**
 * @brief GameScene2::getCurrentScore
 *
 * A member function that returns the score of the guessing minigame.
 * @return int score of the guessing minigame
 */
int GameScene2::getCurrentScore() {
    return stateTracker2->score;
}

/**
 * @brief GameScene2::getLevelState
 *
 * A member function that returns the level of the guessing minigame.
 * @return int level of the guessing minigame
 */
int GameScene2::getLevelState() {
    return stateTracker2->levels[stateTracker2->level]["levelState"];
}

/**
 * @brief GameScene2::nextLevel
 *
 * A member function that loads the next level and mainly sets the timers.
 */
void GameScene2::nextLevel() {
    scoreLabel2->hide();
    quit2->hide();
    greyForeground->hide();
    gameOverLabel->hide();
    nextLevelButton->hide();

    checkGameStateTimer->start(100);
    timeUpdater->start(500);

    pausedTime = 0;
    time->restart();
    updateTimer();
}

/**
 * @brief GameScene2::setUpNextLevel
 *
 * A member function that sets up the next level.
 */
void GameScene2::setUpNextLevel() {
    stateTracker2->level++;
    stateTracker2->currentTime = 0;

    resetAllTeeth();
    orderIndex = 0;

    goLabel->hide();
    start->show();

    for (int i = 0; i < orderSize; i++) {
        guessedOrder[i] = false;
    }

    gameState = NotStarted;
}

/**
 * @brief GameScene2::updateTimer
 *
 * A member function that updates time based labels.
 */
void GameScene2::updateTimer() {
    if (timeUpdater->isSingleShot()) {
        timeUpdater->setSingleShot(false);
        timeUpdater->start(500);
    }

    if (eyesAnimation->state() == QMovie::Running && eyesAnimation->currentFrameNumber() == 10) {
        eyesAnimation->setPaused(true);
    } else if (eyesAnimation->state() == QMovie::Paused){
        int ran = (rand() % 10);
        if (ran == 0) {
            eyesAnimation->setPaused(false);
        }
    } else if (eyesAnimation->state() == QMovie::NotRunning) {
        eyesAnimation->start();
    }

    int secs = (time->elapsed() + pausedTime) / 1000;
    int mins = (secs / 60) % 60;
    secs = secs % 60;

    timeLabel->setText(QString("%1:%2")
    .arg(mins, 2, 10, QLatin1Char('0'))
    .arg(secs, 2, 10, QLatin1Char('0')) );

    stateTracker2->currentTime = time->elapsed() + pausedTime;
}

/**
 * @brief GameScene2::unpauseClicked
 *
 * A member function that unpauses the game (sending it into the 3..2..1 state).
 */
void GameScene2::unpauseClicked() {
   stateTracker2->requestForUnpause = true;
}

/**
 * @brief GameScene2::quitClicked
 *
 * A member function that exists the game and goes back to the GamePage.
 */
void GameScene2::quitClicked() {
    if (!isMiniGame) {
        saveProgress();
        saveFile();
    }

    backToGamePage();
}

/**
 * @brief GameScene2::saveProgressHelper
 *
 * A member function that saves all game state parameters.
 *
 * @param QJsonObject &saveObject that helps save all the current game state parameters.
 */
void GameScene2::saveProgressHelper(QJsonObject &saveObject) const
{
    // Add stateTracker2 fields
    QJsonObject stateTracker2;

    stateTracker2["level"] = this->stateTracker2->level;
    stateTracker2["difficulity"] = this->stateTracker2->difficulity;

    stateTracker2["currentTime"] = this->stateTracker2->currentTime;

    stateTracker2["score"] = this->stateTracker2->score;

    saveObject["stateTracker2"] = stateTracker2;

    // Add paused times
    QJsonObject pausedTimes;

    pausedTimes["pausedTimeUpdater"] = this->pausedTimeUpdater;

    saveObject["pausedTimes"] = pausedTimes;
}

/**
 * @brief GameScene2::saveScoreHelper
 *
 * A member function that saves the current score alongside the date.
 *
 * @param QJsonObject &saveObject that helps save data related to the score.
 */
void GameScene2::saveScoreHelper(QJsonObject &saveObject) const
{
    saveObject["score"] = stateTracker2->score;
    saveObject["day"] = QDate::currentDate().day();
    saveObject["month"] = QDate::currentDate().month();
    saveObject["year"] = QDate::currentDate().year();
}

/**
 * @brief GameScene2::unpauseGame
 *
 * A member function that unpaused the game after the 3..2..1.. state.
 */
void GameScene2::unpauseGame() {
    time->restart();

    timeUpdater->setSingleShot(true);

    timeUpdater->start(pausedTimeUpdater);
    if (pausedTeethUpdater > 0) {
       teethUpdater->start(pausedTeethUpdater);
       pausedTeethUpdater = 0;
    }
    if (pausedToothUpdater > 0) {
       toothUpdater->start(pausedToothUpdater);
       pausedToothUpdater = 0;
    }

    greyForeground->hide();
    unpauseLabel->hide();

    stateTracker2->gamePaused = false;
    stateTracker2->requestForUnpause = false;
}

/**
 * @brief GameScene2::checkGameState
 *
 * A member function that checks and updates the game's state.
 */
void GameScene2::checkGameState() {
    // Check if game paused
    if (stateTracker2->gamePaused) {
       // Pause everything. We need those stats anw for the save functionality

        if (stateTracker2->requestForUnpause) {
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

                if (teethUpdater->isActive()) {
                    pausedTeethUpdater = teethUpdater->remainingTime();

                    if (pausedTeethUpdater < 0) {
                        pausedTeethUpdater = 0;
                    }

                    teethUpdater->stop();
                }

                if (toothUpdater->isActive()) {
                    pausedToothUpdater = toothUpdater->remainingTime();

                    if (pausedToothUpdater < 0) {
                        pausedToothUpdater = 0;
                    }

                    toothUpdater->stop();
                }

                pausedTimeUpdater = timeUpdater->remainingTime();

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

    stateTracker2->setFocus();

    // Update score
    scoreLabel->setText(QStringLiteral("Score: %1").arg(stateTracker2->score));
    scoreLabel->adjustSize();

    // Update level
    levelLabel->setText(QStringLiteral("Level: %1").arg(stateTracker2->level));
    levelLabel->adjustSize();

    // Update difficulity
    difficulityLabel->setText(QStringLiteral("Diff: %1").arg(stateTracker2->difficulity));
    difficulityLabel->adjustSize();

    // Check if time is up
    if (time->elapsed() + pausedTime >= stateTracker2->timeLimit) {
        stateTracker2->score = 0;

        int secs = stateTracker2->timeLimit / 1000;
        int mins = (secs / 60) % 60;
        secs = secs % 60;
        timeLabel->setText(QString("%1:%2")
        .arg(mins, 2, 10, QLatin1Char('0'))
        .arg(secs, 2, 10, QLatin1Char('0')) );

        stateTracker2->currentTime = stateTracker2->timeLimit;

        gameOver(false);
    } else if (time->elapsed() + pausedTime >= stateTracker2->specialTimeLimit) {
        stateTracker2->score = stateTracker2->points;
    } else {
        stateTracker2->score = stateTracker2->specialPoints;
    }
}

/**
 * @brief GameScene2::gameOver
 *
 * A member function that shows the game over screen.
 *
 * @param bool result whether the level was won or lost.
 */
void GameScene2::gameOver(bool result) {
    if (result) {
        stateTracker2->levels[stateTracker2->level]["levelState"] = 1;
    } else {
        stateTracker2->levels[stateTracker2->level]["levelState"] = 2;
    }

    timeUpdater->stop();

    greyForeground->setStyleSheet("background-color: rgba(0, 0, 0, 255);");
    greyForeground->show();

    gameOverLabel->show();

    scoreLabel2->setText(QStringLiteral("Score: %1").arg(stateTracker2->score));
    scoreLabel2->adjustSize();
    scoreLabel2->move((this->width() - scoreLabel2->width()) / 2, 330);
    scoreLabel2->show();

    quit2->show();

    if (result && stateTracker2->level < 3 && !isMiniGame) {
        setUpNextLevel();

        nextLevelButton->show();

        quit2->move(this->width() / 2 - 110, this->height() / 2 + 200);
        nextLevelButton->move(this->width() / 2 + 10, this->height() / 2 + 200);
    } else {
        quit2->move(this->width() / 2 - 60, this->height() / 2 + 200);

        if (!isMiniGame) {
            saveScore();
            saveFile();
        }
    }
}
