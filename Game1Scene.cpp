#include "Game1Scene.h"

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

    bacterias = new Bacteria*[100];
    bacteriasIndex = 0;

    bacterias[bacteriasIndex] = new Bacteria(1,spongeBob,aquarium,greenColorItem,pixmapLifeList);
    addItem(bacterias[bacteriasIndex++]);
    bacterias[bacteriasIndex] = new Bacteria(2,spongeBob,aquarium,greenColorItem,pixmapLifeList);
    addItem(bacterias[bacteriasIndex++]);
    bacterias[bacteriasIndex] = new Bacteria(3,spongeBob,aquarium,greenColorItem,pixmapLifeList);
    addItem(bacterias[bacteriasIndex++]);

    healthyItems = new HealthyItem*[10];
    healthyItemsIndex = 0;
    unhealthyItems = new UnhealthyItem*[10];
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
    views()[0]->close();
    GameOnePage *gameOnePage = new GameOnePage(widget, 1, user, usersFile);
    widget->show();
}

void Game1Scene::updateBacterias() {
    if (updateBacteriasTimer->isSingleShot()) {
        updateBacteriasTimer->setSingleShot(false);
        updateBacteriasTimer->start(5000);
    }

    if (bacteriasIndex >= 97) {
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
