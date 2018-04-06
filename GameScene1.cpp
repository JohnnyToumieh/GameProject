#include "GameScene1.h"

#include "Message.h"

/**
 *\file GameScene1.cpp
 *@brief contains GameScene1 class definition which represents the game 1 shell.
 *
 * It handles saving of games, the creation and deletion of elements and tracking the game state.
 *
 */

/**
 * @brief GameScene1::GameScene1 constructor of GameScene1 class
 *
 * A constructor that sets up the game with all initial attributes.
 * @param QWidget *widget represents the main widget holding all items
 * @param User* user is the user signed in
 * @param QJsonObject dataFile holds the info of the user
 * @param bool resume determines is the game is being resumed
 * @param int level determines if the game should start at a specific level
 */
GameScene1::GameScene1(QWidget *widget, User* user, QJsonObject dataFile, bool resume, int level) : GameScene(widget, user, dataFile, 1)
{
    srand(QTime::currentTime().msec());

    if (resume) {
        QJsonObject aquariumSave = read("aquarium").object();
        aquarium = new Aquarium(aquariumSave["level"].toInt(),
                aquariumSave["currentCleanliness"].toInt(),
                aquariumSave["currentTime"].toInt(),
                aquariumSave["score"].toInt());
    } else {
        aquarium = new Aquarium(level, 0, 0, 0);
    }

    setBackgroundBrush(QBrush(QImage(":game1Background").scaledToHeight(600).scaledToWidth(1000)));
    setSceneRect(0,0,1000,600);

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
    levelLabel->setText(QStringLiteral("Level: %1").arg(aquarium->level));
    addWidget(levelLabel);

    scoreLabel = new QLabel();
    scoreLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 20px; }");
    scoreLabel->move(300, 50);
    scoreLabel->setText(QStringLiteral("Score: %1").arg(aquarium->score));
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
    QPixmap *greenColor = new QPixmap("needle.png");
    greenColor->fill(Qt::green);
    greenColorItem->setPixmap(greenColor->scaled((230 / aquarium->levels[aquarium->level]["maxCleanliness"]) * aquarium->currentCleanliness, 20));
    addItem(greenColorItem);

    spongeBob = new SpongeBob(aquarium, pixmapNeedle, pixmapLifeList);

    if (resume) {
        QJsonObject spongeBobSave = read("spongeBob").object();
        spongeBob->immunityLevel = spongeBobSave["immunityLevel"].toInt();
        spongeBob->immunityLevelDegree = spongeBobSave["immunityLevelDegree"].toInt();
        spongeBob->lives = spongeBobSave["lives"].toInt();
        spongeBob->setX(spongeBobSave["x"].toDouble());
        spongeBob->setY(spongeBobSave["y"].toDouble());
        spongeBob->numCollisionsWithBacterias[0] = (spongeBobSave["numCollisionsWithBacterias"].toObject())["numbBacteriaCollisions1"].toInt();
        spongeBob->numCollisionsWithBacterias[1] = (spongeBobSave["numCollisionsWithBacterias"].toObject())["numbBacteriaCollisions2"].toInt();
        spongeBob->numCollisionsWithBacterias[2] = (spongeBobSave["numCollisionsWithBacterias"].toObject())["numbBacteriaCollisions3"].toInt();

        if (spongeBob->lives < 3) {
            removeItem(pixmapLifeList[0]);
        }
        if (spongeBob->lives < 2) {
            removeItem(pixmapLifeList[1]);
        }

        QJsonObject needleSave = read("needle").object();
        pixmapNeedle->setRotation(needleSave["rotation"].toInt());
    }

    addItem(spongeBob);

    spongeBob->setFlag(QGraphicsItem::ItemIsFocusable);
    spongeBob->setFocus();

    bacterias = new Bacteria*[20];
    for (int i = 0; i < 20; i++) {
        bacterias[i] = NULL;
    }
    bacteriasIndex = 0;

    updateBacteriasTimer = new QTimer(this);

    if (resume) {
        QJsonArray bacteriasSave = read("bacterias").array();
        for (bacteriasIndex = 0; bacteriasIndex < bacteriasSave.size(); bacteriasIndex++) {
            QJsonObject currentBacteria = bacteriasSave[bacteriasIndex].toObject();
            bacterias[bacteriasIndex] = new Bacteria(currentBacteria["type"].toInt(),spongeBob,aquarium,greenColorItem,pixmapLifeList);
            bacterias[bacteriasIndex]->speed = currentBacteria["speed"].toInt();
            bacterias[bacteriasIndex]->setX(currentBacteria["x"].toDouble());
            bacterias[bacteriasIndex]->setY(currentBacteria["y"].toDouble());
            bacterias[bacteriasIndex]->baseY = currentBacteria["y"].toDouble();
            addItem(bacterias[bacteriasIndex]);
        }
    } else {
        updateBacterias();
    }

    viruses = new Virus*[10];
    for (int i = 0; i < 10; i++) {
        viruses[i] = NULL;
    }
    virusesIndex = 0;

    virusTimer = new QTimer(this);
    pestilenceTimer = new QTimer(this);

    if (resume) {
        QJsonArray virusesSave = read("viruses").array();
        for (virusesIndex = 0; virusesIndex < virusesSave.size(); virusesIndex++) {
            QJsonObject currentVirus = virusesSave[virusesIndex].toObject();
            viruses[virusesIndex] = new Virus(currentVirus["type"].toInt(),spongeBob,aquarium);
            viruses[virusesIndex]->speed = currentVirus["speed"].toInt();
            viruses[virusesIndex]->setX(currentVirus["x"].toDouble());
            viruses[virusesIndex]->setY(currentVirus["y"].toDouble());
            viruses[virusesIndex]->baseY = currentVirus["y"].toDouble();
            addItem(viruses[virusesIndex]);
        }
    }

    items = new Item*[20];
    for (int i = 0; i < 20; i++) {
        items[i] = NULL;
    }
    itemsIndex = 0;

    if (resume) {
        QJsonArray itemsSave = read("items").array();
        for (itemsIndex = 0; itemsIndex < itemsSave.size(); itemsIndex++) {
            QJsonObject currentItem = itemsSave[itemsIndex].toObject();
            items[itemsIndex] = new Item(aquarium, spongeBob, currentItem["isHealthy"].toBool(), currentItem["type"].toInt());
            items[itemsIndex]->setX(currentItem["x"].toDouble());
            items[itemsIndex]->setY(currentItem["y"].toDouble());
            addItem(items[itemsIndex]);
        }
    }

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

    QObject::connect(unpause, SIGNAL(clicked()), SLOT(unpauseClicked()));
    QObject::connect(quit, SIGNAL(clicked()), SLOT(quitClicked()));
    QObject::connect(quit2, SIGNAL(clicked()), SLOT(quitClicked()));
    QObject::connect(nextLevelButton, SIGNAL(clicked()), SLOT(nextLevel()));

    checkGameStateTimer = new QTimer(this);
    connect(checkGameStateTimer, SIGNAL(timeout()), this, SLOT(checkGameState()));
    checkGameStateTimer->start(100);

    time = new QTime();
    time->start();

    connect(virusTimer, SIGNAL(timeout()), this, SLOT(virusUpdate()));
    connect(updateBacteriasTimer, SIGNAL(timeout()), this, SLOT(updateBacterias()));
    updateItemsTimer = new QTimer(this);
    connect(updateItemsTimer, SIGNAL(timeout()), this, SLOT(updateItems()));
    timeUpdater = new QTimer(this);
    connect(timeUpdater, SIGNAL(timeout()), this, SLOT(updateTimer()));
    connect(pestilenceTimer, SIGNAL(timeout()), this, SLOT(summonPestilence()));
    unpauseTimer = new QTimer(this);
    connect(unpauseTimer, SIGNAL(timeout()), this, SLOT(unpauseGame()));

    updateItemsTimer->setSingleShot(true);
    updateBacteriasTimer->setSingleShot(true);
    virusTimer->setSingleShot(true);
    pestilenceTimer->setSingleShot(true);
    unpauseTimer->setSingleShot(true);

    if (resume) {
        QJsonObject pausedTimesSave = read("pausedTimes").object();

        timeUpdater->setSingleShot(true);

        timeUpdater->start(pausedTimesSave["pausedTimeUpdater"].toInt());
        updateItemsTimer->start(pausedTimesSave["pausedUpdateItemsTimer"].toInt());
        updateBacteriasTimer->start(pausedTimesSave["pausedUpdateBacteriasTimer"].toInt());
        if (pausedTimesSave.contains("virusTimer")) {
            virusTimer->start(pausedTimesSave["virusTimer"].toInt());
        }
        if (pausedTimesSave.contains("pestilenceTimer")) {
           pestilenceTimer->start(pausedTimesSave["pestilenceTimer"].toInt());
           updateTimer();
           pestilenceTimeLabel->show();
           pestilenceTimeLabel2->show();
        }
    } else {
        updateBacteriasTimer->start(aquarium->levels[aquarium->level]["bacteriaGenerationRate"]);
        updateItemsTimer->start(aquarium->levels[aquarium->level]["itemDropRate"]);
        if (aquarium->level != 1) {
            virusTimer->start(aquarium->levels[aquarium->level]["virusGenerationRate"]);
        }
        timeUpdater->start(500);
    }

    this->pausedTimeUpdater = 0;
    this->pausedUpdateItemsTimer = 0;
    this->pausedUpdateBacteriasTimer = 0;
    this->pausedVirusTimer = 0;
    this->pausedPestilenceTimer = 0;

    updateTimer();
}

