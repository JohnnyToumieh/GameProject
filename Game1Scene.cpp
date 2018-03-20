#include "Game1Scene.h"
#include "SpongeBob.h"
#include "HealthyItem.h"
#include "UnhealthyItem.h"
Game1Scene::Game1Scene(QGraphicsScene *parent) : QGraphicsScene(parent)
{
    aquarium = new Aquarium(1, 10, 1, 0, 1, 300000, 0, 0);

    setBackgroundBrush(QBrush(QImage("background2.JPG").scaledToHeight(600).scaledToWidth(1000)));
    setSceneRect(0,0,1000,600);

    timeLabel = new QLabel();
    timeLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 40px; }");
    timeLabel->move(this->width() / 2 - 20, 25);
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

    spongeBob = new SpongeBob(pixmapNeedle, pixmapLifeList);
    addItem(spongeBob);

    spongeBob->setFlag(QGraphicsItem::ItemIsFocusable);
    spongeBob->setFocus();

    Bacteria* bacteria = new Bacteria(1,spongeBob,aquarium,greenColorItem,pixmapLifeList);
    addItem(bacteria);
    bacteria = new Bacteria(2,spongeBob,aquarium,greenColorItem,pixmapLifeList);
    addItem(bacteria);
    bacteria = new Bacteria(3,spongeBob,aquarium,greenColorItem,pixmapLifeList);
    addItem(bacteria);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateItems()));
    timer->start(3000);

    QTimer *timer3 = new QTimer(this);
    connect(timer3, SIGNAL(timeout()), this, SLOT(updateBacterias()));
    timer3->start(5000);

    time = new QTime();
    time->start();

    updateTimer();
    timeUpdater = new QTimer(this);;
    connect(timeUpdater, SIGNAL(timeout()), this, SLOT(updateTimer()));
    timeUpdater->start(500);

    QTimer* timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(checkGameState()));
    timer2->start(100);
}

void Game1Scene::updateTimer() {
    int secs = time->elapsed() / 1000;
    int mins = (secs / 60) % 60;
    secs = secs % 60;

    timeLabel->setText(QString("%1:%2")
    .arg(mins, 2, 10, QLatin1Char('0'))
    .arg(secs, 2, 10, QLatin1Char('0')) );

    aquarium->currentTime = time->elapsed();
}

void Game1Scene::updateItems(){
    int random_number = (rand() % 2) + 1;

    if(random_number==1) {
        HealthyItem *healthyItem = new HealthyItem(spongeBob);
        addItem(healthyItem);
    } else if(random_number==2) {
        UnhealthyItem *unhealthyItem = new UnhealthyItem(spongeBob);
        addItem(unhealthyItem);
    }
}

void Game1Scene::updateBacterias() {
    Bacteria* bacteria = new Bacteria(1,spongeBob,aquarium,greenColorItem,pixmapLifeList);
    addItem(bacteria);
    bacteria = new Bacteria(2,spongeBob,aquarium,greenColorItem,pixmapLifeList);
    addItem(bacteria);
    bacteria = new Bacteria(3,spongeBob,aquarium,greenColorItem,pixmapLifeList);
    addItem(bacteria);
}

void Game1Scene::checkGameState() {
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
