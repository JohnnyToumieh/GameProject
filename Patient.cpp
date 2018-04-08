#include "Patient.h"

/**
 *\file Patient.cpp
 *@brief contains Patient class definition which represents the patient floating in the aquarium
 *
 * Patient class represents the patient object with its different attributes
 * such as type and position...
 *
 */

/**
 * @brief Patient::Patient constructor of Patient class
 *
 * A constructor that set the patient along with its attributes
 * @param int type argument integer indicating the type of the patient
 * @param SpongeBob* spongeBob is the spongebob object of the active game level
 * @param Aquarium* aquarium
 * @param QGraphicsPixmapItem* greenColorItem is the cleanliness green bar
 * @param QGraphicsPixmapItem** pixmapLifeList the array of spongebob's lives
 */

Patient::Patient(int type, Office* office, QObject *parent)
{
    srand(QTime::currentTime().msec());

    this->type=type;
    this->office=office;

    this->justPaused = true;
    this->toDelete = false;

    if(type==1){
        imageName = ":bacteria1";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(80,80));
    }
    else if(type==2){
        imageName = ":bacteria2";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(80,80));
    }else{
        imageName = ":bacteria3";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(80,80));
    }

    baseY = (rand() % 400) + 100;
    setPos(949, baseY);

    this->speed = (rand() % 100) + 100;

    speedTimer = new QTimer(this);
    connect(speedTimer, SIGNAL(timeout()), this, SLOT(update()));
    speedTimer->start(this->speed);

    checkGameStateTimer = new QTimer(this);
    connect(checkGameStateTimer, SIGNAL(timeout()), this, SLOT(checkGameState()));
    checkGameStateTimer->start(100);
}

/**
 * @brief Patient::checkGameState triggered on checkGameStateTimer to check the game state and update
 * accordingly
 *
 * A function that check if the game is paused to stop moving the patient and checks for collisions
 * and accordingly modify attributes (score,cleanliness) and mark the patient with toDelete as true
 *
 */
void Patient::checkGameState() {
    // Check if game paused
    if (office->gamePaused) {
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
}

/**
 * @brief Patient::checkGameState triggered on speedTimer to update the place and movement of the patient
 *
 * update() function move the patient in a sinusoidal movement and delete them on borders. When spongebob
 * is vulnerable then patient chase him.
 *
 */

void Patient::update(){
    if (x() - 30 <= 0) {
        toDelete = true;
        speedTimer->stop();
        checkGameStateTimer->stop();
        return;
    } else {
        setPos(x() - 30, baseY + 20*qSin(x()+30));
    }
}