int GameScene1::getCurrentScore() {
    return aquarium->score;
}

/**
 * @brief GameScene1::nextLevel
 *
 * A member function that loads the next level and mainly sets the timers.
 */
void GameScene1::nextLevel() {
    scoreLabel2->hide();
    quit2->hide();
    greyForeground->hide();
    gameOverLabel->hide();
    nextLevelButton->hide();

    spongeBob->setFocus();

    updateBacterias();

    checkGameStateTimer->start(100);
    updateBacteriasTimer->start(aquarium->levels[aquarium->level]["bacteriaGenerationRate"]);
    updateItemsTimer->start(aquarium->levels[aquarium->level]["itemDropRate"]);
    if (aquarium->level != 1) {
        virusTimer->start(aquarium->levels[aquarium->level]["virusGenerationRate"]);
    }
    timeUpdater->start(500);

    pausedTime = 0;
    time->restart();
    updateTimer();
}

/**
 * @brief GameScene1::setUpNextLevel
 *
 * A member function that sets up the next level.
 */
void GameScene1::setUpNextLevel() {
    aquarium->level++;
    aquarium->currentCleanliness = 0;
    aquarium->currentTime = 0;

    if (spongeBob->lives < 3) {
        addItem(pixmapLife1);
    }
    if (spongeBob->lives < 2) {
        addItem(pixmapLife2);
    }

    spongeBob->reset();

    QPixmap *greenColor = new QPixmap("needle.png");
    greenColor->fill(Qt::green);
    greenColorItem->setPixmap(greenColor->scaled((230 / aquarium->levels[aquarium->level]["maxCleanliness"]) * aquarium->currentCleanliness, 20));

    pixmapNeedle->setTransformOriginPoint(pixmapNeedle->boundingRect().center().x() + 20,
                                               pixmapNeedle->boundingRect().center().y());
    pixmapNeedle->setRotation(0);
}

