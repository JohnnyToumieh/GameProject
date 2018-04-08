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
GameScene3::GameScene3(QWidget *widget, User* user, QJsonObject dataFile, bool resume, int level) : GameScene(widget, user, dataFile, 2)
{
    srand(QTime::currentTime().msec());

    if (resume) {
        QJsonObject officeSave = read("office").object();
        office = new Office(officeSave["level"].toInt(),
                officeSave["currentReputation"].toInt(),
                officeSave["currentTime"].toInt(),
                officeSave["score"].toInt());
    } else {
        office = new Office(level, 0, 0, 0);
    }

    setBackgroundBrush(QBrush(QImage(":game2Background").scaledToHeight(600).scaledToWidth(1000)));
    setSceneRect(0,0,1000,600);

    aquarium = new QGraphicsPixmapItem();
    QPixmap *picAquarium  = new QPixmap(":aquarium0");
    aquarium->setPixmap(picAquarium->scaled(153,87));
    aquarium->setPos(52, 313);
    addItem(aquarium);

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

    spongeBob = new SpongeBob(office, pixmapNeedle, pixmapLifeList);

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
            bacterias[bacteriasIndex] = new Bacteria(currentBacteria["type"].toInt(),spongeBob,office,greenColorItem,pixmapLifeList);
            bacterias[bacteriasIndex]->speed = currentBacteria["speed"].toInt();
            bacterias[bacteriasIndex]->setX(currentBacteria["x"].toDouble());
            bacterias[bacteriasIndex]->setY(currentBacteria["y"].toDouble());
            bacterias[bacteriasIndex]->baseY = currentBacteria["y"].toDouble();
            addItem(bacterias[bacteriasIndex]);
        }
    } else {
        //updateBacterias();
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
            viruses[virusesIndex] = new Virus(currentVirus["type"].toInt(),spongeBob,office);
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
            items[itemsIndex] = new Item(office, spongeBob, currentItem["isHealthy"].toBool(), currentItem["type"].toInt());
            items[itemsIndex]->setX(currentItem["x"].toDouble());
            items[itemsIndex]->setY(currentItem["y"].toDouble());
            addItem(items[itemsIndex]);
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
        //updateBacteriasTimer->start(office->levels[office->level]["bacteriaGenerationRate"]);
        //updateItemsTimer->start(office->levels[office->level]["itemDropRate"]);
        if (office->level != 1) {
            virusTimer->start(office->levels[office->level]["virusGenerationRate"]);
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

    spongeBob->setFocus();

    updateBacterias();

    checkGameStateTimer->start(100);
    updateBacteriasTimer->start(office->levels[office->level]["bacteriaGenerationRate"]);
    updateItemsTimer->start(office->levels[office->level]["itemDropRate"]);
    if (office->level != 1) {
        virusTimer->start(office->levels[office->level]["virusGenerationRate"]);
    }
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

    if (spongeBob->lives < 3) {
        addItem(pixmapLife1);
    }
    if (spongeBob->lives < 2) {
        addItem(pixmapLife2);
    }

    spongeBob->reset();

    QPixmap *greenColor = new QPixmap(":needle");
    greenColor->fill(Qt::green);
    greenColorItem->setPixmap(greenColor->scaled((230 / office->levels[office->level]["maxReputation"]) * office->currentReputation, 20));

    pixmapNeedle->setTransformOriginPoint(pixmapNeedle->boundingRect().center().x() + 20,
                                               pixmapNeedle->boundingRect().center().y());
    pixmapNeedle->setRotation(0);
}

/**
 * @brief GameScene3::virusUpdate
 *
 * A member function that generates a new virus.
 */
void GameScene3::virusUpdate(){
    int time = (rand() % 2000) + office->levels[office->level]["virusGenerationRate"] - 1000;
    virusTimer->start(time);

    if (virusesIndex >= 9) {
        virusesIndex = 0;
    }

    int weight = (rand() % (office->levels[office->level]["virusWeight1"]
                        + office->levels[office->level]["virusWeight2"]
                        + office->levels[office->level]["virusWeight3"]));

    int type = 1;
    if (weight < office->levels[office->level]["virusWeight1"]) {
        type = 1;
    } else if (weight < office->levels[office->level]["virusWeight1"] + office->levels[office->level]["virusWeight2"]) {
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
        viruses[virusesIndex] = new Virus(type, spongeBob, office);
        addItem(viruses[virusesIndex++]);
    }
}

/**
 * @brief GameScene3::summonPestilence
 *
 * A member function that summons Pestilence.
 */
void GameScene3::summonPestilence() {
    pestilenceTimeLabel->hide();
    pestilenceTimeLabel2->hide();

    if (virusesIndex >= 9) {
        virusesIndex = 0;
    }

    viruses[virusesIndex] = new Virus(3,spongeBob,office);
    addItem(viruses[virusesIndex++]);
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

    int secs = (time->elapsed() + pausedTime) / 1000;
    int mins = (secs / 60) % 60;
    secs = secs % 60;

    timeLabel->setText(QString("%1:%2")
    .arg(mins, 2, 10, QLatin1Char('0'))
    .arg(secs, 2, 10, QLatin1Char('0')) );

    office->currentTime = time->elapsed() + pausedTime;

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
 * @brief GameScene3::updateItems
 *
 * A member function that generates a new item.
 */
void GameScene3::updateItems(){
    int time = (rand() % 500) + office->levels[office->level]["itemDropRate"] - 250;
    updateItemsTimer->start(time);

    int weight = (rand() % (office->levels[office->level]["healthyItemWeight"]
                        + office->levels[office->level]["unhealthyItemWeight"]));

    bool isHealthy = true;
    if (weight < office->levels[office->level]["healthyItemWeight"]) {
        isHealthy = true;
    } else {
        isHealthy = false;
    }

    if (itemsIndex >= 20) {
        itemsIndex = 0;
    }

    items[itemsIndex] = new Item(office, spongeBob, isHealthy);
    addItem(items[itemsIndex++]);
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
    if (spongeBob->lives > 0) {
        saveProgress();

        saveFile();
    }

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

    office["score"] = this->office->score;

    saveObject["office"] = office;

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
 * @brief GameScene3::updateBacterias
 *
 * A member function that generates a new bacteria.
 */
void GameScene3::updateBacterias() {
    int time = (rand() % 1000) + office->levels[office->level]["bacteriaGenerationRate"] - 500;
    updateBacteriasTimer->start(time);

    if (bacteriasIndex >= 19) {
        bacteriasIndex = 0;
    }

    int weight = (rand() % (office->levels[office->level]["bacteriaWeight1"]
                        + office->levels[office->level]["bacteriaWeight2"]
                        + office->levels[office->level]["bacteriaWeight3"]));

    int type = 1;
    if (weight < office->levels[office->level]["bacteriaWeight1"]) {
        type = 1;
    } else if (weight < office->levels[office->level]["bacteriaWeight1"] + office->levels[office->level]["bacteriaWeight2"]) {
        type = 2;
    } else {
        type = 3;
    }

    bacterias[bacteriasIndex] = new Bacteria(type,spongeBob,office,greenColorItem,pixmapLifeList);
    addItem(bacterias[bacteriasIndex++]);
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
    scoreLabel->setText(QStringLiteral("Score: %1").arg(office->score));
    scoreLabel->adjustSize();

    // Update level
    levelLabel->setText(QStringLiteral("Level: %1").arg(office->level));
    levelLabel->adjustSize();

    // Check if spongebob is dead
    if (spongeBob->lives == 0) {
        gameOver(false);
    }

    // Check if time is up
    if (time->elapsed() >= office->levels[office->level]["maxTime"]) {
        int secs = office->levels[office->level]["maxTime"] / 1000;
        int mins = (secs / 60) % 60;
        secs = secs % 60;
        timeLabel->setText(QString("%1:%2")
        .arg(mins, 2, 10, QLatin1Char('0'))
        .arg(secs, 2, 10, QLatin1Char('0')) );

        office->currentTime = office->levels[office->level]["maxTime"];

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
        office->score += office->levels[office->level]["maxTime"] / office->currentTime - 1;
        office->score += spongeBob->lives * 100;
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
