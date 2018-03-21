#include "Game1Scene.h"

#include "Message.h"

Game1Scene::Game1Scene(QWidget *widget, User* user, QJsonObject usersFile, QGraphicsScene *parent) : QGraphicsScene(parent)
{
    this->widget = widget;
    this->user = user;
    this->usersFile = usersFile;

    aquarium = new Aquarium(1, 10, 1, 0, 1, 300000, 0, 0);

    setBackgroundBrush(QBrush(QImage("background2.JPG").scaledToHeight(600).scaledToWidth(1000)));
    setSceneRect(0,0,1000,600);

    timeLabel = new QLabel();
    timeLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 40px; }");
    timeLabel->move(this->width() / 2 - 30, 25);
    addWidget(timeLabel);

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

    greenColorItem= new QGraphicsPixmapItem();
    greenColorItem->setPos(15,51);
    addItem(greenColorItem);

    pixmapLifeList = new QGraphicsPixmapItem*[3];

    pixmapLifeList[0]=pixmapLife1;
    pixmapLifeList[1]=pixmapLife2;
    pixmapLifeList[2]=pixmapLife3;

    spongeBob = new SpongeBob(aquarium, pixmapNeedle, pixmapLifeList);
    addItem(spongeBob);

    spongeBob->setFlag(QGraphicsItem::ItemIsFocusable);
    spongeBob->setFocus();

    bacterias = new Bacteria*[20];
    for (int i = 0; i < 20; i++) {
        bacterias[i] = NULL;
    }
    bacteriasIndex = 0;

    bacterias[bacteriasIndex] = new Bacteria(1,spongeBob,aquarium,greenColorItem,pixmapLifeList);
    addItem(bacterias[bacteriasIndex++]);
    bacterias[bacteriasIndex] = new Bacteria(2,spongeBob,aquarium,greenColorItem,pixmapLifeList);
    addItem(bacterias[bacteriasIndex++]);
    bacterias[bacteriasIndex] = new Bacteria(3,spongeBob,aquarium,greenColorItem,pixmapLifeList);
    addItem(bacterias[bacteriasIndex++]);

    healthyItems = new HealthyItem*[10];
    for (int i = 0; i < 10; i++) {
        healthyItems[i] = NULL;
    }
    healthyItemsIndex = 0;
    unhealthyItems = new UnhealthyItem*[10];
    for (int i = 0; i < 10; i++) {
        unhealthyItems[i] = NULL;
    }
    unhealthyItemsIndex = 0;

    updateItemsTimer = new QTimer(this);
    connect(updateItemsTimer, SIGNAL(timeout()), this, SLOT(updateItems()));
    updateItemsTimer->start(3000);

    updateBacteriasTimer = new QTimer(this);
    connect(updateBacteriasTimer, SIGNAL(timeout()), this, SLOT(updateBacterias()));
    updateBacteriasTimer->start(5000);

    pausedTime = 0;
    justPaused = true;

    greyForeground = new QWidget();
    greyForeground->setStyleSheet("background-color: rgba(105, 105, 105, 100);");
    greyForeground->setFixedHeight(this->height());
    greyForeground->setFixedWidth(this->width());
    addWidget(greyForeground);
    greyForeground->hide();

    unpause = new QPushButton("Unpause");
    unpause->move(this->width() / 2 - 30, this->height() / 2 - 20);
    addWidget(unpause);
    unpause->hide();

    quit = new QPushButton("Quit");
    quit->move(this->width() / 2 - 30, this->height() / 2 + 10);
    addWidget(quit);
    quit->hide();

    QObject::connect(unpause, SIGNAL(clicked()), SLOT(unpauseClicked()));
    QObject::connect(quit, SIGNAL(clicked()), SLOT(quitClicked()));

    checkGameStateTimer = new QTimer(this);
    connect(checkGameStateTimer, SIGNAL(timeout()), this, SLOT(checkGameState()));
    checkGameStateTimer->start(100);

    time = new QTime();
    time->start();

    timeUpdater = new QTimer(this);
    updateTimer();
    connect(timeUpdater, SIGNAL(timeout()), this, SLOT(updateTimer()));
    timeUpdater->start(500);
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
}