/**
 * @brief GameScene1::virusUpdate
 *
 * A member function that generates a new virus.
 */
void GameScene1::virusUpdate(){
    int time = (rand() % 2000) + aquarium->levels[aquarium->level]["virusGenerationRate"] - 1000;
    virusTimer->start(time);

    if (virusesIndex >= 9) {
        virusesIndex = 0;
    }

    int weight = (rand() % (aquarium->levels[aquarium->level]["virusWeight1"]
                        + aquarium->levels[aquarium->level]["virusWeight2"]
                        + aquarium->levels[aquarium->level]["virusWeight3"]));

    int type = 1;
    if (weight < aquarium->levels[aquarium->level]["virusWeight1"]) {
        type = 1;
    } else if (weight < aquarium->levels[aquarium->level]["virusWeight1"] + aquarium->levels[aquarium->level]["virusWeight2"]) {
        type = 2;
    } else {
        if (!pestilenceTimer->isActive()) {
            type = 3;
        } else {
            type = 1;
        }
    }

    if (type == 3){
        pestilenceTimer->start(10000);
        updateTimer();
        pestilenceTimeLabel->show();
        pestilenceTimeLabel2->show();
    } else {
        viruses[virusesIndex] = new Virus(type, spongeBob, aquarium);
        addItem(viruses[virusesIndex++]);
    }
}

/**
 * @brief GameScene1::summonPestilence
 *
 * A member function that summons Pestilence.
 */
void GameScene1::summonPestilence() {
    pestilenceTimeLabel->hide();
    pestilenceTimeLabel2->hide();

    if (virusesIndex >= 9) {
        virusesIndex = 0;
    }

    viruses[virusesIndex] = new Virus(3,spongeBob,aquarium);
    addItem(viruses[virusesIndex++]);
}

