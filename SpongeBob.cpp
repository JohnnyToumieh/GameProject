#include "SpongeBob.h"
#include <QTimer>
SpongeBob::SpongeBob(QGraphicsPixmapItem *needle, QGraphicsPixmapItem** pixmapLifeList, QObject *parent) : QObject(parent)
{
    this->pixmapLifeList=pixmapLifeList;

    setPixmap((QPixmap("bob1.png")).scaled(80,80));
    setPos(500,100);

    this->needle=needle;
    this->immunityLevel=1;
    this->immunityLevelDegree=1;
    this->lives=3;
    this->canCollide = true;
    this->blinkerStatus = false;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(changeGlow()));
    timer->start(500);

    collisionTimer = new QTimer(this);
    collisionTimer->setSingleShot(true);
    connect(collisionTimer, SIGNAL(timeout()), this, SLOT(setCanCollide()));

    collisionBlinker = new QTimer(this);
    connect(collisionBlinker, SIGNAL(timeout()), this, SLOT(toggleVisibility()));
}

void SpongeBob::changeGlow(){
    if(immunityLevel==1){
        setPixmap((QPixmap("bob1.png")).scaled(80,80));
    }
    if(immunityLevel==2){
        setPixmap((QPixmap("bob2.png")).scaled(80,80));
    }
    if(immunityLevel==3){
        setPixmap((QPixmap("bob3.png")).scaled(80,80));
    }
}

void SpongeBob::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Right && x()+10 < 930)
        setPos(x()+10,y());
    if (event->key() == Qt::Key_Left && x()-10 > -30)
        setPos(x()-10,y());
    if (event->key() == Qt::Key_Up && y()-10 > 0)
        setPos(x(),y()-10);
    if (event->key() == Qt::Key_Down && y()+10 < 550)
        setPos(x(),y()+10);
}

void SpongeBob::toggleVisibility() {
    if (!blinkerStatus) {
        setPixmap((QPixmap("bob1.png")).scaled(0,0));
    } else {
        setPixmap((QPixmap("bob1.png")).scaled(80,80));
    }
    blinkerStatus = !blinkerStatus;

    if (collisionBlinker->isSingleShot()) {
        collisionBlinker->setSingleShot(false);
        collisionBlinker->start(500);
    }
}

void SpongeBob::setCanCollide() {
    canCollide = true;
    collisionBlinker->stop();
    setPixmap((QPixmap("bob1.png")).scaled(80,80));
    blinkerStatus = false;
}

void SpongeBob::collisionWithBacteria(){
    // Update lives
    if(lives > 0) {
        lives = lives - 1;
        if(lives == 2) {
            scene()->removeItem(pixmapLifeList[0]);
        } else if(lives == 1) {
            scene()->removeItem(pixmapLifeList[1]);
        } else {
            scene()->removeItem(pixmapLifeList[2]);
        }

        canCollide = false;

        if (lives != 0) {
            collisionTimer->start(4000);
            collisionBlinker->setSingleShot(true);
            collisionBlinker->start(1);
        }
    }
}
