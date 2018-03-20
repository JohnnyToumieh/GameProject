#include "Game1Scene.h"
#include "SpongeBob.h"
#include "HealthyItem.h"
#include "UnhealthyItem.h"
Game1Scene::Game1Scene(QGraphicsScene *parent) : QGraphicsScene(parent)
{
    setBackgroundBrush(QBrush(QImage("background2.JPG").scaledToHeight(600).scaledToWidth(1000)));
    setSceneRect(0,0,1000,600);

    timeLabel = new QLabel();
    timeLabel->setStyleSheet("QLabel { background-color : black; color : white; font: 40px; }");
    timeLabel->move(this->width() / 2 - 20, 25);
    addWidget(timeLabel);

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

    spongeBob = new SpongeBob(pixmapNeedle, pixmapLifeList);
    addItem(spongeBob);

    spongeBob->setFlag(QGraphicsItem::ItemIsFocusable);
    spongeBob->setFocus();

    bacteria = new Bacteria(1,this->spongeBob,pixmapLifeList);
    addItem(bacteria);
    bacteria = new Bacteria(2,this->spongeBob,pixmapLifeList);
    addItem(bacteria);
    bacteria = new Bacteria(3,this->spongeBob,pixmapLifeList);
    addItem(bacteria);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(3000);

    time = new QTime();
    time->start();

    updateTimer();
    timeUpdater = new QTimer(this);;
    connect(timeUpdater, SIGNAL(timeout()), this, SLOT(updateTimer()));
    timeUpdater->start(500);
}

void Game1Scene::updateTimer() {
    int secs = time->elapsed() / 1000;
    int mins = (secs / 60) % 60;
    secs = secs % 60;
    timeLabel->setText(QString("%1:%2")
    .arg(mins, 2, 10, QLatin1Char('0'))
    .arg(secs, 2, 10, QLatin1Char('0')) );

    if (time->elapsed() >= 300000) {
        this->gameOver(false);
    }
}

void Game1Scene::update(){
    int random_number= (rand()%2)+1;

    if(random_number==1){
        HealthyItem *healthyItem = new HealthyItem(this->spongeBob);
        addItem(healthyItem);
    }

    else if(random_number==2){
        UnhealthyItem *unhealthyItem = new UnhealthyItem(this->spongeBob);
        addItem(unhealthyItem);
    }
}

void Game1Scene::gameOver(bool result) {
    timeUpdater->stop();
}
