#include "GameScene2.h"

#include "Message.h"

/**
 *\file GameScene2.cpp
 *@brief contains GameScene2 class definition which represents the game 2 shell.
 *
 * It handles saving of games, the creation and deletion of elements and tracking the game state.
 *
 */

/**
 * @brief GameScene2::GameScene2 constructor of GameScene2 class
 *
 * A constructor that sets up the game with all initial attributes.
 * @param QWidget *widget represents the main widget holding all items
 * @param User* user is the user signed in
 * @param QJsonObject dataFile holds the info of the user
 * @param bool resume determines is the game is being resumed
 * @param int level determines if the game should start at a specific level
 */
GameScene2::GameScene2(QWidget *widget, int width, int height, User* user, QJsonObject dataFile, bool resume, int level, bool isMiniGame) : GameScene(widget, user, dataFile, 2, isMiniGame)
{
    srand(QTime::currentTime().msec());

    gameState = NotStarted;

    if (resume) {
        QJsonObject aquariumSave = read("aquarium").object();
        aquarium = new Aquarium(aquariumSave["level"].toInt(),
                aquariumSave["currentCleanliness"].toInt(),
                aquariumSave["currentTime"].toInt(),
                aquariumSave["score"].toInt());
    } else {
        aquarium = new Aquarium(level, 0, 0, 0);
    }

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

    upperTeeth = new QLabel*[6];
    for (int i = 0; i < 6; i++) {
        upperTeeth[i] = new QLabel();
        upperTeeth[i]->setStyleSheet("QLabel { background-color : white; }");
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
    }
    upperTeethIndex = 0;

    lowerTeeth = new QLabel*[6];
    for (int i = 0; i < 6; i++) {
        lowerTeeth[i] = new QLabel();
        lowerTeeth[i]->setStyleSheet("QLabel { background-color : white; }");
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
    }
    lowerTeethIndex = 0;

    start = new QPushButton("Start");
    addWidget(start);
    start->resize(150, 70);
    start->move(this->width() / 2 + 150, this->height() / 2 - 35);
    start->setFocusPolicy(Qt::NoFocus);

    connect(start, SIGNAL(clicked()), SLOT(startClicked()));

    toothUpdater = new QTimer();
    toothUpdater->setSingleShot(true);

    connect(toothUpdater, SIGNAL(timeout()), this, SLOT(highlightTooth()));

    teethUpdater = new QTimer();
    teethUpdater->setSingleShot(true);

    connect(teethUpdater, SIGNAL(timeout()), this, SLOT(dehighlightAllTeeth()));

    goLabel = new QLabel("GO!");
    goLabel->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; font: 80px; }");
    goLabel->move(this->width() / 2 + 150, this->height() / 2 - 35);
    addWidget(goLabel);
    goLabel->hide();

    timeLabel = new QLabel("00:00");
    timeLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 40px; }");
    addWidget(timeLabel);
    timeLabel->move(this->width() / 2 - timeLabel->width() / 2, this->height() / 24);

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
    levelLabel->setText(QStringLiteral("Level: %1").arg(aquarium->level));
    addWidget(levelLabel);

    scoreLabel = new QLabel();
    scoreLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 20px; }");
    scoreLabel->move(300, 50);
    scoreLabel->setText(QStringLiteral("Score: %1").arg(aquarium->score));
    scoreLabel->adjustSize();
    addWidget(scoreLabel);

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

    if (resume) {
         pausedTime = aquarium->currentTime;
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

    unpauseTimer->setSingleShot(true);

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

void GameScene2::startClicked() {
    start->hide();

    order[0] = 1;
    order[1] = 3;
    order[2] = 8;

    orderIndex = 0;

    if (aquarium->level == 1) {
        highlightAllTeeth();
        teethUpdater->start(5000);
    } else if (aquarium->level == 2) {
        highlightTooth();
        toothUpdater->start(3000);
    }

    gameState = DisplayingTeeth;
}

void GameScene2::highlightAllTeeth() {
    for (int i = 0; i < 3; i++) {
        if (order[i] < 6) {
            upperTeeth[order[i]]->setStyleSheet("QLabel { background-color : black; }");
        } else {
            lowerTeeth[order[i] - 6]->setStyleSheet("QLabel { background-color : black; }");
        }
    }
}

void GameScene2::dehighlightAllTeeth() {
    resetAllTeeth();

    orderIndex = 0;

    goLabel->show();

    gameState = GuessingTeeth;
}

void GameScene2::resetAllTeeth() {
    for (int i = 0; i < 12; i++) {
        if (i < 6) {
            upperTeeth[i]->setStyleSheet("QLabel { background-color : white; }");
        } else {
            lowerTeeth[i - 6]->setStyleSheet("QLabel { background-color : white; }");
        }
    }
}

void GameScene2::highlightTooth() {
    if (orderIndex > 0) {
        if (order[orderIndex - 1] < 6) {
            upperTeeth[order[orderIndex - 1]]->setStyleSheet("QLabel { background-color : white; }");
        } else {
            lowerTeeth[order[orderIndex - 1] - 6]->setStyleSheet("QLabel { background-color : white; }");
        }
    }

    if (orderIndex < 3) {
        if (order[orderIndex] < 6) {
            upperTeeth[order[orderIndex]]->setStyleSheet("QLabel { background-color : black; }");
        } else {
            lowerTeeth[order[orderIndex] - 6]->setStyleSheet("QLabel { background-color : black; }");
        }

        orderIndex++;

        toothUpdater->start(3000);
    } else {
        orderIndex = 0;

        goLabel->show();

        gameState = GuessingTeeth;
    }
}

int GameScene2::getCurrentScore() {
    return aquarium->score;
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
    aquarium->level++;
    aquarium->currentCleanliness = 0;
    aquarium->currentTime = 0;

    resetAllTeeth();
    orderIndex = 0;

    goLabel->hide();
    start->show();

    for (int i = 0; i < 3; i++) {
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

    aquarium->currentTime = time->elapsed() + pausedTime;
}

/**
 * @brief GameScene2::unpauseClicked
 *
 * A member function that unpauses the game (sending it into the 3..2..1 state).
 */
void GameScene2::unpauseClicked() {
   aquarium->requestForUnpause = true;
}

/**
 * @brief GameScene2::quitClicked
 *
 * A member function that exists the game and goes back to the GamePage.
 */
void GameScene2::quitClicked() {
    saveProgress();
    saveFile();

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
    // Add aquarium fields
    QJsonObject aquarium;

    aquarium["level"] = this->aquarium->level;

    aquarium["currentCleanliness"] = this->aquarium->currentCleanliness;
    aquarium["currentTime"] = this->aquarium->currentTime;

    aquarium["score"] = this->aquarium->score;

    saveObject["aquarium"] = aquarium;

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
    saveObject["score"] = aquarium->score;
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

    greyForeground->hide();
    unpauseLabel->hide();

    aquarium->gamePaused = false;
    aquarium->requestForUnpause = false;
}

/**
 * @brief GameScene2::checkGameState
 *
 * A member function that checks and updates the game's state.
 */
void GameScene2::checkGameState() {
    // Check if game paused
    if (aquarium->gamePaused) {
       // Pause everything. We need those stats anw for the save functionality

        if (aquarium->requestForUnpause) {
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

    if (gameState == GuessingTeeth) {
        for (int i = 0; i < 12; i++) {
            if ((i < 6 && upperTeeth[i]->hasFocus())
                    || (i >= 6 && lowerTeeth[i - 6]->hasFocus())) {
                if (aquarium->level == 1) {
                    int j = 0;
                    for (j = 0; j < 3; j++) {
                        if (order[j] == i && !guessedOrder[j]) {
                            if (i < 6) {
                                upperTeeth[i]->setStyleSheet("QLabel { background-color : green; }");
                            } else {
                                lowerTeeth[i - 6]->setStyleSheet("QLabel { background-color : green; }");
                            }

                            guessedOrder[j] = true;
                            break;
                        }
                    }
                    if (j == 3) {
                        if (i < 6) {
                            upperTeeth[i]->setStyleSheet("QLabel { background-color : red; }");
                        } else {
                            lowerTeeth[i - 6]->setStyleSheet("QLabel { background-color : red; }");
                        }

                        gameState = GameLost;
                        gameOver(false);
                    }
                } else if (aquarium->level == 2) {
                    if (order[orderIndex] == i) {
                        if (i < 6) {
                            upperTeeth[i]->setStyleSheet("QLabel { background-color : green; }");
                        } else {
                            lowerTeeth[i - 6]->setStyleSheet("QLabel { background-color : green; }");
                        }

                        guessedOrder[orderIndex] = true;

                        orderIndex++;
                    } else {
                        if (i < 6) {
                            upperTeeth[i]->setStyleSheet("QLabel { background-color : red; }");
                        } else {
                            lowerTeeth[i - 6]->setStyleSheet("QLabel { background-color : red; }");
                        }

                        gameState = GameLost;
                        gameOver(false);
                    }
                }
                break;
            }
        }

        // Check if we guessed all teeth

        int i = 0;
        for (i = 0; i < 3; i++) {
            if (!guessedOrder[i]) {
                break;
            }
        }

        if (i == 3) {
            orderIndex = 0;
            gameState = GameWon;
            gameOver(true);
        }
     }

    mouth->setFocus();

    // Update score
    scoreLabel->setText(QStringLiteral("Score: %1").arg(aquarium->score));
    scoreLabel->adjustSize();

    // Update level
    levelLabel->setText(QStringLiteral("Level: %1").arg(aquarium->level));
    levelLabel->adjustSize();

    // Check if time is up
    if (time->elapsed() + pausedTime >= aquarium->levels[aquarium->level]["maxTime"]) {
        int secs = aquarium->levels[aquarium->level]["maxTime"] / 1000;
        int mins = (secs / 60) % 60;
        secs = secs % 60;
        timeLabel->setText(QString("%1:%2")
        .arg(mins, 2, 10, QLatin1Char('0'))
        .arg(secs, 2, 10, QLatin1Char('0')) );

        aquarium->currentTime = aquarium->levels[aquarium->level]["maxTime"];

        gameOver(false);
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
    timeUpdater->stop();

    if (result) {
        aquarium->score += aquarium->levels[aquarium->level]["maxTime"] / aquarium->currentTime - 1;
    }

    greyForeground->setStyleSheet("background-color: rgba(0, 0, 0, 255);");
    greyForeground->show();

    gameOverLabel->show();

    scoreLabel2->setText(QStringLiteral("Score: %1").arg(aquarium->score));
    scoreLabel2->adjustSize();
    scoreLabel2->move((this->width() - scoreLabel2->width()) / 2, 330);
    scoreLabel2->show();

    quit2->show();

    if (result && aquarium->level < 3) {
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