/**
 * @brief GameScene1::updateTimer
 *
 * A member function that updates time based labels.
 */
void GameScene1::updateTimer() {
    if (timeUpdater->isSingleShot()) {
        timeUpdater->setSingleShot(false);
        timeUpdater->start(500);
    }

    int secs = (time->elapsed() + pausedTime) / 1000;
    int mins = (secs / 60) % 60;
    secs = secs % 60;

    timeLabel->setText(QString("%1:%2")
    .arg(mins, 2, 10, QLatin1Char('0'))
    .arg(secs, 2, 10, QLatin1Char('0')) );

    aquarium->currentTime = time->elapsed() + pausedTime;

    if (pestilenceTimer->isActive()) {
        secs = (pestilenceTimer->remainingTime()) / 1000;
        mins = (secs / 60) % 60;
        secs = secs % 60;

        pestilenceTimeLabel->setText(QString("%1:%2")
        .arg(mins, 2, 10, QLatin1Char('0'))
        .arg(secs, 2, 10, QLatin1Char('0')) );
    }
}

/**
 * @brief GameScene1::updateItems
 *
 * A member function that generates a new item.
 */
void GameScene1::updateItems(){
    int time = (rand() % 500) + aquarium->levels[aquarium->level]["itemDropRate"] - 250;
    updateItemsTimer->start(time);

    int weight = (rand() % (aquarium->levels[aquarium->level]["healthyItemWeight"]
                        + aquarium->levels[aquarium->level]["unhealthyItemWeight"]));

    bool isHealthy = true;
    if (weight < aquarium->levels[aquarium->level]["healthyItemWeight"]) {
        isHealthy = true;
    } else {
        isHealthy = false;
    }

    if (itemsIndex >= 20) {
        itemsIndex = 0;
    }

    items[itemsIndex] = new Item(aquarium, spongeBob, isHealthy);
    addItem(items[itemsIndex++]);
}

/**
 * @brief GameScene1::unpauseClicked
 *
 * A member function that unpauses the game (sending it into the 3..2..1 state).
 */
void GameScene1::unpauseClicked() {
   aquarium->requestForUnpause = true;
}

/**
 * @brief GameScene1::quitClicked
 *
 * A member function that exists the game and goes back to the GamePage.
 */
void GameScene1::quitClicked() {
    if (spongeBob->lives > 0) {
        saveProgress();

        saveFile();
    }

    backToGamePage();
}

/**
 * @brief GameScene1::saveProgressHelper
 *
 * A member function that saves all game state parameters.
 *
 * @param QJsonObject &saveObject that helps save all the current game state parameters.
 */
