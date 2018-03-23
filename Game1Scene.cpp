#include "Game1Scene.h"

#include "Message.h"

Game1Scene::Game1Scene(QWidget *widget, User* user, QJsonObject usersFile, bool resume, int level, QGraphicsScene *parent) : QGraphicsScene(parent)
{
    srand(QTime::currentTime().msec());

    this->widget = widget;
    this->user = user;
    this->usersFile = usersFile;

    if (resume) {
        QJsonObject aquariumSave = read("aquarium").object();
        aquarium = new Aquarium(aquariumSave["level"].toInt(),
                aquariumSave["maxCleanliness"].toInt(), aquariumSave["incrementCleanliness"].toInt(), aquariumSave["currentCleanliness"].toInt(),
                aquariumSave["immunityFactor"].toInt(),
                aquariumSave["maxTime"].toInt(), aquariumSave["currentTime"].toInt(),
                aquariumSave["score"].toInt());
    } else {
        aquarium = new Aquarium(level, 10, 1, 0, 1, 300000, 0, 0);
    }


    setBackgroundBrush(QBrush(QImage("background2.JPG").scaledToHeight(600).scaledToWidth(1000)));
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
    QPixmap *picNeedle  = new QPixmap("needle.png");
    pixmapNeedle->setPixmap(picNeedle->scaled(80,20));
    pixmapNeedle->setPos(850,80);
    addItem(pixmapNeedle);

    pixmapLife1 = new QGraphicsPixmapItem();
    pixmapLife2 = new QGraphicsPixmapItem();
    pixmapLife3 = new QGraphicsPixmapItem();
    QPixmap *picLife  = new QPixmap("life.png");

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
    greenColorItem->setPixmap(greenColor->scaled((230 / aquarium->maxCleanliness) * aquarium->currentCleanliness, 20));
    addItem(greenColorItem);

    spongeBob = new SpongeBob(aquarium, pixmapNeedle, pixmapLifeList);

    if (resume) {
        QJsonObject spongeBobSave = read("spongeBob").object();
        spongeBob->immunityLevel = spongeBobSave["immunityLevel"].toInt();
        spongeBob->immunityLevelDegree = spongeBobSave["immunityLevelDegree"].toInt();
        spongeBob->lives = spongeBobSave["lives"].toInt();
        spongeBob->setX(spongeBobSave["x"].toInt());
        spongeBob->setY(spongeBobSave["y"].toInt());
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
        pixmapNeedle->setTransformOriginPoint(needleSave["x"].toInt(), needleSave["y"].toInt());
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
            bacterias[bacteriasIndex]->setX(currentBacteria["x"].toInt());
            bacterias[bacteriasIndex]->setY(currentBacteria["y"].toInt());
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
            viruses[virusesIndex]->setX(currentVirus["x"].toInt());
            viruses[virusesIndex]->setY(currentVirus["y"].toInt());
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
            items[itemsIndex]->setX(currentItem["x"].toInt());
            items[itemsIndex]->setY(currentItem["y"].toInt());
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
    QGraphicsProxyWidget *proxyWidget = addWidget(greyForeground);
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

    quit2 = new QPushButton("Quit");
    quit2->move(this->width() / 2 - 60, this->height() / 2 + 150);
    proxyWidget = addWidget(quit2);
    proxyWidget->setZValue(10000);
    quit2->hide();

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

    updateItemsTimer->setSingleShot(true);
    updateBacteriasTimer->setSingleShot(true);
    virusTimer->setSingleShot(true);
    pestilenceTimer->setSingleShot(true);

    if (resume) {
        QJsonObject pausedTimesSave = read("pausedTimes").object();

        timeUpdater->setSingleShot(true);

        timeUpdater->start(pausedTimesSave["pausedTimeUpdater"].toInt());
        updateItemsTimer->start(pausedTimesSave["pausedUpdateItemsTimer"].toInt());
        updateBacteriasTimer->start(pausedTimesSave["pausedUpdateBacteriasTimer"].toInt());
        virusTimer->start(pausedTimesSave["virusTimer"].toInt());
        if (pausedTimesSave.contains("pestilenceTimer")) {
           pestilenceTimer->start(pausedTimesSave["pestilenceTimer"].toInt());
           updateTimer();
           pestilenceTimeLabel->show();
           pestilenceTimeLabel2->show();
        }
    } else {
        updateBacteriasTimer->start(2000);
        updateItemsTimer->start(3000);
        virusTimer->start(10000);
        timeUpdater->start(500);
    }

    updateTimer();
}

void Game1Scene::nextLevel() {
    scoreLabel2->hide();
    quit2->hide();
    greyForeground->hide();
    gameOverLabel->hide();
    nextLevelButton->hide();

    spongeBob->setFocus();

    updateBacterias();

    checkGameStateTimer->start(100);
    updateBacteriasTimer->start(2000);
    updateItemsTimer->start(3000);
    timeUpdater->start(500);
    virusTimer->start(10000);

    pausedTime = 0;
    time->restart();
    updateTimer();
}

void Game1Scene::setUpNextLevel() {
    aquarium->level++;
    aquarium->currentCleanliness = 0;
    aquarium->currentTime = 0;
    aquarium->immunityFactor = 0;

    spongeBob->reset();
    if (spongeBob->lives < 3) {
        addItem(pixmapLife1);
    }
    if (spongeBob->lives < 2) {
        addItem(pixmapLife2);
    }

    QPixmap *greenColor = new QPixmap("needle.png");
    greenColor->fill(Qt::green);
    greenColorItem->setPixmap(greenColor->scaled((230 / aquarium->maxCleanliness) * aquarium->currentCleanliness, 20));

    pixmapNeedle->setTransformOriginPoint(0, 0);
    pixmapNeedle->setRotation(0);
}

void Game1Scene::virusUpdate(){
    int time = (rand() % 2000) + 8000;
    virusTimer->start(time);

    if (virusesIndex >= 9) {
        virusesIndex = 0;
    }
    if (aquarium->level == 3) {
        int random_number=(rand() % 20) + 1;
        if(random_number == 10 && !pestilenceTimer->isActive()){
            pestilenceTimer->start(10000);
            updateTimer();
            pestilenceTimeLabel->show();
            pestilenceTimeLabel2->show();
        } else if(random_number > 10){
            viruses[virusesIndex] = new Virus(1,spongeBob,aquarium);
            addItem(viruses[virusesIndex++]);
        } else {
            viruses[virusesIndex] = new Virus(2,spongeBob,aquarium);
            addItem(viruses[virusesIndex++]);
        }
    } else if (aquarium->level == 2) {
        viruses[virusesIndex] = new Virus(1,spongeBob,aquarium);
        addItem(viruses[virusesIndex++]);
    }
}

void Game1Scene::summonPestilence() {
    pestilenceTimeLabel->hide();
    pestilenceTimeLabel2->hide();

    if (virusesIndex >= 9) {
        virusesIndex = 0;
    }

    viruses[virusesIndex] = new Virus(3,spongeBob,aquarium);
    addItem(viruses[virusesIndex++]);
}

void Game1Scene::updateTimer() {
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

void Game1Scene::updateItems(){
    int time = (rand() % 500) + 3000;
    updateItemsTimer->start(time);

    int random_number = (rand() % 2) + 1;

    if (itemsIndex >= 20) {
        itemsIndex = 0;
    }

    if(random_number==1) {
        items[itemsIndex] = new Item(aquarium, spongeBob, true);
        addItem(items[itemsIndex++]);
    } else if(random_number==2) {
        items[itemsIndex] = new Item(aquarium, spongeBob, false);
        addItem(items[itemsIndex++]);
    }
}

void Game1Scene::unpauseClicked() {
    aquarium->gamePaused = false;

    greyForeground->hide();
    unpause->hide();
    quit->hide();

    spongeBob->setFocus();
}

void Game1Scene::quitClicked() {
    if (spongeBob->lives > 0) {
        saveProgress();

        saveFile();
    }

    views()[0]->close();
    GameOnePage *gameOnePage = new GameOnePage(widget, 1, user, usersFile);
    widget->show();
}

void Game1Scene::saveFile() {
    QFile saveFile(QStringLiteral("Data.json"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        Message *msg = new Message("Couldn't open data file to save.");
        msg->show();
    }

    QJsonDocument saveDoc(usersFile);
    saveFile.write(saveDoc.toJson());
}

QJsonDocument Game1Scene::read(QString type) {
    QJsonObject save;
    if (usersFile.contains("games") && usersFile["games"].isArray()) {
        QJsonArray games = usersFile["games"].toArray();
        if (games.size() > 0 && games[0].isObject()) {
            QJsonObject gameData = games[0].toObject();
            if (gameData.contains("users_save") && gameData["users_save"].isArray()) {
                QJsonArray userArray = gameData["users_save"].toArray();

                // If save for user already created, overwrite it
                for (int userIndex = 0; userIndex < userArray.size(); userIndex++) {
                    QJsonObject userObject = userArray[userIndex].toObject();
                    if (userObject.contains("username") && userObject["username"].isString() && userObject["username"] == this->user->username) {
                        save = userObject["save"].toObject();
                        if (save[type].isObject()) {
                            return (QJsonDocument) save[type].toObject();
                        } else if (save[type].isArray()) {
                            return (QJsonDocument) save[type].toArray();
                        }
                    }
                }
            }
        }
    }
    return (QJsonDocument) save;
}

bool Game1Scene::saveProgress() {
    if (usersFile.contains("games") && usersFile["games"].isArray()) {
        QJsonArray games = usersFile["games"].toArray();
        if (games.size() > 0 && games[0].isObject()) {
            QJsonObject gameData = games[0].toObject();
            if (gameData.contains("users_save") && gameData["users_save"].isArray()) {
                QJsonArray userArray = gameData["users_save"].toArray();

                // If save for user already created, overwrite it
                for (int userIndex = 0; userIndex < userArray.size(); userIndex++) {
                    QJsonObject userObject = userArray[userIndex].toObject();
                    if (userObject.contains("username") && userObject["username"].isString() && userObject["username"] == this->user->username) {
                        QJsonObject saveObject;
                        saveProgressHelper(saveObject);
                        userObject["save"] = saveObject;
                        userArray[userIndex] = userObject;
                        gameData["users_save"] = userArray;
                        games[0] = gameData;
                        usersFile["games"] = games;
                        return true;
                    }
                }

                // If save for user never created before, create it
                QJsonObject userObject;
                QJsonObject saveObject;
                saveProgressHelper(saveObject);
                userObject["save"] = saveObject;
                userObject["username"] = this->user->username;
                userArray.append(userObject);
                gameData["users_save"] = userArray;
                games[0] = gameData;
                usersFile["games"] = games;
                return true;
            } else {
                // If no save of any users have ever been created before, create new list and create the save of the user
                QJsonArray userArray;
                QJsonObject userObject;
                QJsonObject saveObject;
                saveProgressHelper(saveObject);
                userObject["save"] = saveObject;
                userObject["username"] = this->user->username;
                userArray.append(userObject);
                gameData["users_save"] = userArray;
                games[0] = gameData;
                usersFile["games"] = games;
                return true;
            }
        }
    }
    return false;
}

void Game1Scene::saveProgressHelper(QJsonObject &saveObject) const
{
    // Add aquarium fields
    QJsonObject aquarium;

    aquarium["level"] = this->aquarium->level;
    aquarium["score"] = this->aquarium->score;

    aquarium["maxCleanliness"] = this->aquarium->maxCleanliness;
    aquarium["incrementCleanliness"] = this->aquarium->incrementCleanliness;
    aquarium["currentCleanliness"] = this->aquarium->currentCleanliness;

    aquarium["immunityFactor"] = this->aquarium->immunityFactor;

    aquarium["maxTime"] = this->aquarium->maxTime;
    aquarium["currentTime"] = this->aquarium->currentTime;

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
    pausedTimes["pausedVirusTimer"] = this->pausedVirusTimer;
    if (this->pausedPestilenceTimer != 0) {
        pausedTimes["pausedPestilenceTimer"] = this->pausedPestilenceTimer;
    }

    saveObject["pausedTimes"] = pausedTimes;

    // Add needle orientation
    QJsonObject needle;

    needle["x"] = this->pixmapNeedle->boundingRect().center().x();
    needle["y"] = this->pixmapNeedle->boundingRect().center().y();
    needle["rotation"] = this->pixmapNeedle->rotation();

    saveObject["needle"] = needle;
}

bool Game1Scene::saveScore() {
    if (usersFile.contains("games") && usersFile["games"].isArray()) {
        QJsonArray games = usersFile["games"].toArray();
        if (games.size() > 0 && games[0].isObject()) {
            QJsonObject gameData = games[0].toObject();

            if (gameData.contains("top_score") && gameData["top_score"].isObject()) {
                QJsonObject userObject = gameData["top_score"].toObject();
                if (userObject.contains("score") && userObject["score"].toInt() < aquarium->score) {
                    userObject["score"] = aquarium->score;
                    userObject["username"] = user->username;
                    gameData["top_score"] = userObject;
                    games[0] = gameData;
                    usersFile["games"] = games;
                }
            } else {
                QJsonObject userObject;
                userObject["score"] = aquarium->score;
                userObject["username"] = user->username;
                gameData["top_score"] = userObject;
                games[0] = gameData;
                usersFile["games"] = games;
            }

            if (gameData.contains("users_score") && gameData["users_score"].isArray()) {
                QJsonArray userArray = gameData["users_score"].toArray();

                // If score for user already created, add another
                for (int userIndex = 0; userIndex < userArray.size(); userIndex++) {
                    QJsonObject userObject = userArray[userIndex].toObject();
                    if (userObject.contains("username") && userObject["username"].isString() && userObject["username"] == this->user->username) {
                        QJsonObject saveObject;
                        saveScoreHelper(saveObject);
                        QJsonArray scoresArray = userObject["scores"].toArray();
                        scoresArray.append(saveObject);
                        userObject["scores"] = scoresArray;
                        userArray[userIndex] = userObject;
                        gameData["users_score"] = userArray;
                        games[0] = gameData;
                        usersFile["games"] = games;
                        return true;
                    }
                }

                // If score for user never created before, create it
                QJsonObject userObject;
                QJsonObject saveObject;
                saveScoreHelper(saveObject);
                QJsonArray scoresArray = userObject["scores"].toArray();
                scoresArray.append(saveObject);
                userObject["scores"] = scoresArray;
                userObject["username"] = this->user->username;
                userArray.append(userObject);
                gameData["users_score"] = userArray;
                games[0] = gameData;
                usersFile["games"] = games;
                return true;
            } else {
                // If no score of any users have ever been created before, create new list and create the score of the user
                QJsonArray userArray;
                QJsonObject userObject;
                QJsonObject saveObject;
                saveScoreHelper(saveObject);
                QJsonArray scoresArray;
                scoresArray.append(saveObject);
                userObject["scores"] = scoresArray;
                userObject["username"] = this->user->username;
                userArray.append(userObject);
                gameData["users_score"] = userArray;
                games[0] = gameData;
                usersFile["games"] = games;
                return true;
            }
        }
    }
    return false;
}

void Game1Scene::saveScoreHelper(QJsonObject &saveObject) const
{
    saveObject["score"] = aquarium->score;
    saveObject["day"] = QDate::currentDate().day();
    saveObject["month"] = QDate::currentDate().month();
    saveObject["year"] = QDate::currentDate().year();
}

void Game1Scene::updateBacterias() {
    int time = (rand() % 1000) + 2000;
    updateBacteriasTimer->start(time);

    if (bacteriasIndex >= 19) {
        bacteriasIndex = 0;
    }

    int type = (rand() % 3) + 1;

    bacterias[bacteriasIndex] = new Bacteria(type,spongeBob,aquarium,greenColorItem,pixmapLifeList);
    addItem(bacterias[bacteriasIndex++]);
}

void Game1Scene::checkGameState() {
    // Check if game paused
    if (aquarium->gamePaused) {
       // Pause everything. We need those stats anw for the save functionality

        if (justPaused) {
            pausedTime += time->elapsed();

            pausedTimeUpdater = timeUpdater->remainingTime();
            pausedUpdateItemsTimer = updateItemsTimer->remainingTime();
            pausedUpdateBacteriasTimer = updateBacteriasTimer->remainingTime();
            pausedVirusTimer = virusTimer->remainingTime();

            if (pausedTimeUpdater < 0) {
                pausedTimeUpdater = 0;
            }
            if (pausedUpdateItemsTimer < 0) {
                pausedUpdateItemsTimer = 0;
            }
            if (pausedUpdateBacteriasTimer < 0) {
                pausedUpdateBacteriasTimer = 0;
            }
            if (pausedVirusTimer < 0) {
                pausedVirusTimer = 0;
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
            virusTimer->stop();

            justPaused = false;

            greyForeground->show();
            unpause->show();
            quit->show();
        }

        return;
    } else {
        if (!justPaused) {
            time->restart();

            timeUpdater->setSingleShot(true);

            timeUpdater->start(pausedTimeUpdater);
            updateItemsTimer->start(pausedUpdateItemsTimer);
            updateBacteriasTimer->start(pausedUpdateBacteriasTimer);
            virusTimer->start(pausedVirusTimer);
            if (pausedPestilenceTimer > 0) {
                pestilenceTimer->start(pausedPestilenceTimer);
                pausedPestilenceTimer = 0;
            }

            justPaused = true;

            greyForeground->hide();
            unpause->hide();
            quit->hide();
        }
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
    if (time->elapsed() >= aquarium->maxTime) {
        int secs = aquarium->maxTime / 1000;
        int mins = (secs / 60) % 60;
        secs = secs % 60;
        timeLabel->setText(QString("%1:%2")
        .arg(mins, 2, 10, QLatin1Char('0'))
        .arg(secs, 2, 10, QLatin1Char('0')) );

        aquarium->currentTime = aquarium->maxTime;

        gameOver(false);
    }

    // Check if the aquarium is cleaned
    if (aquarium->currentCleanliness == aquarium->maxCleanliness) {
        gameOver(true);
    }
}

void Game1Scene::gameOver(bool result) {
    timeUpdater->stop();
    updateItemsTimer->stop();
    updateBacteriasTimer->stop();
    checkGameStateTimer->stop();
    virusTimer->stop();
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
        aquarium->score += aquarium->maxTime / aquarium->currentTime - 1;
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
        // Should i also save when he overrides a resume game?

        quit2->move(this->width() / 2 - 60, this->height() / 2 + 150);

        saveScore();
        saveFile();
    }
}
