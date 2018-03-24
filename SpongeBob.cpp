#include "SpongeBob.h"

SpongeBob::SpongeBob(Aquarium* aquarium, QGraphicsPixmapItem *needle, QGraphicsPixmapItem** pixmapLifeList, QObject *parent) : QObject(parent)
{
    this->aquarium = aquarium;
    this->pixmapLifeList = pixmapLifeList;
    this->needle=needle;

    vulnerable=false;

    setPixmap((QPixmap("bob1.png")).scaled(80,80));
    setPos(500,100);

    this->immunityLevel=1;
    this->savedImmunityLevel=-1;
    this->savedImmunityLevelDegree=-1;
    this->unchangeableImmunityLevel=false;
    this->immunityLevelDegree=1;
    this->lives=3;
    this->canCollide = true;
    this->blinkerStatus = false;

    this->numCollisionsWithBacterias=new int[3];
    this->numCollisionsWithBacterias[0] = 0;
    this->numCollisionsWithBacterias[1] = 0;
    this->numCollisionsWithBacterias[2] = 0;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(changeGlow()));
    timer->start(100);

    collisionTimer = new QTimer(this);
    collisionTimer->setSingleShot(true);
    connect(collisionTimer, SIGNAL(timeout()), this, SLOT(setCanCollide()));

    collisionBlinker = new QTimer(this);
    connect(collisionBlinker, SIGNAL(timeout()), this, SLOT(toggleVisibility()));

    vulnerableTimer = new QTimer(this);
    vulnerableTimer->setSingleShot(true);
    connect(vulnerableTimer, SIGNAL(timeout()), this, SLOT(resetVulnerability()));
}

void SpongeBob::reset() {
    // Not resetting numCollisionsWithBacterias

    this->vulnerable=false;
    this->immunityLevel=1;
    this->savedImmunityLevel=-1;
    this->savedImmunityLevelDegree=-1;
    this->unchangeableImmunityLevel=false;
    this->immunityLevelDegree=1;
    this->lives=3;
    this->canCollide = true;
    this->blinkerStatus = false;

    changeGlow();
    setPos(500,100);
}

void SpongeBob::changeGlow(){
    if(immunityLevel==0){
        setPixmap((QPixmap("bob.png")).scaled(80,80));
    }
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
    if (event->key() == Qt::Key_Escape) {
        if (aquarium->gamePaused && !aquarium->requestForUnpause) {
            aquarium->requestForUnpause = true;
        } else if (aquarium->gamePaused && aquarium->requestForUnpause) {
            aquarium->requestForUnpause = false;
        } else {
            aquarium->gamePaused = !aquarium->gamePaused;
        }
    }
    if (!aquarium->gamePaused) {
        if (event->key() == Qt::Key_Right && x()+10 < 930)
            setPos(x()+10,y());
        if (event->key() == Qt::Key_Left && x()-10 > -30)
            setPos(x()-10,y());
        if (event->key() == Qt::Key_Up && y()-10 > 80)
            setPos(x(),y()-10);
        if (event->key() == Qt::Key_Down && y()+10 < 470)
            setPos(x(),y()+10);
    }
}

void SpongeBob::toggleVisibility() {
    if (!blinkerStatus) {
        setPixmap((QPixmap("bob1.png")).scaled(0,0));
    } else {
        setPixmap((QPixmap("bob1.png")).scaled(80,80));
    }
    blinkerStatus = !blinkerStatus;
}

void SpongeBob::setCanCollide() {
    canCollide = true;
    collisionBlinker->stop();
    if (blinkerStatus == true) {
        toggleVisibility();
    }
}

void SpongeBob::collisionWithBacteria(int bacteriaType){
    // Update lives
    if(lives > 0) {
        numCollisionsWithBacterias[bacteriaType]++;

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
            toggleVisibility();
            collisionBlinker->start(500);
        }
    }
}

void SpongeBob::setVulnerable(int type) {
    if (type == 1) {
        vulnerable = true;
        savedImmunityLevel = immunityLevel;
        savedImmunityLevelDegree = immunityLevelDegree;
        immunityLevel = 0;
        immunityLevelDegree = 0;
        unchangeableImmunityLevel = true;

        changeGlow();
        needle->setRotation(0);

        vulnerableTimer->start(5000);
    } else if (type == 2) {
        immunityLevel = 1;
        immunityLevelDegree = 0;

        needle->setRotation(0);
    } else if (type == 3) {
        lives = 0;
    }
}

void SpongeBob::resetVulnerability() {
    vulnerable = false;
    immunityLevel = savedImmunityLevel;
    immunityLevelDegree = savedImmunityLevelDegree;
    unchangeableImmunityLevel = false;
    int steps = aquarium->levels[aquarium->level]["stepsPerImmunity"];
    if(immunityLevel==1){
        needle->setRotation((48/steps) * immunityLevelDegree);
    }else if(immunityLevel==2){
        needle->setRotation(48+(80/steps) * immunityLevelDegree);
    }else{
        needle->setRotation(80+48+(48/steps) * immunityLevelDegree);
    }


}