void GameScene1::saveProgressHelper(QJsonObject &saveObject) const
{
    // Add aquarium fields
    QJsonObject aquarium;

    aquarium["level"] = this->aquarium->level;

    aquarium["currentCleanliness"] = this->aquarium->currentCleanliness;
    aquarium["currentTime"] = this->aquarium->currentTime;

    aquarium["score"] = this->aquarium->score;

    saveObject["aquarium"] = aquarium;

    // Add spongeBob fields
    QJsonObject spongeBob;

    spongeBob["immunityLevel"] = this->spongeBob->immunityLevel;
    spongeBob["immunityLevelDegree"] = this->spongeBob->immunityLevelDegree;
    spongeBob["lives"] = this->spongeBob->lives;
    spongeBob["x"] = this->spongeBob->x();
    spongeBob["y"] = this->spongeBob->y();

    QJsonObject bacteriaCollisions;
    for (int i = 0; i < 3; i++) {
        bacteriaCollisions[QStringLiteral("numbBacteriaCollisions%1").arg(i + 1)] = this->spongeBob->numCollisionsWithBacterias[i];
    }
    spongeBob["numbBacteriaCollisions"] = bacteriaCollisions;

    saveObject["spongeBob"] = spongeBob;

    // Add bacterias fields
    QJsonArray bacterias;

    for (int i = 0; i < 20; i++) {
        if (this->bacterias[i] != NULL) {
            QJsonObject currentBacteria;

            currentBacteria["x"] = this->bacterias[i]->x();
            currentBacteria["y"] = this->bacterias[i]->y();
            currentBacteria["type"] = this->bacterias[i]->type;
            currentBacteria["speed"] = this->bacterias[i]->speed;

            bacterias.append(currentBacteria);
        }
    }

    if (!bacterias.empty()) {
        saveObject["bacterias"] = bacterias;
    }

    // Add viruses fields
    QJsonArray viruses;

    for (int i = 0; i < 10; i++) {
        if (this->viruses[i] != NULL) {
            QJsonObject currentVirus;

            currentVirus["x"] = this->viruses[i]->x();
            currentVirus["y"] = this->viruses[i]->y();
            currentVirus["speed"] = this->viruses[i]->speed;
            currentVirus["type"]= this->viruses[i]->type;

            viruses.append(currentVirus);
        }
    }

    if (!viruses.empty()) {
        saveObject["viruses"] = viruses;
    }

    // Add items fields
    QJsonArray items;

    for (int i = 0; i < 20; i++) {
        if (this->items[i] != NULL) {
            QJsonObject currentItem;

            currentItem["x"] = this->items[i]->x();
            currentItem["y"] = this->items[i]->y();
            currentItem["type"] = this->items[i]->type;
            currentItem["isHealthy"] = this->items[i]->isHealthy;

            items.append(currentItem);
        }
    }

    if (!items.empty()) {
        saveObject["items"] = items;
    }

    // Add paused times
    QJsonObject pausedTimes;

    pausedTimes["pausedTimeUpdater"] = this->pausedTimeUpdater;
    pausedTimes["pausedUpdateItemsTimer"] = this->pausedUpdateItemsTimer;
    pausedTimes["pausedUpdateBacteriasTimer"] = this->pausedUpdateBacteriasTimer;
    if (this->pausedVirusTimer != 0) {
        pausedTimes["pausedVirusTimer"] = this->pausedVirusTimer;
    }
    if (this->pausedPestilenceTimer != 0) {
        pausedTimes["pausedPestilenceTimer"] = this->pausedPestilenceTimer;
    }

    saveObject["pausedTimes"] = pausedTimes;

    // Add needle orientation
    QJsonObject needle;

    needle["rotation"] = this->pixmapNeedle->rotation();

    saveObject["needle"] = needle;
}

/**
 * @brief GameScene1::saveScoreHelper
 *
 * A member function that saves the current score alongside the date.
 *
 * @param QJsonObject &saveObject that helps save data related to the score.
 */
void GameScene1::saveScoreHelper(QJsonObject &saveObject) const
{
    saveObject["score"] = aquarium->score;
    saveObject["day"] = QDate::currentDate().day();
    saveObject["month"] = QDate::currentDate().month();
    saveObject["year"] = QDate::currentDate().year();
}

/**
 * @brief GameScene1::updateBacterias
 *
 * A member function that generates a new bacteria.
 */
void GameScene1::updateBacterias() {
    int time = (rand() % 1000) + aquarium->levels[aquarium->level]["bacteriaGenerationRate"] - 500;
    updateBacteriasTimer->start(time);

    if (bacteriasIndex >= 19) {
        bacteriasIndex = 0;
    }

    int weight = (rand() % (aquarium->levels[aquarium->level]["bacteriaWeight1"]
                        + aquarium->levels[aquarium->level]["bacteriaWeight2"]
                        + aquarium->levels[aquarium->level]["bacteriaWeight3"]));

    int type = 1;
    if (weight < aquarium->levels[aquarium->level]["bacteriaWeight1"]) {
        type = 1;
    } else if (weight < aquarium->levels[aquarium->level]["bacteriaWeight1"] + aquarium->levels[aquarium->level]["bacteriaWeight2"]) {
        type = 2;
    } else {
        type = 3;
    }

    bacterias[bacteriasIndex] = new Bacteria(type,spongeBob,aquarium,greenColorItem,pixmapLifeList);
    addItem(bacterias[bacteriasIndex++]);
}

/**
 * @brief GameScene1::unpauseGame
 *
 * A member function that unpaused the game after the 3..2..1.. state.
 */
void GameScene1::unpauseGame() {
    time->restart();

    timeUpdater->setSingleShot(true);

    timeUpdater->start(pausedTimeUpdater);
    updateItemsTimer->start(pausedUpdateItemsTimer);
    updateBacteriasTimer->start(pausedUpdateBacteriasTimer);
    if (pausedVirusTimer > 0) {
        virusTimer->start(pausedVirusTimer);
    }
    if (pausedPestilenceTimer > 0) {
        pestilenceTimer->start(pausedPestilenceTimer);
        pausedPestilenceTimer = 0;
    }

    greyForeground->hide();
    unpauseLabel->hide();

    aquarium->gamePaused = false;
    aquarium->requestForUnpause = false;
}

