#include "SpongeBob.h"
/**
 *\file SpongeBob.cpp
 *@brief contains SpongeBob class definition which represents spongeBob moved by the user
 *  in the aquarium
 *
 *
 */

/**
 * @brief SpongeBob::SpongeBob constructor of SpongeBob class
 *
 * A constructor that ceates SpongeBob along with his attributes
 * @param Aquarium* aquarium
 * @param QGraphicsPixmapItem *needle is the needle shape on the immunity meter
 * @param QGraphicsPixmapItem** pixmapLifeList list of heart shapes (lives) spongebob has
 * @param QObject *parent
 */

SpongeBob::SpongeBob(Aquarium* aquarium, QGraphicsPixmapItem *needle, QGraphicsPixmapItem** pixmapLifeList, QObject *parent) : QObject(parent)
{
    this->aquarium = aquarium;
    this->pixmapLifeList = pixmapLifeList;
    this->needle=needle;

    vulnerable=false;

    this->immunityLevel=1;
    this->savedImmunityLevel=-1;
    this->savedImmunityLevelDegree=-1;
    this->unchangeableImmunityLevel=false;
    this->immunityLevelDegree=1;
    this->lives=3;
    this->canCollide = true;
    this->blinkerStatus = false;

    this->speed = 10;

    changeGlow();
    setPos(this->aquarium->width / 2 - (this->aquarium->width / 12.5) / 2, this->aquarium->height / 6);

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

/**
 * @brief SpongeBob::reset function that resets all attributes of spongebob back to defualt
 *
 *
 */
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
    setPos(this->aquarium->width / 2 - (this->aquarium->width / 12.5) / 2, this->aquarium->height / 6);
}

/**
 * @brief SpongeBob::changeGlow function that checks for immunity level of spongebob
 * and change his glow color accordingly
 *
 *
 */
void SpongeBob::changeGlow(){
    if(immunityLevel==0){
        imageName = ":spongeBob0";
    }
    if(immunityLevel==1){
        imageName = ":spongeBob1";
    }
    if(immunityLevel==2){
        imageName = ":spongeBob2";
    }
    if(immunityLevel==3){
        imageName = ":spongeBob3";
    }
    if (canCollide) {
        setPixmap((QPixmap(imageName)).scaled(this->aquarium->width / 12.5, this->aquarium->width / 12.5));
    }
}

/**
 * @brief SpongeBob::changeGlow: changes spongebpb glow color
 *
 * function that checks for immunity level of spongebob
 * and change his glow color accordingly (green for level 1, yellow 2, red 3(highest))
 */
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
        if (event->key() == Qt::Key_Right) {
            int newX = x() + speed;
            (newX > aquarium->width - aquarium->width / 12.5) ? setX(aquarium->width - aquarium->width / 12.5) : setX(newX);
        }
        if (event->key() == Qt::Key_Left) {
            int newX = x() - speed;
            (newX < 0) ? setX(0) : setX(newX);
        }
        if (event->key() == Qt::Key_Up) {
            int newY = y() - speed;
            (newY < this->aquarium->height / 6) ? setY(this->aquarium->height / 6) : setY(newY);
        }
        if (event->key() == Qt::Key_Down) {
            int newY = y() + speed;
            (newY > aquarium->height * 11 / 12 - aquarium->width / 12.5) ? setY(aquarium->height * 11 / 12 - aquarium->width / 12.5) : setY(newY);
        }
    }
}

/**
 * @brief SpongeBob::toggleVisibility: make spongebob visible then unvisible
 *
 * function that used when spongebob hits a bacteria and loose a live
 * then the shape blink using this function
 */
void SpongeBob::toggleVisibility() {
    if (!blinkerStatus) {
        setPixmap((QPixmap(imageName)).scaled(0,0));
    } else {
        setPixmap((QPixmap(imageName)).scaled(this->aquarium->width / 12.5, this->aquarium->width / 12.5));
    }
    blinkerStatus = !blinkerStatus;
}

/**
 * @brief SpongeBob::setCanCollide: make spongebob can collide
 *
 * function that reset spongebob to state where he can collide after being stopped
 * when he lose a life
 */
void SpongeBob::setCanCollide() {
    canCollide = true;
    collisionBlinker->stop();
    if (blinkerStatus == true) {
        toggleVisibility();
    }
}

/**
 * @brief SpongeBob::collisionWithBacteria: triggered when spongebob collide with bacteria
 * stronger than him
 *
 * function that responds to spongebob collision with a bacteria stronger than him
 * one live is removed and spongebob set to blink.
 * @param int bacteriaType represents type of the bacteria collided with
 */
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

/**
 * @brief SpongeBob::setVulnerable: triggered when spongebob collide with a virus
 *
 * function that responds to spongebob collision with a virus
 * he is then made vulnerable to all kinds of bacteria
 * @param int type represents type of the virus collided with
 */
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

/**
 * @brief SpongeBob::resetVulnerability:
 *
 * function that is called to restore spongebob immunity (5 seconds after collision with virus)
 * @param int type represents type of the virus collided with
 */
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