void Game1Scene::updateItems(){
    if (updateItemsTimer->isSingleShot()) {
        updateItemsTimer->setSingleShot(false);
        updateItemsTimer->start(3000);
    }

    int random_number = (rand() % 2) + 1;

    for (int i = 0; i < 10; i++) {
        if (healthyItems[i] != NULL && healthyItems[i]->toDelete) {
            delete healthyItems[i];
            healthyItems[i] = NULL;
        }
    }
    for (int i = 0; i < 10; i++) {
        if (unhealthyItems[i] != NULL && unhealthyItems[i]->toDelete) {
            delete unhealthyItems[i];
            unhealthyItems[i] = NULL;
        }
    }

    if(random_number==1) {
        if (healthyItemsIndex >= 10) {
            healthyItemsIndex = 0;
        }
        healthyItems[healthyItemsIndex] = new HealthyItem(aquarium, spongeBob);
        addItem(healthyItems[healthyItemsIndex++]);
    } else if(random_number==2) {
        if (unhealthyItemsIndex >= 10) {
            unhealthyItemsIndex = 0;
        }
        unhealthyItems[unhealthyItemsIndex] = new UnhealthyItem(aquarium, spongeBob);
        addItem(unhealthyItems[unhealthyItemsIndex++]);
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
    saveProgress();

    QFile saveFile(QStringLiteral("Data.json"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        Message *msg = new Message("Couldn't open data file to save.");
        msg->show();
    }

    QJsonDocument saveDoc(usersFile);
    saveFile.write(saveDoc.toJson());

    views()[0]->close();
    GameOnePage *gameOnePage = new GameOnePage(widget, 1, user, usersFile);
    widget->show();
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
                // If no users have ever been created before, create new list and create the user
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

    // Add healthyitems fields
    QJsonArray healthyItems;

    for (int i = 0; i < 10; i++) {
        if (this->healthyItems[i] != NULL) {
            QJsonObject currentHealthyItem;

            currentHealthyItem["x"] = this->healthyItems[i]->x();
            currentHealthyItem["y"] = this->healthyItems[i]->y();
            currentHealthyItem["type"] = this->healthyItems[i]->type;

            healthyItems.append(currentHealthyItem);
        }
    }

    if (!healthyItems.empty()) {
        saveObject["healthyItems"] = healthyItems;
    }

    // Add unhealthyitems fields
    QJsonArray unhealthyItems;

    for (int i = 0; i < 10; i++) {
        if (this->unhealthyItems[i] != NULL) {
            QJsonObject currentUnhealthyItem;

            currentUnhealthyItem["x"] = this->unhealthyItems[i]->x();
            currentUnhealthyItem["y"] = this->unhealthyItems[i]->y();
            currentUnhealthyItem["type"] = this->unhealthyItems[i]->type;

            unhealthyItems.append(currentUnhealthyItem);
        }
    }

    if (!unhealthyItems.empty()) {
        saveObject["unhealthyItems"] = unhealthyItems;
    }
}


void Game1Scene::updateBacterias() {
    if (updateBacteriasTimer->isSingleShot()) {
        updateBacteriasTimer->setSingleShot(false);
        updateBacteriasTimer->start(5000);
    }

    for (int i = 0; i < 20; i++) {
        if (bacterias[i] != NULL && bacterias[i]->toDelete) {
            delete bacterias[i];
            bacterias[i] = NULL;
        }
    }

    if (bacteriasIndex >= 17) {
        bacteriasIndex = 0;
    }

    bacterias[bacteriasIndex] = new Bacteria(1,spongeBob,aquarium,greenColorItem,pixmapLifeList);
    addItem(bacterias[bacteriasIndex++]);
    bacterias[bacteriasIndex] = new Bacteria(2,spongeBob,aquarium,greenColorItem,pixmapLifeList);
    addItem(bacterias[bacteriasIndex++]);
    bacterias[bacteriasIndex] = new Bacteria(3,spongeBob,aquarium,greenColorItem,pixmapLifeList);
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

            timeUpdater->stop();
            updateItemsTimer->stop();
            updateBacteriasTimer->stop();

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
            updateItemsTimer->setSingleShot(true);
            updateBacteriasTimer->setSingleShot(true);

            timeUpdater->start(pausedTimeUpdater);
            updateItemsTimer->start(pausedUpdateItemsTimer);
            updateBacteriasTimer->start(pausedUpdateBacteriasTimer);

            justPaused = true;

            greyForeground->hide();
            unpause->hide();
            quit->hide();
        }
    }

    spongeBob->setFocus();

    // Update score
    scoreLabel->setText(QStringLiteral("Score: %1").arg(aquarium->score));
    scoreLabel->adjustSize();

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

    if (result) {
        aquarium->score += aquarium->maxTime / aquarium->currentTime - 1;
    }
}