/**
 * @brief GameScene1::checkGameState
 *
 * A member function that checks and updates the game's state.
 */
void GameScene1::checkGameState() {
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
                pausedUpdateItemsTimer = updateItemsTimer->remainingTime();
                pausedUpdateBacteriasTimer = updateBacteriasTimer->remainingTime();

                if (pausedTimeUpdater < 0) {
                    pausedTimeUpdater = 0;
                }
                if (pausedUpdateItemsTimer < 0) {
                    pausedUpdateItemsTimer = 0;
                }
                if (pausedUpdateBacteriasTimer < 0) {
                    pausedUpdateBacteriasTimer = 0;
                }

                if (virusTimer->isActive()) {
                    pausedVirusTimer = virusTimer->remainingTime();

                    if (pausedVirusTimer < 0) {
                        pausedVirusTimer = 0;
                    }

                    virusTimer->stop();
                }

                if (pestilenceTimer->isActive()) {
                    pausedPestilenceTimer = pestilenceTimer->remainingTime();

                    if (pausedPestilenceTimer < 0) {
                        pausedPestilenceTimer = 0;
                    }

                    pestilenceTimer->stop();
                }

                timeUpdater->stop();
                updateItemsTimer->stop();
                updateBacteriasTimer->stop();

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

    spongeBob->setFocus();

    // Remove bacterias
    for (int i = 0; i < 20; i++) {
        if (bacterias[i] != NULL && bacterias[i]->toDelete) {
            delete bacterias[i];
            bacterias[i] = NULL;
        }
    }

    // Remove items
    for (int i = 0; i < 20; i++) {
        if (items[i] != NULL && items[i]->toDelete) {
            delete items[i];
            items[i] = NULL;
        }
    }

    // Remove viruses
    for (int i = 0; i < 10; i++) {
        if (viruses[i] != NULL && viruses[i]->toDelete) {
            delete viruses[i];
            viruses[i] = NULL;
        }
    }

    // Update score
    scoreLabel->setText(QStringLiteral("Score: %1").arg(aquarium->score));
    scoreLabel->adjustSize();

    // Update level
    levelLabel->setText(QStringLiteral("Level: %1").arg(aquarium->level));
    levelLabel->adjustSize();

    // Check if spongebob is dead
    if (spongeBob->lives == 0) {
        gameOver(false);
    }

    // Check if time is up
    if (time->elapsed() >= aquarium->levels[aquarium->level]["maxTime"]) {
        int secs = aquarium->levels[aquarium->level]["maxTime"] / 1000;
        int mins = (secs / 60) % 60;
        secs = secs % 60;
        timeLabel->setText(QString("%1:%2")
        .arg(mins, 2, 10, QLatin1Char('0'))
        .arg(secs, 2, 10, QLatin1Char('0')) );

        aquarium->currentTime = aquarium->levels[aquarium->level]["maxTime"];

        gameOver(false);
    }

    // Check if the aquarium is cleaned
    if (aquarium->currentCleanliness == aquarium->levels[aquarium->level]["maxCleanliness"]) {
        gameOver(true);
    }
}

/**
 * @brief GameScene1::gameOver
 *
 * A member function that shows the game over screen.
 *
 * @param bool result whether the level was won or lost.
 */
void GameScene1::gameOver(bool result) {
    timeUpdater->stop();
    updateItemsTimer->stop();
    updateBacteriasTimer->stop();
    checkGameStateTimer->stop();
    if (virusTimer->isActive()) {
        virusTimer->stop();
    }
    if (pestilenceTimer->isActive()) {
        pestilenceTimer->stop();
    }

    for (int i = 0; i < 20; i++) {
        if (bacterias[i] != NULL) {
            delete bacterias[i];
            bacterias[i] = NULL;
        }
    }
    bacteriasIndex = 0;

    for (int i = 0; i < 10; i++) {
        if (viruses[i] != NULL) {
            delete viruses[i];
            viruses[i] = NULL;
        }
    }
    virusesIndex = 0;

    for (int i = 0; i < 20; i++) {
        if (items[i] != NULL) {
            delete items[i];
            items[i] = NULL;
        }
    }
    itemsIndex = 0;

    if (result) {
        aquarium->score += aquarium->levels[aquarium->level]["maxTime"] / aquarium->currentTime - 1;
        aquarium->score += spongeBob->lives * 100;
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
