#include "Virus.h"
/**
 *\file Virus.cpp
 *@brief contains Virus class definition which represents the virus floating in the aquarium
 *
 * Virus class represents the virus object
 *
 */

/**
 * @brief Virus::Virus constructor of Virus class
 *
 * A constructor that set the bacteria along with its attributes
 * @param int type argument integer indicating the type of the bacteria
 * @param SpongeBob* spongeBob is the spongebob object of the active game level
 * @param Aquarium* aquarium
 */
Virus::Virus(int type,SpongeBob* spongeBob,Aquarium* aquarium,QObject *parent)
{
    srand(QTime::currentTime().msec());

    this->spongeBob=spongeBob;
    this->aquarium=aquarium;
    this->type=type;
    this->justPaused = true;
    this->toDelete = false;

    if (this->type == 1) {
        this->speed = (rand() % 100) + this->aquarium->levels[this->aquarium->level]["virusSpeed1"] - 50;
    } else if (this->type == 2) {
        this->speed = (rand() % 100) + this->aquarium->levels[this->aquarium->level]["virusSpeed2"] - 50;
    } else if (this->type == 3) {
        this->speed = this->aquarium->levels[this->aquarium->level]["virusSpeed3"];
    }

    if(type==1){
        imageName = ":virus1";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(60,60));
    }else if (type==2){
        imageName = ":virus2";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(60,60));
    }else if (type==3){
        imageName = ":pestilence";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(120,120));
    }
    baseY = (rand() % 300) + 250;
    setPos(0, baseY);

    speedTimer = new QTimer(this);
    connect(speedTimer, SIGNAL(timeout()), this, SLOT(update()));
    speedTimer->start(this->speed);

    checkGameStateTimer = new QTimer(this);
    connect(checkGameStateTimer, SIGNAL(timeout()), this, SLOT(checkGameState()));
    checkGameStateTimer->start(100);
}

/**
 * @brief Virus::checkGameState triggered on checkGameStateTimer to check the game state and update
 * accordingly
 * A function that check if the game is paused to stop moving the Virus and checks for collisions
 * and accordingly making spongebob vulnerable and mark the virus with toDelete as true
 *
 */
void Virus::checkGameState() {
    // Check if game paused
    if (aquarium->gamePaused) {
        if (justPaused) {
            speedTimer->stop();

            justPaused = false;
        }

        return;
    } else {
        if (!justPaused) {
            speedTimer->start(this->speed);

            justPaused = true;
        }
    }

    if(!scene()->collidingItems(this).isEmpty()){
        QList<QGraphicsItem*> collisions = scene()->collidingItems(this);
        for (int i = 0; i < collisions.size(); i++) {
            if (collisions.at(i)->hasFocus()) {
                //make SpongeBob vulnerable to all bacteria
                spongeBob->setVulnerable(type);

                //delete this Virus
                toDelete = true;
                speedTimer->stop();
                checkGameStateTimer->stop();
                return;
            }
        }
    }
}

/**
 * @brief Virus::update triggered on speedTimer to update the place and movement of the items
 *
 * update() function move the virus in a sinusoidal movementand delete them on at the end of the screen.
 *
 */
void Virus::update(){
    if (x() + 30 >= 950) {
        toDelete = true;
        speedTimer->stop();
        checkGameStateTimer->stop();
        return;
    } else {
        if (this->type == 3) {
            setPos(x()+30, baseY +60*qSin(x()+30));
        } else {
            setPos(x()+30, baseY +20*qSin(x()+30));
        }
    }
}
